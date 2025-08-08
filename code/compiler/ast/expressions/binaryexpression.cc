//------------------------------------------------------------------------------
//  binaryexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "binaryexpression.h"

#include "ast/function.h"
#include "ast/expressions/symbolexpression.h"
#include "compiler.h"
#include "boolexpression.h"
#include "boolvecexpression.h"
#include "floatexpression.h"
#include "floatvecexpression.h"
#include "intexpression.h"
#include "intvecexpression.h"
#include "uintexpression.h"
#include "uintvecexpression.h"
#include "util.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
BinaryExpression::BinaryExpression(uint32_t op, Expression* left, Expression* right) 
    : op(op)
    , left(left)
    , right(right)
{
    this->resolved = Alloc<BinaryExpression::__Resolved>();
    this->thisResolved = Symbol::Resolved(this);
    this->thisResolved->leftConversion = nullptr;
    this->thisResolved->rightConversion = nullptr;
    this->thisResolved->lhsType = nullptr;
    this->thisResolved->rhsType = nullptr;
    this->thisResolved->retType = nullptr;
    this->thisResolved->constValueExpression = nullptr;
    this->symbolType = Symbol::BinaryExpressionType;
}


//------------------------------------------------------------------------------
/**
*/
BinaryExpression::~BinaryExpression()
{
    this->left->~Expression();
    this->right->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::Resolve(Compiler* compiler)
{
    if (this->isLhsValue || this->op == '=')
        this->left->isLhsValue = true;
    if (!this->left->Resolve(compiler))
        return false;
    if (!this->right->Resolve(compiler))
        return false;

    this->left->EvalType(this->thisResolved->leftType);
    this->right->EvalType(this->thisResolved->rightType);

    // Disallow binary operators on pointers and arrays
    if (thisResolved->leftType.modifiers.size != 0)
    {
        compiler->Error(Format("Invalid operator '%c' on type '%s'", this->op, this->thisResolved->leftType.ToString().c_str()), this);
        return false;
    }
    
    if (thisResolved->rightType.modifiers.size != 0)
    {
        compiler->Error(Format("Invalid operator '%c' on type '%s'", this->op, this->thisResolved->rightType.ToString().c_str()), this);
        return false;
    }
    
    this->left->EvalTypeSymbol(this->thisResolved->lhsType);
    if (this->thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(this->thisResolved->leftType.name, this);
        return false;
    }
    if (this->thisResolved->lhsType->name == UNDEFINED_TYPE)
    {
        compiler->Error(Format("'%s' can not be used with binary expressions", this->thisResolved->leftType.ToString().c_str()), this);
        return false;
    }
    if (thisResolved->lhsType->name == FUNCTION_TYPE && this->op != '=')
    {
        compiler->Error(Format("functions can not be used with operator '%c'", this->op), this);
        return false;
    }
    
    this->right->EvalTypeSymbol(this->thisResolved->rhsType);
    if (this->thisResolved->rhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(this->thisResolved->rightType.name, this);
        return false;
    }
    if (this->thisResolved->rhsType->name == UNDEFINED_TYPE)
    {
        compiler->Error(Format("'%s' can not be used with binary expressions", this->thisResolved->rightType.ToString().c_str()), this);
        return false;
    }
    if (thisResolved->rhsType->name == FUNCTION_TYPE && this->op != '=')
    {
        compiler->Error(Format("functions can not be used with operator '%c'", this->op), this);
        return false;
    }

    thisResolved->isAssignment = this->op == '=' || this->op == '+=' || this->op == '-=' || this->op == '*=' || this->op == '/=' || this->op == '%=' || this->op == '<<=' || this->op == '>>=' || this->op == '&=' || this->op == '^=' || this->op == '|='; 
    if (thisResolved->isAssignment)
    {
        unsigned leftAccess;
        this->left->EvalAccessFlags(leftAccess);
        if (leftAccess == AccessFlags::Const &&
            !compiler->currentState.allowConstOverride)
        {
            compiler->Error(Format("Assignment illegal on left hand value of 'const'"), this);
            return false;
        }

        if (this->thisResolved->leftType.literal)
        {
            compiler->Error(Format("Assignment illegal on literal"), this);
            return false;
        }
    }

    // If assignment, allow if types are identical
    if (!this->thisResolved->leftType.Assignable(this->thisResolved->rightType) && this->op == '=')
    {
        // If not, or the operator is otherwise, look for conversion assignment or comparison operators
        //TStr functionName = Format("operator%s(%s)", FourCCToString(this->op).c_str(), this->thisResolved->rightType.Name().c_str());
        TransientString functionName = TransientString("operator", FourCCToString(this->op), "(", this->thisResolved->rightType.Name().c_str(), ")");
        Symbol* conversionFunction = this->thisResolved->lhsType->GetSymbol(functionName);
        if (conversionFunction == nullptr)
        {
            compiler->Error(Format("Type '%s' does not implement '%s'", this->thisResolved->leftType.ToString().c_str(), functionName.c_str()), this);
            return false;
        }
        else
        {
            if (compiler->options.disallowImplicitConversion)
            {
                compiler->Error(Format("Implicit conversion between '%s' and '%s' is disallowed. Either turn off 'disallowImplicitConversion' or explicitly convert types", this->thisResolved->leftType.ToString().c_str(), this->thisResolved->rightType.ToString().c_str()), this);
                return false;
            }
        }
        Function* fun = static_cast<Function*>(conversionFunction);
        this->thisResolved->rightConversion = static_cast<Function*>(conversionFunction);
        this->thisResolved->returnType = this->thisResolved->leftType;
    }
    else if (this->op != '=') // If not an assignment, allow promotion of either side of the operator
    {
        TStr functionName = TStr("operator", FourCCToString(this->op), "(", this->thisResolved->rightType.Name(), ")");
        Function* operatorFunction = this->thisResolved->lhsType->GetSymbol<Function>(functionName);
        if (operatorFunction == nullptr)
        {
            if (compiler->options.disallowImplicitPromotion)
            {
                TransientString conversionSignature = TransientString(this->thisResolved->lhsType->name, "(", this->thisResolved->rhsType->name, ")");
                Symbol* conversionFunction = compiler->GetSymbol(conversionSignature);
                compiler->Error(Format("'%s' does not implement '%s'", this->thisResolved->lhsType->name.c_str(), functionName.c_str()), this);
                return false;
            }
            else
            {
                /// Attempt to promote left and right side to the smallest common denominator
                TypeCode promotedType = Type::PromoteTypes(this->thisResolved->lhsType->baseType, this->thisResolved->rhsType->baseType);
                if (promotedType == TypeCode::InvalidType)
                {
                    compiler->Error(Format("Type '%s' could not be promoted to '%s'", this->thisResolved->leftType.ToString().c_str(), this->thisResolved->rightType.ToString().c_str()), this    );
                    return false;
                }
                Type::FullType promotedFullType = Type::TypeFromCode(promotedType, max(this->thisResolved->lhsType->columnSize, this->thisResolved->rhsType->columnSize), max(this->thisResolved->lhsType->rowSize, this->thisResolved->rhsType->rowSize));

                // If we have an assignment, then promotion of the left type is not allowed
                if (thisResolved->isAssignment && promotedFullType != thisResolved->leftType)
                {
                    compiler->Error(Format("Type '%s' does not implement '%s'", this->thisResolved->leftType.ToString().c_str(), functionName.c_str()), this);
                    return false;
                }
                
                TransientString promotedOperatorFunctionName = TransientString("operator", FourCCToString(this->op), "(", promotedFullType.Name(), ")");
                Type* promotedLhsType = compiler->GetType(promotedFullType);
                Function* promotedOperatorFunction = promotedLhsType->GetSymbol<Function>(promotedOperatorFunctionName);
                if (promotedOperatorFunction == nullptr)
                {
                    compiler->Error(Format("'%s' does not implement '%s' when implicitly promoting either side of operator", this->thisResolved->lhsType->name.c_str(), functionName.c_str()), this);
                    return false;
                }
                
                if (promotedFullType != this->thisResolved->leftType)
                {
                    TransientString lhsConversion = TransientString(promotedFullType.Name(), "(", this->thisResolved->leftType.Name(), ")");
                    this->thisResolved->leftConversion = compiler->GetSymbol<Function>(lhsConversion);
                    if (this->thisResolved->leftConversion == nullptr)
                    {
                        compiler->Error(Format("'%s' can't be constructed from '%s' when promoting left hand", promotedFullType.ToString().c_str(), this->thisResolved->lhsType->name.c_str()), this);
                        return false;        
                    }
                    
                }
                if (promotedFullType != this->thisResolved->rightType)
                {
                    TransientString rhsConversion = TransientString(promotedFullType.Name(), "(", this->thisResolved->rightType.Name(), ")");
                    this->thisResolved->rightConversion = compiler->GetSymbol<Function>(rhsConversion);
                    if (this->thisResolved->rightConversion == nullptr)
                    {
                        compiler->Error(Format("'%s' can't be constructed from '%s' when promoting right hand",  promotedFullType.Name().c_str(), this->thisResolved->rhsType->name.c_str()), this);
                        return false;        
                    }
                }
                
                this->thisResolved->returnType = promotedOperatorFunction->returnType;
            }
        }
        else
        {
            Function* fun = static_cast<Function*>(operatorFunction);
            this->thisResolved->returnType = fun->returnType;
            this->thisResolved->leftConversion = nullptr;
            this->thisResolved->rightConversion = nullptr;
        }
    }
    else // directly assignable (same type)
    {
        this->thisResolved->returnType = this->thisResolved->leftType;
    }
    this->thisResolved->retType = compiler->GetType(this->thisResolved->returnType);

    // Test if we can evaluate this expression at compile time
    if (!thisResolved->isAssignment && this->thisResolved->leftType.literal && this->thisResolved->rightType.literal)
    {
        ValueUnion value;
        if (this->EvalValue(value))
        {
#define X(Type, type, ty)\
    if (value.columnSize > 1)\
        this->thisResolved->constValueExpression = Alloc<Type##VecExpression>(FixedArray<ty>(value.type, value.type + value.columnSize));\
    else\
        this->thisResolved->constValueExpression = Alloc<Type##Expression>(value.type[0]);\
        this->thisResolved->constValueExpression->Resolve(compiler);

            switch (value.code)
            {
                VALUE_UNION_SWITCH()
            }
            
            this->thisResolved->returnType.literal = true;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->returnType.name == UNDEFINED_TYPE)
        return false;
    out = thisResolved->returnType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryExpression::EvalTypeSymbol(Type*& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->retType == nullptr)
        return false;
    out = thisResolved->retType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryExpression::EvalSymbol(FixedString& out) const
{
    bool ret = true;
    ret &= this->left->EvalSymbol(out);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryExpression::EvalValue(ValueUnion& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->lhsType == nullptr || thisResolved->rhsType == nullptr)
        return false;
    
    ValueUnion lval, rval;
    if (!(this->left->EvalValue(lval) & this->right->EvalValue(rval)))
        return false;

#define OPERATOR_EXECUTE(mem, op)\
    out.mem[0] = lval.mem[0] op rval.mem[0];\
    if (lval.columnSize > 1)\
        out.mem[1] = lval.mem[1] op rval.mem[1];\
    if (lval.columnSize > 2)\
        out.mem[2] = lval.mem[2] op rval.mem[2];\
    if (lval.columnSize > 3)\
        out.mem[3] = lval.mem[3] op rval.mem[3];\
    if (lval.rowSize > 1)\
    {\
        out.mem[4] = lval.mem[4] op rval.mem[4];\
        if (lval.columnSize > 1)\
            out.mem[5] = lval.mem[5] op rval.mem[5];\
        if (lval.columnSize > 2)\
            out.mem[6] = lval.mem[6] op rval.mem[6];\
        if (lval.columnSize > 3)\
            out.mem[7] = lval.mem[7] op rval.mem[7];\
    }\
    if (lval.rowSize > 2)\
    {\
        out.mem[8] = lval.mem[8] op rval.mem[8];\
        if (lval.columnSize > 1)\
            out.mem[9] = lval.mem[9] op rval.mem[9];\
        if (lval.columnSize > 2)\
            out.mem[10] = lval.mem[10] op rval.mem[10];\
        if (lval.columnSize > 3)\
            out.mem[11] = lval.mem[11] op rval.mem[11];\
    }\
    if (lval.rowSize > 3)\
    {\
        out.mem[12] = lval.mem[12] op rval.mem[12];\
        if (lval.columnSize > 1)\
            out.mem[13] = lval.mem[13] op rval.mem[13];\
        if (lval.columnSize > 2)\
            out.mem[14] = lval.mem[14] op rval.mem[14];\
        if (lval.columnSize > 3)\
            out.mem[15] = lval.mem[15] op rval.mem[15];\
    }

#define OPERATOR_SCALAR(label, op) \
    case label:\
    {\
        switch (this->thisResolved->lhsType->baseType)\
        {\
            case TypeCode::Float:\
            case TypeCode::Float16:\
                OPERATOR_EXECUTE(f, op)\
                break;\
            case TypeCode::Int:\
            case TypeCode::Int16:\
                OPERATOR_EXECUTE(i, op)\
                break;\
            case TypeCode::UInt:\
            case TypeCode::UInt16:\
                OPERATOR_EXECUTE(ui, op)\
                break;\
            default:\
                return false;\
        }\
        break;\
    }
#define OPERATOR_ALL(label, op) \
    case label:\
    {\
        switch (this->thisResolved->lhsType->baseType)\
        {\
            case TypeCode::Bool:\
                OPERATOR_EXECUTE(b, op)\
                break;\
            case TypeCode::Float:\
            case TypeCode::Float16:\
                OPERATOR_EXECUTE(f, op)\
                break;\
            case TypeCode::Int:\
            case TypeCode::Int16:\
                OPERATOR_EXECUTE(i, op)\
                break;\
            case TypeCode::UInt:\
            case TypeCode::UInt16:\
                OPERATOR_EXECUTE(ui, op)\
                break;\
            default:\
                return false;\
        }\
        break;\
    }

    #define OPERATOR_INTEGER(label, op) \
    case label:\
    {\
        switch (this->thisResolved->lhsType->baseType)\
        {\
            case TypeCode::Int:\
            case TypeCode::Int16:\
                OPERATOR_EXECUTE(i, op)\
                break;\
            case TypeCode::UInt:\
            case TypeCode::UInt16:\
                OPERATOR_EXECUTE(ui, op)\
                break;\
            default:\
                return false;\
        }\
        break;\
    }
    
    switch (this->op)
    {
        OPERATOR_ALL('+', +)
        OPERATOR_ALL('-', -)
        OPERATOR_ALL('*', *)
        OPERATOR_SCALAR('/', /)
        OPERATOR_INTEGER('%', %)
        OPERATOR_INTEGER('^', ^)
        OPERATOR_INTEGER('|', |)
        OPERATOR_INTEGER('&', &)
        OPERATOR_INTEGER('>>', >>)
        OPERATOR_INTEGER('<<', <<)
        OPERATOR_INTEGER('||', ||)
        OPERATOR_INTEGER('&&', &&)
        OPERATOR_INTEGER('<', <)
        OPERATOR_INTEGER('>', >)
        OPERATOR_INTEGER('<=', <=)
        OPERATOR_INTEGER('>=', >=)
        OPERATOR_INTEGER('==', ==)
        OPERATOR_INTEGER('!=', !=)
    }

    if (!out.SetType(this->thisResolved->retType))
        return false;

    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
BinaryExpression::EvalString() const
{
    TransientString left, right;
    left = this->left->EvalString();
    right = this->right->EvalString();
    return TransientString(left, FourCCToString(this->op), right);
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::EvalAccessFlags(unsigned& out) const
{
    unsigned left, right;
    this->left->EvalAccessFlags(left);
    this->right->EvalAccessFlags(right);
    out = left & right;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryExpression::EvalStorage(Storage& out) const
{
    // Binary operations that return a value has to be on the stack
    out = Storage::Default;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::EvalBool(int lhs, int rhs) const
{
    if (this->op == '<')
    {
        return lhs < rhs;
    }
    else if (this->op == '>')
    {
        return lhs > rhs;
    }
    else if (this->op == '<=')
    {
        return lhs <= rhs;
    }
    else if (this->op == '>=')
    {
        return lhs >= rhs;
    }
    else if (this->op == '==')
    {
        return lhs == rhs;
    }
    else if (this->op == '!=')
    {
        return lhs != rhs;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::EvalBool(float lhs, int rhs) const
{
    if (this->op == '<')
    {
        return lhs < rhs;
    }
    else if (this->op == '>')
    {
        return lhs > rhs;
    }
    else if (this->op == '<=')
    {
        return lhs <= rhs;
    }
    else if (this->op == '>=')
    {
        return lhs >= rhs;
    }
    else if (this->op == '==')
    {
        return lhs == rhs;
    }
    else if (this->op == '!=')
    {
        return lhs != rhs;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::EvalBool(int lhs, float rhs) const
{
    if (this->op == '<')
    {
        return lhs < rhs;
    }
    else if (this->op == '>')
    {
        return lhs > rhs;
    }
    else if (this->op == '<=')
    {
        return lhs <= rhs;
    }
    else if (this->op == '>=')
    {
        return lhs >= rhs;
    }
    else if (this->op == '==')
    {
        return lhs == rhs;
    }
    else if (this->op == '!=')
    {
        return lhs != rhs;
    }
    else if (this->op == '||')
    {
        return lhs || rhs;
    }
    else if (this->op == '&&')
    {
        return lhs && rhs;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::EvalBool(float lhs, float rhs) const
{
    if (this->op == '<')
    {
        return lhs < rhs;
    }
    else if (this->op == '>')
    {
        return lhs > rhs;
    }
    else if (this->op == '<=')
    {
        return lhs <= rhs;
    }
    else if (this->op == '>=')
    {
        return lhs >= rhs;
    }
    else if (this->op == '==')
    {
        return lhs == rhs;
    }
    else if (this->op == '!=')
    {
        return lhs != rhs;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::EvalBool(bool lhs, bool rhs) const
{
    if (this->op == '==')
    {
        return lhs == rhs;
    }
    else if (this->op == '!=')
    {
        return lhs != rhs;
    }
    return false;
}
} // namespace GPULang
