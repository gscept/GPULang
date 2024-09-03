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
}

//------------------------------------------------------------------------------
/**
*/
IfStatement::~IfStatement()
{
    delete this->condition;
    delete this->ifStatement;
    delete this->elseStatement;
}

} // namespace GPULang
