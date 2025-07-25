//------------------------------------------------------------------------------
//  floatexpression.cc
//  (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "floatvecexpression.h"
#include "generated/types.h"
#include "compiler.h"
#include "util.h"
#include "ast/types/type.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
FloatVecExpression::FloatVecExpression(const FixedArray<float>& values) 
    : values(values)
{
    this->resolved = Alloc<FloatVecExpression::__Resolved>();
    this->symbolType = FloatVecExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
FloatVecExpression::~FloatVecExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
FloatVecExpression::Resolve(Compiler* compiler)
{
    static ConstantString Types[] =
    {
        "f32x2",
        "f32x3",
        "f32x4"
    };
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ Types[this->values.size-1]};
    thisResolved->fullType.literal = true;
    thisResolved->type = &Float32Type;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
FloatVecExpression::EvalType(Type::FullType& out) const
{
    auto thisResolved = Symbol::Resolved(this);
    out = thisResolved->fullType;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
FloatVecExpression::EvalTypeSymbol(Type*& out) const
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
FloatVecExpression::EvalSymbol(FixedString& out) const
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
FloatVecExpression::EvalValue(ValueUnion& out) const
{
    int index = 0;
    for (const float val : this->values)
        out.f[index++] = val;
    out.columnSize = this->values.size;
    out.rowSize = 1;
    out.code = TypeCode::Float32;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
FloatVecExpression::EvalString() const
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
FloatVecExpression::EvalAccessFlags(unsigned& out) const
{
    out = AccessFlags::Const;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
FloatVecExpression::EvalStorage(Storage& out) const
{
    out = Storage::Default;
    return true;
}

} // namespace GPULang
