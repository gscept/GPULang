#pragma once
//------------------------------------------------------------------------------
/**
    For loop

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
#include <vector>
namespace GPULang
{

struct ForStatement : public Statement
{
    /// constructor
    ForStatement(const FixedArray<Variable*> declarations, Expression* condition, Expression* loop, Statement* contents);
    /// destructor
    virtual ~ForStatement();

    struct __Resolved : public Statement::__Resolved
    {
        virtual ~__Resolved() {};
        Scope scope;
    };

    FixedArray<Variable*> declarations;
    Expression* condition;
    Expression* loop;
    Statement* contents;

    unsigned unrollCount = 0;
};

} // namespace GPULang
