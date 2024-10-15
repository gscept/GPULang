#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::BoolExpression
    
    A boolean expression holds a single boolean argument.
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct BoolExpression : public Expression
{
public:
	/// constructor
	BoolExpression(bool value);
	/// destructor
	virtual ~BoolExpression();

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

	bool value;

    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType fullType;
        Type* type;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------