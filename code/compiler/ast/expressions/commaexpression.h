#pragma once
//------------------------------------------------------------------------------
/**
    Comma expression

    < expression , expression >

    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include <vector>

namespace GPULang
{

struct CommaExpression : public Expression
{
    /// constructor
    CommaExpression(Expression* left, Expression* right);
    /// destructor
    virtual ~CommaExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval symbol
    bool EvalSymbol(FixedString& out) const override;
    /// evaluates expression as a string
    TransientString EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;
    /// evaluates storage
    bool EvalStorage(Storage& out) const override;
    
    Expression* left;
    Expression* right;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType rightType;
        Type* rhsType;
        Type::FullType leftType;
        Type* lhsType;
    };
};

} // namespace GPULang
