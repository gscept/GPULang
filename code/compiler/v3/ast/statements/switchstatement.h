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
namespace AnyFX
{

struct SwitchStatement : public Statement
{
    /// constructor
    SwitchStatement(Expression* switchExpression, const std::vector<std::string>& caseValues, const std::vector<Statement*>& caseStatements, Statement* defaultStatement);
    /// destructor
    virtual ~SwitchStatement();

    Expression* switchExpression;
    std::vector<std::string> caseValues;
    std::vector<Statement*> caseStatements;
    Statement* defaultStatement;
};

} // namespace AnyFX
