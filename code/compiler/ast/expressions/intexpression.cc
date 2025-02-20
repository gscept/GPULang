//------------------------------------------------------------------------------
//  intexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "intexpression.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
IntExpression::IntExpression(int value) :
    value(value)
{
    this->resolved = new IntExpression::__Resolved;
    this->symbolType = IntExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
IntExpression::~IntExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
IntExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ "i32" };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
    thisResolved->text = this->EvalString();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalSymbol(std::string& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalInt(int& out) const
{
    out = this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalUInt(unsigned& out) const
{
    out = (unsigned)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
    Convert this integer to a float
*/
bool
IntExpression::EvalFloat(float& out) const
{
    out = (float)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalBool(bool& out) const
{
    out = this->value != 0;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
IntExpression::EvalString() const
{
    return Format("%d", this->value);
}

//------------------------------------------------------------------------------
/**
*/
bool 
IntExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang