#pragma once
//------------------------------------------------------------------------------
/**
    Variable declaration

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
#include <vector>
namespace AnyFX
{

struct ExpressionStatement : public Statement
{
    /// constructor
    ExpressionStatement(Expression* expr);
    /// destructor
    virtual ~ExpressionStatement();

    Expression* expr;
};

} // namespace AnyFX
