#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::BinaryExpression
    
    A binary expression is a two-part expression, using a left-hand side and right-hand side argument together with an operator.

    < expression op expression >
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct BinaryExpression : public Expression
{
public:
	/// constructor
	BinaryExpression(uint32_t op, Expression* left, Expression* right);
	/// destructor
	virtual ~BinaryExpression();

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

	/// helper functions for compile time bool evaluations
	bool EvalBool(int lhs, int rhs) const;
	bool EvalBool(float lhs, int rhs) const;
	bool EvalBool(int lhs, float rhs) const;
	bool EvalBool(float lhs, float rhs) const;
	bool EvalBool(bool lhs, bool rhs) const;

	uint32_t op;
	Expression* left;
	Expression* right;

    struct __Resolved : public Expression::__Resolved
    {
        Function* conversionFunction;
        Type::FullType leftType;
        Type* lhsType;
        Type::FullType rightType;
        Type* rhsType;
        Type::FullType returnType;
        Type* retType;
    };
}; 


} // namespace GPULang
//------------------------------------------------------------------------------