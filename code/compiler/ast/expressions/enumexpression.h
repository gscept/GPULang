#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::EnumExpression
    
    Expression used to resolve enums
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct EnumExpression : public Expression
{
public:
	/// constructor
    EnumExpression(int value, Type::FullType type,  Type::FullType underlyingType);
	/// destructor
	virtual ~EnumExpression();

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
    
	unsigned int value;
    Type::FullType type;
    Type::FullType underlyingType;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType fullType;
        Type* type;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------
