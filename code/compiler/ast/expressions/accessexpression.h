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

    bool deref;
    Expression* left;
    Expression* right;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType leftType;
        Type* lhsType;
        Type::FullType rightType;
        Type* rhsType;
        FixedString rightSymbol;
        Type::FullType returnType;
        Type* retType;
        Type::SwizzleMask swizzleMask;
        Type* swizzleType = nullptr;
    };
};

} // namespace GPULang
