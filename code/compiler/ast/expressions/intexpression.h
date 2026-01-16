#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::IntExpression
    
    An int expression is a simple expression which holds an integer.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct IntExpression : public Expression
{
public:
    /// constructor
    IntExpression(int value);
    /// destructor
    virtual ~IntExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval type symbol
    bool EvalTypeSymbol(Type*& out) const override;
    /// eval symbol
    bool EvalSymbol(FixedString& out) const override;
    /// Evaluate value
    bool EvalValue(ValueUnion& out) const override;
    /// evaluates expression as a string
    TransientString EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;
    /// evaluates storage
    bool EvalStorage(Storage& out) const override;
    /// Evaluates domain
    bool EvalDomain(Domain& out) const override;

    int value;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType fullType;
        Type* type = nullptr;
    } intResolved;
    __Resolved* thisResolved;
};
} // namespace GPULang
//------------------------------------------------------------------------------
