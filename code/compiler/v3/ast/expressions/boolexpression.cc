//------------------------------------------------------------------------------
//  boolexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "boolexpression.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
BoolExpression::BoolExpression(bool value) :
    value(value)
{
    this->resolved = new BoolExpression::__Resolved;
    this->symbolType = BoolExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
BoolExpression::~BoolExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
BoolExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ "b8" };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetSymbol<Type>(thisResolved->fullType.name);
    thisResolved->text = this->EvalString();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalSymbol(std::string& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalInt(int& out) const
{
    out = (int)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BoolExpression::EvalUInt(unsigned& out) const
{
    out = (unsigned) this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalFloat(float& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalBool(bool& out) const
{
    out = this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string 
BoolExpression::EvalString() const
{
    return this->value ? "true" : "false";
}

//------------------------------------------------------------------------------
/**
*/
bool 
BoolExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

} // namespace GPULang