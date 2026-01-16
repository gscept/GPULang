//------------------------------------------------------------------------------
//  uintexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uintexpression.h"
#include "util.h"
#include "compiler.h"
#include "generated/types.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UIntExpression::UIntExpression(int value) :
    value(value)
{
    this->resolved = Alloc<UIntExpression::__Resolved>();
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
    thisResolved->fullType = Type::FullType{ ConstantString("u32") };
    thisResolved->fullType.literal = true;
    thisResolved->type = &UInt32Type;
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
UIntExpression::EvalTypeSymbol(Type*& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->type;
    assert(out->symbolType == Symbol::SymbolType::TypeType || out->symbolType == Symbol::SymbolType::EnumerationType || out->symbolType == Symbol::SymbolType::StructureType);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntExpression::EvalValue(ValueUnion& out) const
{
    out.code = TypeCode::UInt32;
    out.ui[0] = this->value;
    out.columnSize = 1;
    out.rowSize = 1;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
UIntExpression::EvalString() const
{
    return TransientString(this->value);
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

//------------------------------------------------------------------------------
/**
*/
bool
UIntExpression::EvalStorage(Storage& out) const
{
    auto res = Symbol::Resolved(this);
    out = res->storage;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
UIntExpression::EvalDomain(Domain& out) const
{
    out = Domain::Device;
    return true;
}

} // namespace GPULang
