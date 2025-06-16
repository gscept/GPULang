//------------------------------------------------------------------------------
//  uintexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uintvecexpression.h"

#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UIntVecExpression::UIntVecExpression(const std::vector<unsigned int>& values) :
    values(values)
{
    this->resolved = Alloc<UIntVecExpression::__Resolved>();
    this->symbolType = UIntVecExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
UIntVecExpression::~UIntVecExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
UIntVecExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ Format("u32x%d", this->values.size()) };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntVecExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntVecExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntVecExpression::EvalValue(ValueUnion& out) const
{
    int index = 0;
    for (const unsigned int val : this->values)
        out.ui[index++] = val;
    out.columnSize = this->values.size();
    out.rowSize = 1;
    out.code = TypeCode::UInt;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
UIntVecExpression::EvalString() const
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
UIntVecExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
UIntVecExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
