//------------------------------------------------------------------------------
//  boolexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "boolexpression.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
BoolExpression::BoolExpression(bool value) :
    value(value)
{
    this->resolved = Alloc<BoolExpression::__Resolved>();
    this->symbolType = BoolExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
BoolExpression::BoolExpression() :
    value(false)
{
    this->resolved = Alloc<BoolExpression::__Resolved>();
    this->symbolType = BoolExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
BoolExpression::~BoolExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
BoolExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ ConstantString("b8") };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalValue(ValueUnion& out) const
{
    out.code = TypeCode::Bool;
    out.b[0] = this->value;
    out.columnSize = 1;
    out.rowSize = 1;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
BoolExpression::EvalString() const
{
    return this->value ? "true" : "false";
}

//------------------------------------------------------------------------------
/**
*/
bool 
BoolExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
