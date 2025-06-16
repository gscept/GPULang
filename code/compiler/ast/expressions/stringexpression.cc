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
    this->resolved = nullptr;
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
TransientString
StringExpression::EvalString() const
{
    return TransientString(this->value);
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
