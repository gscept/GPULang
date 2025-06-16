#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::StringExpression
    
    An int expression is a simple expression which holds a string
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include <string>
namespace GPULang
{
struct StringExpression : public Expression
{
public:
	/// constructor
    StringExpression(std::string value);
	/// destructor
	virtual ~StringExpression();
	
    /// evaluates expression as a string
    TransientString EvalString() const override;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const override;
	/// evaluates storage
	bool EvalStorage(Storage& out) const override;
	
	FixedString value;
}; 
} // namespace GPULang
//------------------------------------------------------------------------------
