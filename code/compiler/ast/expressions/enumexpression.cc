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
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
    thisResolved->text = this->EvalString();
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
std::string
EnumExpression::EvalString() const
{
    return Format("%d", this->value);
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