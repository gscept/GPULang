//------------------------------------------------------------------------------
//  intexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "intvecexpression.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
IntVecExpression::IntVecExpression(const std::vector<int>& values) :
    values(values)
{
    this->resolved = Alloc<IntVecExpression::__Resolved>();
    this->symbolType = IntVecExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
IntVecExpression::~IntVecExpression()
{
}

//------------------------------------------------------------------------------
/**
*/
bool 
IntVecExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ Format("i32x%d", this->values.size()) };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntVecExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntVecExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntVecExpression::EvalValue(ValueUnion& out) const
{
    int index = 0;
    for (const int val : this->values)
        out.i[index++] = val;
    out.columnSize = this->values.size();
    out.rowSize = 1;
    out.code = TypeCode::Int;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
IntVecExpression::EvalString() const
{
    TransientString ret;
    for (const bool val : this->values)
    {
        ret.Concatenate<false>(val, " ");
    }
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
bool 
IntVecExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
IntVecExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
