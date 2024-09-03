//------------------------------------------------------------------------------
//  @file ternaryexpression.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "ternaryexpression.h"
#include "compiler.h"
#include "util.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
TernaryExpression::TernaryExpression(Expression* lhs, Expression* ifExpression, Expression* elseExpression)
    : lhs(lhs)
    , ifExpression(ifExpression)
    , elseExpression(elseExpression)
{
    this->resolved = new TernaryExpression::__Resolved;
    this->symbolType = TernaryExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
TernaryExpression::~TernaryExpression()
{
    delete this->lhs;
    delete this->ifExpression;
    delete this->elseExpression;
}

//------------------------------------------------------------------------------
/**
*/
bool 
TernaryExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    if (!this->lhs->Resolve(compiler))
        return false;
    if (!this->ifExpression->Resolve(compiler))
        return false;
    if (!this->elseExpression->Resolve(compiler))
        return false;

    Type::FullType type1, type2, type3;
    this->lhs->EvalType(type3);
    this->ifExpression->EvalType(type1);
    this->elseExpression->EvalType(type2);

    if (
    (type3.name != "bool"
        && type3.name != "int"
        && type3.name != "uint")
    || !type3.modifiers.empty())
    {
        compiler->Error(Format("Condition must evaluate to 'bool', 'int' or 'uint', but got '%s'", type3.name.c_str()), this);
        return false;
    }

    if (type1.name != type2.name)
    {
        compiler->Error(Format("Ternary results are of different types '%s' and '%s'", type1.name.c_str(), type2.name.c_str()), this);
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
TernaryExpression::EvalString() const
{
    std::string lhs, ifStr, elseStr;
    lhs = this->lhs->EvalString();
    ifStr = this->ifExpression->EvalString();
    elseStr = this->elseExpression->EvalString();
    return Format("%s ? %s : %s", lhs.c_str(), ifStr.c_str(), elseStr.c_str());
}

} // namespace GPULang
