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
EnumExpression::EnumExpression(int value, Type::FullType type, Type::FullType underlyingType)
    : value(value)
    , type(type)
    , underlyingType(underlyingType)
{
    this->resolved = Alloc<EnumExpression::__Resolved>();
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
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
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
EnumExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalValue(ValueUnion& out) const
{
    out.code = TypeCode::Int;
    out.columnSize = 1;
    out.i[0] = this->value;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
EnumExpression::EvalString() const
{
    return TransientString(this->value);
}

//------------------------------------------------------------------------------
/**
*/
bool 
EnumExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalStorage(Storage& out) const
{
    return Expression::EvalStorage(out);
}

} // namespace GPULang
