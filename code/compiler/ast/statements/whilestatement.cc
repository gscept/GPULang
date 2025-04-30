//------------------------------------------------------------------------------
//  @file whilestatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "whilestatement.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
WhileStatement::WhileStatement(Expression* condition, Statement* statement, bool isDoWhile)
    : condition(condition)
    , statement(statement)
    , isDoWhile(isDoWhile)
{
    this->symbolType = WhileStatementType;
    this->resolved = Alloc<WhileStatement::__Resolved>();
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->scope.type = Scope::ScopeType::Local;
}

//------------------------------------------------------------------------------
/**
*/
WhileStatement::~WhileStatement()
{
    this->condition->~Expression();
    this->statement->~Statement();
}

} // namespace GPULang
