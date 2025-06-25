#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::FloatVecExpression
    
    A simple expression which holds a floating point value.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct FloatVecExpression : public Expression
{
public:
	/// constructor
	FloatVecExpression(const FixedArray<float>& values);
	/// destructor
	virtual ~FloatVecExpression();

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
	
    FixedArray<float> values;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType fullType;
        Type* type;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------
