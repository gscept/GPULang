//------------------------------------------------------------------------------
//  boolvecexpression.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "boolvecexpression.h"
#include "util.h"
#include "compiler.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
BoolVecExpression::BoolVecExpression(const std::vector<bool>& values) :
    values(values)
{
    this->resolved = new BoolVecExpression::__Resolved;
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
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->fullType = Type::FullType{ Format("b8x%d", this->values.size()) };
    thisResolved->fullType.literal = true;
    thisResolved->type = compiler->GetType(thisResolved->fullType);
    thisResolved->text = this->EvalString();
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
BoolVecExpression::EvalSymbol(std::string& out) const
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
    out.columnSize = this->values.size();
    out.rowSize = 1;
    out.code = TypeCode::Bool;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
std::string 
BoolVecExpression::EvalString() const
{
    std::string ret;
    for (const bool val : this->values)
    {
        ret += val ? "true " : "false ";
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