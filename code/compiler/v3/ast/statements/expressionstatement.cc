//------------------------------------------------------------------------------
//  @file expressionstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "expressionstatement.h"
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
ExpressionStatement::ExpressionStatement(Expression* expr)
    : expr(expr)
{
    this->symbolType = ExpressionStatementType;
}

//------------------------------------------------------------------------------
/**
*/
ExpressionStatement::~ExpressionStatement()
{
    delete this->expr;
}

} // namespace AnyFX
