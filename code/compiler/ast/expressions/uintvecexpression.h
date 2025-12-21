#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::UIntVecExpression
    
    An unsigned expression is a simple expression which holds an unsigned integer.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct UIntVecExpression : public Expression
{
public:
	/// constructor
    UIntVecExpression(const FixedArray<unsigned int>& values);
	/// destructor
	virtual ~UIntVecExpression();

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
    
    FixedArray<unsigned int> values;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType fullType;
        Type* type = nullptr;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------
