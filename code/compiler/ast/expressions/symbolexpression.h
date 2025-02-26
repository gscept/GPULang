#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::SymbolExpression
    
    Evaluates, if possible, a symbol as a single value, currently this is
	only possible for constants.
    
    (C) 2020 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"

namespace GPULang
{
struct SymbolExpression : public Expression
{
public:
	/// constructor
	SymbolExpression(const std::string symbol);
	/// destructor
	virtual ~SymbolExpression();

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

	std::string symbol;

    struct __Resolved : public Expression::__Resolved
    {
        Symbol* symbol;
        Type::FullType fullType;
        Type* type;
    };
	__Resolved* thisResolved;
}; 
} // namespace GPULang
//------------------------------------------------------------------------------