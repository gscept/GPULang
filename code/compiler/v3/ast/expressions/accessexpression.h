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
    /// eval uint
    bool EvalUInt(unsigned& out) const override;
    /// eval int
    bool EvalInt(int& out) const override;
    /// eval float
    bool EvalFloat(float& out) const override;
    /// eval bool
    bool EvalBool(bool& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;

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
