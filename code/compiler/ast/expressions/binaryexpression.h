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
    	Function* leftConversion = nullptr;
    	Function* rightConversion = nullptr;
        Type::FullType leftType;
        Type* lhsType = nullptr;
        Type::FullType rightType;
        Type* rhsType = nullptr;
        Type::FullType returnType;
        Type* retType = nullptr;

        virtual ~__Resolved()
        {
            if (this->constValueExpression != nullptr)
                this->constValueExpression->~Expression();
        }

    	Expression* constValueExpression = nullptr;

    	bool isAssignment = false;
        bool isAddressOperation = false;
    };
	__Resolved* thisResolved;
}; 


} // namespace GPULang
//------------------------------------------------------------------------------
