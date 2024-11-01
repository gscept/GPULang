#pragma once
//------------------------------------------------------------------------------
/**
    Initializer expression

    < { expression0 .. expressionN } >

    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include <vector>

namespace GPULang
{

struct ArrayInitializerExpression : public Expression
{
    /// constructor
    ArrayInitializerExpression(const std::vector<Expression*>& values);
    /// Destructor
    virtual ~ArrayInitializerExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;

    std::vector<Expression*> values;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType fullType;
        Type* type;
    };
};

} // namespace GPULang
