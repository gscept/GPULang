//------------------------------------------------------------------------------
//  enumexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "enumexpression.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
EnumExpression::EnumExpression(int value, Type::FullType type)
    : value(value)
    , type(type)
{
    this->resolved = new EnumExpression::__Resolved;
    this->symbolType = EnumExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
EnumExpression::~EnumExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
EnumExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = this->type;
    thisResolved->type = compiler->GetSymbol<Type>(thisResolved->fullType.name);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalSymbol(std::string& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalInt(int& out) const
{
    out = (int)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalUInt(unsigned& out) const
{
    out = this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
    Convert this integer to a float
*/
bool
EnumExpression::EvalFloat(float& out) const
{
    out = (float)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalBool(bool& out) const
{
    out = this->value != 0;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
EnumExpression::EvalString() const
{
    return Format("%d", this->value);
}

} // namespace GPULang