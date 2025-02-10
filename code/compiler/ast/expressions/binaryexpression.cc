//------------------------------------------------------------------------------
//  binaryexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "binaryexpression.h"
#include "ast/function.h"
#include "ast/expressions/symbolexpression.h"
#include "compiler.h"
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

    this->thisResolved->lhsType = compiler->GetSymbol<Type>(this->thisResolved->leftType.name);
    if (this->thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(this->thisResolved->leftType.name, this);
        return false;
    }
    this->thisResolved->rhsType = compiler->GetSymbol<Type>(this->thisResolved->rightType.name);
    if (this->thisResolved->rhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(this->thisResolved->rightType.name, this);
        return false;
    }

    if (this->op == '=' || this->op == '+=' || this->op == '-=' || this->op == '*=' || this->op == '/=' || this->op == '%=' || this->op == '<<=' || this->op == '>>=' || this->op == '&=' || this->op == '^=' || this->op == '|=')
    {
        unsigned leftAccess;
        this->left->EvalAccessFlags(leftAccess);
        if (leftAccess == AccessFlags::Const &&
            !compiler->currentState.allowConstOverride)
        {
            compiler->Error(Format("Assignment illegal on left hand value of 'const'"), this);
            return false;
        }

        Type::FullType leftType;
        this->left->EvalType(leftType);
        if (leftType.literal)
        {
            compiler->Error(Format("Assignment illegal on literal"), this);
            return false;
        }
    }

    // If assignment, allow if types are identical
    if (!this->thisResolved->leftType.Assignable(this->thisResolved->rightType) && this->op == '=')
    {
        // If not, or the operator is otherwise, look for conversion assignment or comparison operators
        std::string functionName = Format("operator%s(%s)", FourCCToString(this->op).c_str(), this->thisResolved->rightType.name.c_str());
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
    else if (this->op != '=')
    {
        std::string functionName = Format("operator%s(%s)", FourCCToString(this->op).c_str(), this->thisResolved->rightType.name.c_str());
        Symbol* operatorFunction = this->thisResolved->lhsType->GetSymbol(functionName);
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
                std::string promotedOperatorFunctionName = Format("operator%s(%s)", FourCCToString(this->op).c_str(), promotedFullType.name.c_str());
                Type* promotedLhsType = compiler->GetSymbol<Type>(promotedFullType.name);
                Function* promotedOperatorFunction = promotedLhsType->GetSymbol<Function>(promotedOperatorFunctionName);
                if (promotedOperatorFunction == nullptr)
                {
                    compiler->Error(Format("'%s' does not implement '%s' when implicitly promoting either side of operator", this->thisResolved->lhsType->name.c_str(), functionName.c_str()), this);
                    return false;
                }
                
                if (promotedFullType != this->thisResolved->leftType)
                {
                    std::string lhsConversion = Format("%s(%s)", promotedFullType.name.c_str(), this->thisResolved->leftType.name.c_str());
                    this->thisResolved->leftConversion = compiler->GetSymbol<Function>(lhsConversion);
                    if (this->thisResolved->leftConversion == nullptr)
                    {
                        compiler->Error(Format("'%s' can't be constructed from '%s' when promoting left hand", promotedFullType.name.c_str(), this->thisResolved->lhsType->name.c_str()), this);
                        return false;        
                    }
                    
                }
                if (promotedFullType != this->thisResolved->rightType)
                {
                    std::string rhsConversion = Format("%s(%s)", promotedFullType.name.c_str(), this->thisResolved->rightType.name.c_str());
                    this->thisResolved->rightConversion = compiler->GetSymbol<Function>(rhsConversion);
                    if (this->thisResolved->rightConversion == nullptr)
                    {
                        compiler->Error(Format("'%s' can't be constructed from '%s' when promoting right hand",  promotedFullType.name.c_str(), this->thisResolved->rhsType->name.c_str()), this);
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
    else
    {
        this->thisResolved->returnType = this->thisResolved->leftType;
    }
    
    this->thisResolved->retType = compiler->GetSymbol<Type>(this->thisResolved->returnType.name);
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
BinaryExpression::EvalInt(int& out) const
{
    int lVal, rVal;
    this->left->EvalInt(lVal);
    this->right->EvalInt(rVal);

    if (this->op == '+')
    {
        out = lVal + rVal;
    }
    else if (this->op == '-')
    {
        out = lVal - rVal;
    }
    else if (this->op == '*')
    {
        out = lVal * rVal;
    }
    else if (this->op == '/')
    {
        out = lVal / rVal;
    }
    else if (this->op == '%')
    {
        out = lVal % rVal;
    }
    else if (this->op == '^')
    {
        out = lVal ^ rVal;
    }
    else if (this->op == '|')
    {
        out = lVal | rVal;
    }
    else if (this->op == '&')
    {
        out = lVal & rVal;
    }
    else if (this->op == '>>')
    {
        out = lVal >> rVal;
    }
    else if (this->op == '<<')
    {
        out = lVal << rVal;
    }
    else if (this->op == '||')
    {
        out = (int)(lVal || rVal);
    }
    else if (this->op == '&&')
    {
        out = (int)(lVal & rVal);
    }
    else if (this->op == '<')
    {
        out = (int)(lVal < rVal);
    }
    else if (this->op == '>')
    {
        out = (int)(lVal > rVal);
    }
    else if (this->op == '<=')
    {
        out = (int)(lVal <= rVal);
    }
    else if (this->op == '>=')
    {
        out = (int)(lVal >= rVal);
    }
    else if (this->op == '==')
    {
        out = (int)(lVal == rVal);
    }
    else if (this->op == '!=')
    {
        out = (int)(lVal != rVal);
    }
    else
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BinaryExpression::EvalUInt(unsigned& out) const
{
    unsigned lVal, rVal;
    this->left->EvalUInt(lVal);
    this->right->EvalUInt(rVal);

    if (this->op == '+')
    {
        out = lVal + rVal;
    }
    else if (this->op == '-')
    {
        out = lVal - rVal;
    }
    else if (this->op == '*')
    {
        out = lVal * rVal;
    }
    else if (this->op == '/')
    {
        out = lVal / rVal;
    }
    else if (this->op == '%')
    {
        out = lVal % rVal;
    }
    else if (this->op == '^')
    {
        out = lVal ^ rVal;
    }
    else if (this->op == '|')
    {
        out = lVal | rVal;
    }
    else if (this->op == '&')
    {
        out = lVal & rVal;
    }
    else if (this->op == '>>')
    {
        out = lVal >> rVal;
    }
    else if (this->op == '<<')
    {
        out = lVal << rVal;
    }
    else if (this->op == '||')
    {
        out = (unsigned)(lVal || rVal);
    }
    else if (this->op == '&&')
    {
        out = (unsigned)(lVal & rVal);
    }
    else if (this->op == '<')
    {
        out = (unsigned)(lVal < rVal);
    }
    else if (this->op == '>')
    {
        out = (unsigned)(lVal > rVal);
    }
    else if (this->op == '<=')
    {
        out = (unsigned)(lVal <= rVal);
    }
    else if (this->op == '>=')
    {
        out = (unsigned)(lVal >= rVal);
    }
    else if (this->op == '==')
    {
        out = (unsigned)(lVal == rVal);
    }
    else if (this->op == '!=')
    {
        out = (unsigned)(lVal != rVal);
    }
    else
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::EvalFloat(float& out) const
{
    float lVal, rVal;
    this->left->EvalFloat(lVal);
    this->right->EvalFloat(rVal);

    if (this->op == '+')
    {
        out = lVal + rVal;
    }
    else if (this->op == '-')
    {
        out = lVal - rVal;
    }
    else if (this->op == '*')
    {
        out = lVal * rVal;
    }
    else if (this->op == '/')
    {
        out = lVal / rVal;
    }
    else
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::EvalBool(bool& out) const
{
    Type::FullType lhsType, rhsType;
    this->left->EvalType(lhsType);
    this->right->EvalType(rhsType);

    bool ret = true;
    if (lhsType.name == "i32")
    {
        int lVal;
        ret &= this->left->EvalInt(lVal);
        if (rhsType.name == "f32")
        {
            float rVal;
            ret &= this->right->EvalFloat(rVal);
            out = EvalBool(lVal, rVal);
        }
        else if (rhsType.name == "i32")
        {
            int rVal;
            ret &= this->right->EvalInt(rVal);
            out = EvalBool(lVal, rVal);
        }
    }
    else if (lhsType.name == "f32")
    {
        float lVal;
        ret &= this->left->EvalFloat(lVal);
        if (rhsType.name == "i32")
        {
            int rVal;
            ret &= this->right->EvalInt(rVal);
            out = EvalBool(lVal, rVal);
        }
        else if (rhsType.name == "f32")
        {
            float rVal;
            ret &= this->right->EvalFloat(rVal);
            out = EvalBool(lVal, rVal);
        }
    }
    else if (lhsType.name == "b8")
    {
        bool lVal;
        ret &= this->left->EvalBool(lVal);
        if (rhsType.name == "b8")
        {
            bool rVal;
            ret &= this->right->EvalBool(rVal);
            out = EvalBool(lVal, rVal);
        }
    }
    else
    {
        ret = false;
    }
    return ret;
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