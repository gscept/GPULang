//------------------------------------------------------------------------------
//  binaryexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "binaryexpression.h"

#include "ast/function.h"
#include "ast/expressions/symbolexpression.h"
#include "compiler.h"
#include "floatexpression.h"
#include "intexpression.h"
#include "uintexpression.h"
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
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::Resolve(Compiler* compiler)
{
    this->thisResolved->text = this->EvalString();
    if (this->isLhsValue || this->op == '=')
        this->left->isLhsValue = true;
    if (!this->left->Resolve(compiler))
        return false;
    if (!this->right->Resolve(compiler))
        return false;

    this->left->EvalType(this->thisResolved->leftType);
    this->right->EvalType(this->thisResolved->rightType);

    this->thisResolved->lhsType = compiler->GetType(this->thisResolved->leftType);
    if (this->thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(this->thisResolved->leftType.name, this);
        return false;
    }
    this->thisResolved->rhsType = compiler->GetType(this->thisResolved->rightType);
    if (this->thisResolved->rhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(this->thisResolved->rightType.name, this);
        return false;
    }

    bool isAssignment = this->op == '=' || this->op == '+=' || this->op == '-=' || this->op == '*=' || this->op == '/=' || this->op == '%=' || this->op == '<<=' || this->op == '>>=' || this->op == '&=' || this->op == '^=' || this->op == '|='; 
    if (isAssignment)
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
        std::string functionName = Format("operator%s(%s)", FourCCToString(this->op).c_str(), this->thisResolved->rightType.Name().c_str());
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
        std::string functionName = Format("operator%s(%s)", FourCCToString(this->op).c_str(), this->thisResolved->rightType.Name().c_str());
        Function* operatorFunction = this->thisResolved->lhsType->GetSymbol<Function>(functionName);
        if (operatorFunction == nullptr)
        {
            if (compiler->options.disallowImplicitPromotion)
            {
                std::string conversionSignature = Format("%s(%s)", this->thisResolved->lhsType->name.c_str(), this->thisResolved->rhsType->name.c_str());
                Symbol* conversionFunction = compiler->GetSymbol(conversionSignature);
                compiler->Error(Format("'%s' does not implement '%s'", this->thisResolved->lhsType->name.c_str(), functionName.c_str()), this);
                return false;
            }
            else
            {
                /// Attempt to promote left and right side to the smallest common denominator
                TypeCode promotedType = Type::PromoteTypes(this->thisResolved->lhsType->baseType, this->thisResolved->rhsType->baseType);
                Type::FullType promotedFullType = Type::TypeFromCode(promotedType, max(this->thisResolved->lhsType->columnSize, this->thisResolved->rhsType->columnSize), max(this->thisResolved->lhsType->rowSize, this->thisResolved->rhsType->rowSize));

                // If we have an assignment, then promotion of the left type is not allowed
                if (isAssignment && promotedFullType != thisResolved->leftType)
                {
                    compiler->Error(Format("Type '%s' does not implement '%s'", this->thisResolved->leftType.ToString().c_str(), functionName.c_str()), this);
                    return false;
                }
                
                std::string promotedOperatorFunctionName = Format("operator%s(%s)", FourCCToString(this->op).c_str(), promotedFullType.Name().c_str());
                Type* promotedLhsType = compiler->GetType(promotedFullType);
                Function* promotedOperatorFunction = promotedLhsType->GetSymbol<Function>(promotedOperatorFunctionName);
                if (promotedOperatorFunction == nullptr)
                {
                    compiler->Error(Format("'%s' does not implement '%s' when implicitly promoting either side of operator", this->thisResolved->lhsType->name.c_str(), functionName.c_str()), this);
                    return false;
                }
                
                if (promotedFullType != this->thisResolved->leftType)
                {
                    std::string lhsConversion = Format("%s(%s)", promotedFullType.Name().c_str(), this->thisResolved->leftType.Name().c_str());
                    this->thisResolved->leftConversion = compiler->GetSymbol<Function>(lhsConversion);
                    if (this->thisResolved->leftConversion == nullptr)
                    {
                        compiler->Error(Format("'%s' can't be constructed from '%s' when promoting left hand", promotedFullType.ToString().c_str(), this->thisResolved->lhsType->name.c_str()), this);
                        return false;        
                    }
                    
                }
                if (promotedFullType != this->thisResolved->rightType)
                {
                    std::string rhsConversion = Format("%s(%s)", promotedFullType.Name().c_str(), this->thisResolved->rightType.Name().c_str());
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
    if (!isAssignment && this->thisResolved->leftType.literal && this->thisResolved->rightType.literal)
    {
        ValueUnion value;
        if (this->EvalValue(value))
        {
            thisResolved->returnType.literal = true;
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
    out = thisResolved->returnType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryExpression::EvalSymbol(std::string& out) const
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
    ValueUnion lval, rval;
    if (!(this->left->EvalValue(lval) & this->right->EvalValue(rval)))
        return false;

    lval.Convert(this->thisResolved->retType->baseType);
    rval.Convert(this->thisResolved->retType->baseType);
    out.SetType(this->thisResolved->retType);

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

#define OPERATOR_ALL(label, op) \
    case label:\
    {\
        switch (this->thisResolved->retType->baseType)\
        {\
            case TypeCode::Bool:\
                OPERATOR_EXECUTE(b, op)\
                return true;\
            case TypeCode::Float:\
            case TypeCode::Float16:\
                OPERATOR_EXECUTE(f, op)\
                return true;\
            case TypeCode::Int:\
            case TypeCode::Int16:\
                OPERATOR_EXECUTE(i, op)\
                return true;\
            case TypeCode::UInt:\
            case TypeCode::UInt16:\
                OPERATOR_EXECUTE(ui, op)\
                return true;\
            default:\
                break;\
        }\
        break;\
    }

    #define OPERATOR_INTEGER(label, op) \
    case label:\
    {\
        switch (this->thisResolved->retType->baseType)\
        {\
            case TypeCode::Int:\
            case TypeCode::Int16:\
                OPERATOR_EXECUTE(i, op)\
                return true;\
            case TypeCode::UInt:\
            case TypeCode::UInt16:\
                OPERATOR_EXECUTE(ui, op)\
                return true;\
            default:\
                break;\
        }\
        break;\
    }
    
    switch (this->op)
    {
        OPERATOR_ALL('+', +)
        OPERATOR_ALL('-', -)
        OPERATOR_ALL('*', *)
        OPERATOR_ALL('/', /)
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
    return false;
}

//------------------------------------------------------------------------------
/**
*/
std::string 
BinaryExpression::EvalString() const
{
    std::string left, right;
    left = this->left->EvalString();
    right = this->right->EvalString();
    return Format("%s %s %s", left.c_str(), FourCCToString(this->op).c_str(), right.c_str());
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