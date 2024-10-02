//------------------------------------------------------------------------------
//  @file switchstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "switchstatement.h"
#include <array>
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
SwitchStatement::SwitchStatement(Expression* switchExpression, const std::vector<Expression*>& caseExpressions, const std::vector<Statement*>& caseStatements, Statement* defaultStatement)
    : switchExpression(switchExpression)
    , caseExpressions(caseExpressions)
    , caseStatements(caseStatements)
    , defaultStatement(defaultStatement)
{
    this->symbolType = SwitchStatementType;
}

//------------------------------------------------------------------------------
/**
*/
SwitchStatement::~SwitchStatement()
{
    delete this->switchExpression;
    for (Statement* stat : this->caseStatements)
        delete stat;
    if (this->defaultStatement != nullptr)
        delete this->defaultStatement;
}

} // namespace GPULang
