#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::BoolVecExpression
    
    A boolean expression holds a single boolean argument.
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
namespace GPULang
{
struct BoolVecExpression : public Expression
{
public:
	/// constructor
	BoolVecExpression(const std::vector<bool>& values);
	/// Default constructor
	BoolVecExpression();
	/// destructor
	virtual ~BoolVecExpression();

    /// Resolve
    bool Resolve(Compiler* compiler) override;

    /// eval type
    bool EvalType(Type::FullType& out) const override;
    /// eval symbol
    bool EvalSymbol(std::string& out) const override;
	/// Evaluate value
	bool EvalValue(ValueUnion& out) const override;
    /// evaluates expression as a string
    std::string EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;
	/// evaluates storage
	bool EvalStorage(Storage& out) const override;
	
	std::vector<bool> values;


    struct __Resolved : public Expression::__Resolved
    {
        Type::FullType fullType;
        Type* type;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------