#pragma once
//------------------------------------------------------------------------------
/**
    Member access expression

    < . expression >

    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include <vector>

namespace GPULang
{

struct AccessExpression : public Expression
{
    /// constructor
    AccessExpression(Expression* left, Expression* right, bool deref);
    /// destructor
    virtual ~AccessExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval symbol
    bool EvalSymbol(std::string& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;

    bool deref;
    Expression* left;
    Expression* right;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType leftType;
        Type* lhsType;
        Type::FullType rightType;
        Type* rhsType;
        std::string rightSymbol;
        Type::FullType returnType;
        Type::SwizzleMask swizzleMask;
    };
};

} // namespace GPULang
