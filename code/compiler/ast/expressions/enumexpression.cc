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
EnumExpression::EnumExpression()
    : value(0)
    , type(Type::FullType{ UNDEFINED_TYPE })
    , underlyingType(Type::FullType{ UNDEFINED_TYPE })
{
    this->resolved = Alloc<EnumExpression::__Resolved>();
    this->symbolType = EnumExpressionType;
}

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
    thisResolved->type = compiler->GetType(this->type);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalType(Type::FullType& out) const
{

    if (this->type.name == UNDEFINED_TYPE)
        return false;
    out = this->type;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
EnumExpression::EvalTypeSymbol(Type*& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    if (thisResolved->type == nullptr)
        return false;
    out = thisResolved->type;
    assert(out->symbolType == Symbol::SymbolType::TypeType || out->symbolType == Symbol::SymbolType::EnumerationType || out->symbolType == Symbol::SymbolType::StructureType);
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
