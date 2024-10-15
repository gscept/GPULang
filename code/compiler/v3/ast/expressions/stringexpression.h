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

	std::string value;
}; 
} // namespace GPULang
//------------------------------------------------------------------------------