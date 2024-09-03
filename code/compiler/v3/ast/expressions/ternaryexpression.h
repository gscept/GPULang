#pragma once
//------------------------------------------------------------------------------
/**
    Ternary expression

    < ? expression : expression >

    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include "ast/expressions/expression.h"
namespace AnyFX
{

struct TernaryExpression : public Expression
{
    /// constructor
    TernaryExpression(Expression* lhs, Expression* ifExpression, Expression* elseExpression);
    /// destructor
    virtual ~TernaryExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// evaluates expression as a string
    std::string EvalString() const override;

    Expression* lhs;
    Expression* ifExpression;
    Expression* elseExpression;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType fullType;
        Type* type;
    };
};

} // namespace AnyFX
