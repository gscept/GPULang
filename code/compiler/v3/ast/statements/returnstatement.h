#pragma once
//------------------------------------------------------------------------------
/**
    Return statement

    (C) 2021 Individual contributors, see AUTHORS file
*/  
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
namespace AnyFX
{

struct ReturnStatement : public Statement
{
    /// constructor
    ReturnStatement(Expression* returnValue);
    /// destructor
    virtual ~ReturnStatement();

    Expression* returnValue;
};

} // namespace AnyFX
