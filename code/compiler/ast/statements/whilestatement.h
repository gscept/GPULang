#pragma once
//------------------------------------------------------------------------------
/**
    While loop

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
namespace GPULang
{

struct WhileStatement : public Statement
{
    /// constructor
    WhileStatement(Expression* condition, Statement* statement, bool isDoWhile);
    /// destructor
    virtual ~WhileStatement();

    struct __Resolved : public Statement::__Resolved
    {
        virtual ~__Resolved() {};
        Scope scope;
    };

    Expression* condition;
    Statement* statement;
    bool isDoWhile;
};

} // namespace GPULang
