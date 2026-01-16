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
	/// Default constructor
	BoolExpression();
	/// destructor
	virtual ~BoolExpression();

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
    /// Evalutes domain
    bool EvalDomain(Domain& out) const override;
	
	bool value;

    struct __Resolved : public Expression::__Resolved
    {
        virtual ~__Resolved() {};
        Type::FullType fullType;
        Type* type = nullptr;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------
