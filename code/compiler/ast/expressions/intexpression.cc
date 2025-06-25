//------------------------------------------------------------------------------
//  intexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "intexpression.h"
#include "ast/types/builtins.h"
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
    this->resolved = Alloc<IntExpression::__Resolved>();
    this->symbolType = IntExpressionType;
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
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ ConstantString("i32") };
    thisResolved->fullType.literal = true;
    thisResolved->type = &IntType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
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
    out.code = TypeCode::Int;
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
    out = Storage::Default;
    return true;
}

} // namespace GPULang
