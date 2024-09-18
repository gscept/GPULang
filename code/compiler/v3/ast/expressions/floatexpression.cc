//------------------------------------------------------------------------------
//  floatexpression.cc
//  (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "floatexpression.h"
#include "compiler.h"
#include "util.h"
#include "v3/ast/types/type.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
FloatExpression::FloatExpression(float value) 
    : value(value)
{
    this->resolved = new FloatExpression::__Resolved;
    this->symbolType = FloatExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
FloatExpression::~FloatExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
FloatExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ "f32" };
    thisResolved->type = compiler->GetSymbol<Type>(thisResolved->fullType.name);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
FloatExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
FloatExpression::EvalSymbol(std::string& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
    Convert this float to an integer
*/
bool  
FloatExpression::EvalInt(int& out) const
{
    out = (int)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
FloatExpression::EvalUInt(unsigned& out) const
{
    out = (unsigned)this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
FloatExpression::EvalFloat(float& out) const
{
    out = this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
FloatExpression::EvalBool(bool& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
std::string
FloatExpression::EvalString() const
{
    return Format("%f", this->value);
}
} // namespace GPULang