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
    this->resolved = new IntVecExpression::__Resolved;
    this->symbolType = IntVecExpressionType;
}

//------------------------------------------------------------------------------
/**
*/
IntVecExpression::~IntVecExpression()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool 
IntVecExpression::Resolve(Compiler* compiler)
{
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ Format("i32x%s", this->values.size()) };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
    thisResolved->text = this->EvalString();
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
IntVecExpression::EvalSymbol(std::string& out) const
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
    out.valid = true;
    out.code = TypeCode::Int;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string
IntVecExpression::EvalString() const
{
    std::string ret;
    for (const bool val : this->values)
    {
        ret += Format("%d ", val);
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