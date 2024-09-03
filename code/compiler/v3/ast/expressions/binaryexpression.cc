//------------------------------------------------------------------------------
//  binaryexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "binaryexpression.h"
#include "ast/function.h"
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

    // If assignment, allow if types are identical
    if (this->op == '=')
    {
        if (thisResolved->leftType == thisResolved->rightType)
        {
            thisResolved->returnType = thisResolved->rightType;
            thisResolved->retType = compiler->GetSymbol<Type>(thisResolved->returnType.name);
            return true;
        }
    }

    // If not, or the operator is otherwise, look for conversion assignment or comparison operators
    std::string functionName = Format("operator%s", FourCCToString(this->op).c_str());
    std::vector<Symbol*> functions = thisResolved->lhsType->GetSymbols(functionName);
    if (functions.empty())
    {
        compiler->Error(Format("Type '%s' does not implement '%s' with '%s'", thisResolved->lhsType->name.c_str(), functionName.c_str(), thisResolved->rhsType->name.c_str()), this);
        return false;
    }
    else
    {
        Symbol* matchingOverload = Function::MatchOverload(compiler, functions, { thisResolved->rightType });
        if (matchingOverload != nullptr)
        {
            Function* fun = static_cast<Function*>(matchingOverload);
            thisResolved->returnType = fun->returnType;
            thisResolved->retType = compiler->GetSymbol<Type>(thisResolved->returnType.name);
        }
        else
        {
            compiler->Error(Format("No known overload for '%s' with types '%s' and '%s'", functionName.c_str(), thisResolved->leftType.name.c_str(), thisResolved->rightType.name.c_str()), this);
            return false;
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
    if (lhsType.name == "int")
    {
        int lVal;
        ret &= this->left->EvalInt(lVal);
        if (rhsType.name == "float")
        {
            float rVal;
            ret &= this->right->EvalFloat(rVal);
            out = EvalBool(lVal, rVal);
        }
        else if (rhsType.name == "int")
        {
            int rVal;
            ret &= this->right->EvalInt(rVal);
            out = EvalBool(lVal, rVal);
        }
    }
    else if (lhsType.name == "float")
    {
        float lVal;
        ret &= this->left->EvalFloat(lVal);
        if (rhsType.name == "int")
        {
            int rVal;
            ret &= this->right->EvalInt(rVal);
            out = EvalBool(lVal, rVal);
        }
        else if (rhsType.name == "float")
        {
            float rVal;
            ret &= this->right->EvalFloat(rVal);
            out = EvalBool(lVal, rVal);
        }
    }
    else if (lhsType.name == "bool")
    {
        bool lVal;
        ret &= this->left->EvalBool(lVal);
        if (rhsType.name == "bool")
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