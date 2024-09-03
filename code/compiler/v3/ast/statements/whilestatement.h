#pragma once
//------------------------------------------------------------------------------
/**
    While loop

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
namespace AnyFX
{

struct WhileStatement : public Statement
{
    /// constructor
    WhileStatement(Expression* condition, Statement* statement, bool isDoWhile);
    /// destructor
    virtual ~WhileStatement();

    Expression* condition;
    Statement* statement;
    bool isDoWhile;
};

} // namespace AnyFX
