#pragma once
//------------------------------------------------------------------------------
/**
    Switch

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
#include <vector>
#include <string>
namespace GPULang
{

struct SwitchStatement : public Statement
{
    /// constructor
    SwitchStatement(Expression* switchExpression, const std::vector<Expression*>& caseExpressions, const std::vector<Statement*>& caseStatements, Statement* defaultStatement);
    /// destructor
    virtual ~SwitchStatement();

    struct __Resolved : public Statement::__Resolved
    {
        virtual ~__Resolved() {};
        Scope scope;
    };

    Expression* switchExpression;
    std::vector<Expression*> caseExpressions;
    std::vector<Statement*> caseStatements;
    Statement* defaultStatement;
};

} // namespace GPULang
