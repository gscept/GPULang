//------------------------------------------------------------------------------
//  boolvecexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "boolvecexpression.h"
#include "ast/types/builtins.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
BoolVecExpression::BoolVecExpression(const FixedArray<bool>& values) :
    values(values)
{
    this->resolved = Alloc<BoolVecExpression::__Resolved>();
    this->symbolType = BoolVecExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
BoolVecExpression::~BoolVecExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
BoolVecExpression::Resolve(Compiler* compiler)
{
    static ConstantString Types[] =
    {
        "b8x2",
        "b8x3",
        "b8x4"
    };
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ Types[this->values.size-1]};
    thisResolved->fullType.literal = true;
    thisResolved->type = &BoolType;
    //thisResolved->text = this->EvalString();
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolVecExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolVecExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolVecExpression::EvalValue(ValueUnion& out) const
{
    int index = 0;
    for (const bool val : this->values)
        out.b[index++] = val;
    out.columnSize = this->values.size;
    out.rowSize = 1;
    out.code = TypeCode::Bool;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
BoolVecExpression::EvalString() const
{
    TransientString ret;
    for (const bool val : this->values)
    {
        ret.Append(val ? "true " : "false ");
    }
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool 
BoolVecExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
BoolVecExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
