//------------------------------------------------------------------------------
//  floatexpression.cc
//  (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "floatexpression.h"
#include "ast/types/builtins.h"
#include "compiler.h"
#include "util.h"
#include "ast/types/type.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
FloatExpression::FloatExpression(float value) 
    : value(value)
{
    this->resolved = Alloc<FloatExpression::__Resolved>();
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
    thisResolved->fullType = Type::FullType{ ConstantString("f32") };
    thisResolved->fullType.literal = true;
    thisResolved->type = &FloatType;
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
FloatExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
FloatExpression::EvalValue(ValueUnion& out) const
{
    out.code = TypeCode::Float;
    out.f[0] = this->value;
    out.columnSize = 1;
    out.rowSize = 1;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
FloatExpression::EvalString() const
{
    return TransientString(this->value);
}

//------------------------------------------------------------------------------
/**
*/
bool 
FloatExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
FloatExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
