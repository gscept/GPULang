#pragma once
//------------------------------------------------------------------------------
/**
    Initializer expression

    < { expression0 .. expressionN } >

    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include "ast/variable.h"
#include <vector>

namespace GPULang
{

struct InitializerExpression : public Expression
{
    /// constructor
    InitializerExpression(const std::vector<Expression*>& values, const std::string& type);
    /// Destructor
    virtual ~InitializerExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;

    std::vector<Expression*> values;
    std::string explicitType;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType fullType;
        Type* type;
    };
};

} // namespace GPULang