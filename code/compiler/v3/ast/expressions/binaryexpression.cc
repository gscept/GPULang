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
    this->resolved = new BinaryExpression::__Resolved;
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->conversionFunction = nullptr;
    thisResolved->lhsType = nullptr;
    thisResolved->rhsType = nullptr;
    thisResolved->retType = nullptr;
    this->symbolType = Symbol::BinaryExpressionType;
}


//------------------------------------------------------------------------------
/**
*/
BinaryExpression::~BinaryExpression()
{
    delete this->left;
    delete this->right;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BinaryExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->text = this->EvalString();
    if (this->isLhsValue || this->op == '=')
        this->left->isLhsValue = true;
    if (this->isDeclaration)
        this->left->isDeclaration = true;

    if (!this->left->Resolve(compiler))
        return false;
    if (!this->right->Resolve(compiler))
        return false;

    this->left->EvalType(thisResolved->leftType);
    this->right->EvalType(thisResolved->rightType);

    thisResolved->lhsType = compiler->GetSymbol<Type>(thisResolved->leftType.name);
    if (thisResolved->lhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->leftType.name, this);
        return false;
    }
    thisResolved->rhsType = compiler->GetSymbol<Type>(thisResolved->rightType.name);
    if (thisResolved->rhsType == nullptr)
    {
        compiler->UnrecognizedTypeError(thisResolved->rightType.name, this);
        return false;
    }

    SymbolExpression* lhs = static_cast<SymbolExpression*>(this->left);
    if (lhs->symbolType == Symbol::SymbolType::SymbolExpressionType)
    {
        Variable* leftSymbol = compiler->GetSymbol<Variable>(lhs->symbol);
        if (leftSymbol != nullptr)
        {
            Variable::__Resolved* leftSymbolResolved = Symbol::Resolved(leftSymbol);
            if (leftSymbolResolved->usageBits.flags.isConst && !this->left->isDeclaration)
            {
                compiler->Error(Format("Invalid operator '%c' on const symbol '%s'", this->op, leftSymbol->name.c_str()), this);
                return false;
            }
        }
    }

    // If assignment, allow if types are identical
    if (thisResolved->leftType != thisResolved->rightType)
    {
        // If not, or the operator is otherwise, look for conversion assignment or comparison operators
        std::string functionName = Format("operator%s(%s)", FourCCToString(this->op).c_str(), thisResolved->rightType.name.c_str());
        Symbol* conversionFunction = thisResolved->lhsType->GetSymbol(functionName);
        if (conversionFunction == nullptr)
        {
            compiler->Error(Format("Type '%s' does not implement '%s' with '%s'", thisResolved->lhsType->name.c_str(), functionName.c_str(), thisResolved->rhsType->name.c_str()), this);
            return false;
        }
        Function* fun = static_cast<Function*>(conversionFunction);
        thisResolved->conversionFunction = static_cast<Function*>(conversionFunction);
        thisResolved->returnType = fun->returnType;
    }
    else
    {
        if (this->op == '+')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '-')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '*')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '/')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '%')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '^')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '|')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '&')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '>>')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '<<')
        {
            thisResolved->returnType = thisResolved->rightType;
        }
        else if (this->op == '||')
        {
            thisResolved->returnType = Type::FullType{ "b8" };
        }
        else if (this->op == '&&')
        {
            thisResolved->returnType = Type::FullType{ "b8" };
        }
        else if (this->op == '<')
        {
            thisResolved->returnType = Type::FullType{ "b8" };
        }
        else if (this->op == '>')
        {
            thisResolved->returnType = Type::FullType{ "b8" };
        }
        else if (this->op == '<=')
        {
            thisResolved->returnType = Type::FullType{ "b8" };
        }
        else if (this->op == '>=')
        {
            thisResolved->returnType = Type::FullType{ "b8" };
        }
        else if (this->op == '==')
        {
            thisResolved->returnType = Type::FullType{ "b8" };
        }
        else if (this->op == '!=')
        {
            thisResolved->returnType = Type::FullType{ "b8" };
        }
    }
    
    thisResolved->retType = compiler->GetSymbol<Type>(thisResolved->returnType.name);
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