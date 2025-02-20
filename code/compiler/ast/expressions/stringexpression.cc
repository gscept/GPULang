//------------------------------------------------------------------------------
//  stringexpression.cc
//  (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "stringexpression.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
StringExpression::StringExpression(std::string value) :
    value(value)
{
    this->symbolType = StringExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
StringExpression::~StringExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
StringExpression::EvalInt(int& out) const
{
    out = std::stoi(this->value);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
StringExpression::EvalUInt(unsigned& out) const
{
    out = std::stoul(this->value);
    return true;
}

//------------------------------------------------------------------------------
/**
    Convert this integer to a float
*/
bool
StringExpression::EvalFloat(float& out) const
{
    out = std::stof(this->value);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
StringExpression::EvalBool(bool& out) const
{
    out = this->value == "true";
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
StringExpression::EvalString() const
{
    return this->value;
}

//------------------------------------------------------------------------------
/**
*/
bool 
StringExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
StringExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang