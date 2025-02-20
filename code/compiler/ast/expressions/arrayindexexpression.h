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
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;
    /// evaluates storage
    bool EvalStorage(Storage& out) const override;

    Expression* left;
    Expression* right;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType leftFullType;
        Type* lhsType;
        Type::FullType rightFullType;
        Type* rhsType;
        Type::FullType returnFullType;
        Type* returnType;

        unsigned int literalAccess = 0xFFFFFFFF;
    };
};

} // namespace GPULang
