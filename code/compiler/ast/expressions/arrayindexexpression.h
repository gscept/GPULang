#pragma once
//------------------------------------------------------------------------------
/**
    Array index expression
    < [ expression ] >

    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include <vector>

namespace GPULang
{

struct ArrayIndexExpression : public Expression
{
    /// constructor
    ArrayIndexExpression(Expression* left, Expression* right);
    /// destructor
    virtual ~ArrayIndexExpression();

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

    Expression* left;
    Expression* right;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType leftFullType;
        Type* lhsType = nullptr;
        Type::FullType rightFullType;
        Type* rhsType = nullptr;
        Type::FullType returnFullType;
        Type* returnType = nullptr;

        unsigned int literalAccess = 0xFFFFFFFF;

        bool isAddressIndex = false;
    };
};

} // namespace GPULang
