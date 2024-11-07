#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::FloatExpression
    
    A simple expression which holds a floating point value.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct FloatExpression : public Expression
{
public:
	/// constructor
	FloatExpression(float value);
	/// destructor
	virtual ~FloatExpression();

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

	float value;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType fullType;
        Type* type;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------