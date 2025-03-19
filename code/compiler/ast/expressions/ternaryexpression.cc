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
    this->resolved = Alloc<TernaryExpression::__Resolved>();
    this->symbolType = TernaryExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
TernaryExpression::~TernaryExpression()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
TernaryExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->text = this->EvalString();
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
    (type3.name != "b8"
        && type3.name != "i32"
        && type3.name != "u32")
    || !type3.modifiers.empty())
    {
        compiler->Error(Format("Condition must evaluate to 'b8', 'i32' or 'u32', but got '%s'", type3.name.c_str()), this);
        return false;
    }

    if (type1 != type2)
    {
        compiler->Error(Format("Ternary results are of different types '%s' and '%s'", type1.name.c_str(), type2.name.c_str()), this);
        return false;
    }

    thisResolved->fullType = type1;
    thisResolved->type = compiler->GetType(type1);
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

//------------------------------------------------------------------------------
/**
*/
bool
TernaryExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
TernaryExpression::EvalAccessFlags(unsigned& out) const
{
    unsigned left, right;
    this->ifExpression->EvalAccessFlags(left);
    this->elseExpression->EvalAccessFlags(right);
    out = left & right;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
TernaryExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
