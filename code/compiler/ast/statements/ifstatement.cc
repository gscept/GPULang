//------------------------------------------------------------------------------
//  @file ifstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "ifstatement.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
IfStatement::IfStatement(Expression* condition, Statement* ifStatement, Statement* elseStatement)
    : condition(condition)
    , ifStatement(ifStatement)
    , elseStatement(elseStatement)
{
    this->symbolType = IfStatementType;
    this->resolved = Alloc<IfStatement::__Resolved>();
}

//------------------------------------------------------------------------------
/**
*/
IfStatement::~IfStatement()
{
    this->condition->~Expression();
    this->ifStatement->~Statement();
    if (this->elseStatement != nullptr)
        this->elseStatement->~Statement();
}

} // namespace GPULang
