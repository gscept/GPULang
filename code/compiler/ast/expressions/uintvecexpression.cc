//------------------------------------------------------------------------------
//  uintexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uintvecexpression.h"
#include "generated/types.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UIntVecExpression::UIntVecExpression(const FixedArray<unsigned int>& values) :
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
    static ConstantString Types[] =
    {
        "u32x2",
        "u32x3",
        "u32x4"
    };
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ Types[this->values.size-1]};
    thisResolved->fullType.literal = true;
    thisResolved->type = &UInt32Type;
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
UIntVecExpression::EvalTypeSymbol(Type*& out) const
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
    out.columnSize = this->values.size;
    out.rowSize = 1;
    out.code = TypeCode::UInt32;
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
