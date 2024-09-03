#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::UnaryExpression
    
    A unary expression is a one-sided expression.

    Either

    < op expression >
    
    Or

    < expression op >
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace AnyFX
{
struct UnaryExpression : public Expression
{
public:
    /// constructor
    UnaryExpression(uint32_t op, uint32_t postOp, Expression* expr);
    /// destructor
    virtual ~UnaryExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval symbol
    bool EvalSymbol(std::string& out) const override;
    /// evaluates expression as an integer
    bool EvalInt(int& out) const override;
    /// evaluates expression as an integer
    bool EvalUInt(unsigned& out) const override;
    /// evaulates expression as a float
    bool EvalFloat(float& out) const override;
    /// evaluates expression as a boolean
    bool EvalBool(bool& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;

    uint32_t op;
    uint32_t postOp;
    Expression* expr;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType fullType;
        Type* type;
    };
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------