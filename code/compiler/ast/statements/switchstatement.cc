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
SwitchStatement::SwitchStatement(Expression* switchExpression, const TransientArray<Expression*>& caseExpressions, const TransientArray<Statement*>& caseStatements, Statement* defaultStatement)
    : switchExpression(switchExpression)
    , caseExpressions(caseExpressions)
    , caseStatements(caseStatements)
    , defaultStatement(defaultStatement)
{
    this->symbolType = SwitchStatementType;
    this->resolved = Alloc<SwitchStatement::__Resolved>();
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->scope.type = Scope::ScopeType::Local;
}

//------------------------------------------------------------------------------
/**
*/
SwitchStatement::~SwitchStatement()
{
    this->switchExpression->~Expression();
    for (size_t i = 0; i < this->caseExpressions.size; i++)
    {
        this->caseExpressions[i]->~Expression();
        this->caseStatements[i]->~Statement();
    }
    if (this->defaultStatement != nullptr)
        this->defaultStatement->~Statement();
}

} // namespace GPULang
