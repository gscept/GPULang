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
    bool EvalSymbol(std::string& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;

    Expression* left;
    Expression* right;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType rightType;
        Type* rhsType;
        Type::FullType leftType;
        Type* lhsType;
    };
};

} // namespace GPULang
