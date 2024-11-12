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
    this->resolved = Alloc<SwitchStatement::__Resolved>();
}

//------------------------------------------------------------------------------
/**
*/
SwitchStatement::~SwitchStatement()
{
}

} // namespace GPULang
