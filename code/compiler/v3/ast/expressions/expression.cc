//------------------------------------------------------------------------------
//  expression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "expression.h"
#include "util.h"
#include "compiler.h"
#include "v3/ast/types/type.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Expression::Expression() 
    : isLhsValue(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Expression::~Expression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
Expression::Resolve(Compiler* compiler)
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalType(Type::FullType& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalSymbol(std::string& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalInt(int& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalUInt(unsigned& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalFloat(float& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
Expression::EvalBool(bool& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Expression::EvalAccessFlags(unsigned& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
std::string
Expression::EvalString() const
{
    return "";
}

} // namespace GPULang