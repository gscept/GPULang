//------------------------------------------------------------------------------
//  floatexpression.cc
//  (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "floatvecexpression.h"
#include "compiler.h"
#include "util.h"
#include "ast/types/type.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
FloatVecExpression::FloatVecExpression(const std::vector<float>& values) 
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
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ Format("f32x%d", this->values.size()) };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
    thisResolved->text = this->EvalString();
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
FloatVecExpression::EvalSymbol(std::string& out) const
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
    out.columnSize = this->values.size();
    out.rowSize = 1;
    out.code = TypeCode::Float;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
FloatVecExpression::EvalString() const
{
    std::string ret;
    for (const bool val : this->values)
    {
        ret += Format("%f ", val);
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