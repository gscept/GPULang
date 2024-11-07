//------------------------------------------------------------------------------
//  uintexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uintexpression.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UIntExpression::UIntExpression(int value) :
    value(value)
{
    this->resolved = new UIntExpression::__Resolved;
    this->symbolType = UIntExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
UIntExpression::~UIntExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
UIntExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ "u32" };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetSymbol<Type>(thisResolved->fullType.name);
    thisResolved->text = this->EvalString();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntExpression::EvalSymbol(std::string& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntExpression::EvalInt(int& out) const
{
    out = (int)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntExpression::EvalUInt(unsigned& out) const
{
    out = this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
    Convert this integer to a float
*/
bool
UIntExpression::EvalFloat(float& out) const
{
    out = (float)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntExpression::EvalBool(bool& out) const
{
    out = this->value != 0;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
UIntExpression::EvalString() const
{
    return Format("%d", this->value);
}

//------------------------------------------------------------------------------
/**
*/
bool 
UIntExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

} // namespace GPULang