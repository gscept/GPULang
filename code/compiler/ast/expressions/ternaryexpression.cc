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
    this->lhs->~Expression();
    this->ifExpression->~Expression();
    this->elseExpression->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
bool 
TernaryExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    if (this->lhs == nullptr || this->ifExpression == nullptr || this->elseExpression == nullptr)
        return false;
    
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
    || type3.modifiers.size != 0)
    {
        compiler->Error(Format("Condition must evaluate to 'b8', 'i32' or 'u32', but got '%s'", type3.ToString().c_str()), this);
        return false;
    }

    // Not a bug, if type1 is literal, the first will fail but the other succeed, and vice versa
    if (type1 != type2 && type2 != type1)
    {
        compiler->Error(Format("Ternary results are of different types '%s' and '%s'", type1.ToString().c_str(), type2.ToString().c_str()), this);
        return false;
    }

    // The domain is the amalgamation of all three domains
    // The condition can be different as it is only used for branching
    // The expressions on either side of if or else can influence the domain of the result
    Domain conditionDomain, leftDomain, rightDomain;
    this->lhs->EvalDomain(conditionDomain);
    this->ifExpression->EvalDomain(leftDomain);
    this->elseExpression->EvalDomain(rightDomain);
    Domain resultDomain = Domain::Device;
    thisResolved->domain = PromoteDomain(PromoteDomain(conditionDomain, leftDomain), rightDomain);

    if (!type2.literal)
        type1.literal = false;
    thisResolved->fullType = type1;
    thisResolved->type = compiler->GetType(type1);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
TernaryExpression::EvalValue(ValueUnion& out) const
{
    // Fixme, implement a compile time evaluation path for this
    return Expression::EvalValue(out);
}

//------------------------------------------------------------------------------
/**
*/
bool 
TernaryExpression::EvalDomain(Domain& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->domain;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
TernaryExpression::EvalString() const
{
    TransientString lhs, ifStr, elseStr;
    lhs = this->lhs->EvalString();
    ifStr = this->ifExpression->EvalString();
    elseStr = this->elseExpression->EvalString();
    return TransientString(lhs, " ? ", ifStr, " : ", elseStr);
}

//------------------------------------------------------------------------------
/**
*/
bool
TernaryExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->fullType.name == UNDEFINED_TYPE)
        return false;
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
TernaryExpression::EvalTypeSymbol(Type*& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->type == nullptr)
        return false;
    out = thisResolved->type;
    assert(out->symbolType == Symbol::SymbolType::TypeType || out->symbolType == Symbol::SymbolType::EnumerationType || out->symbolType == Symbol::SymbolType::StructureType);
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
