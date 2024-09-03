#pragma once
//------------------------------------------------------------------------------
/**
    Condition statement

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
namespace AnyFX
{

struct IfStatement : public Statement
{
    /// constructor
    IfStatement(Expression* condition, Statement* ifStatement, Statement* elseStatement);
    /// destructor
    virtual ~IfStatement();

    Expression* condition;
    Statement* ifStatement;
    Statement* elseStatement;
};

} // namespace AnyFX
