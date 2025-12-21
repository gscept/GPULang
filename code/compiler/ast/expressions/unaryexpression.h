#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::UnaryExpression
    
    A unary expression is a one-sided expression.

    Either

    < op expression >
    
    Or

    < expression op >
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct UnaryExpression : public Expression
{
public:
    /// constructor
    UnaryExpression(uint32_t op, bool isPrefix, Expression* expr);
    /// destructor
    virtual ~UnaryExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval type symbol
    bool EvalTypeSymbol(Type*& out) const override;
    /// eval unswizzle type symbol
    bool EvalUnswizzledTypeSymbol(Type*& out) const override;
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
    
    uint32_t op;
    bool isPrefix;
    Expression* expr;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType fullType;
        Symbol* dereffedSymbol = nullptr;
        Type* type = nullptr;
        Type::SwizzleMask swizzleMask;
        Type* swizzleType = nullptr;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------
