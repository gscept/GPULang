//------------------------------------------------------------------------------
//  intexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "intexpression.h"
#include "generated/types.h"
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
    this->thisResolved = &this->intResolved;
    this->symbolType = IntExpressionType;
    this->resolved = this->thisResolved;
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
    this->thisResolved->fullType = Type::FullType{ ConstantString("i32") };
    this->thisResolved->fullType.literal = true;
    this->thisResolved->type = &Int32Type;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalType(Type::FullType& out) const
{
    out = this->thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalTypeSymbol(Type*& out) const
{
    out = this->thisResolved->type;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalValue(ValueUnion& out) const
{
    out.code = TypeCode::Int32;
    out.i[0] = this->value;
    out.columnSize = 1;
    out.rowSize = 1;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
IntExpression::EvalString() const
{
    return TransientString(this->value);
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
    auto res = Symbol::Resolved(this);
    out = res->storage;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
IntExpression::EvalDomain(Domain& out) const
{
    out = Domain::Device; // Static value, uniform across all threads and workgroups
    return true;
}

} // namespace GPULang
