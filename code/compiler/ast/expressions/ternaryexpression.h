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
namespace GPULang
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
    TransientString EvalString() const override;
    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval type symbol
    bool EvalTypeSymbol(Type*& out) const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;
    /// evaluates storage
    bool EvalStorage(Storage& out) const override;
    
    Expression* lhs;
    Expression* ifExpression;
    Expression* elseExpression;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType fullType;
        Type* type;
    };
};

} // namespace GPULang
