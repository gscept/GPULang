#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::IntVecExpression
    
    An int expression is a simple expression which holds an integer.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct IntVecExpression : public Expression
{
public:
    /// constructor
    IntVecExpression(const std::vector<int>& values);
    /// destructor
    virtual ~IntVecExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval symbol
    bool EvalSymbol(std::string& out) const override;
    /// Evaluate value
    bool EvalValue(ValueUnion& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;
    /// evaluates storage
    bool EvalStorage(Storage& out) const override;
    
    std::vector<int> values;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType fullType;
        Type* type;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------