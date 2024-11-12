//------------------------------------------------------------------------------
//  @file expressionstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "expressionstatement.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
ExpressionStatement::ExpressionStatement(Expression* expr)
    : expr(expr)
{
    this->symbolType = ExpressionStatementType;
    this->resolved = Alloc<ExpressionStatement::__Resolved>();
}

//------------------------------------------------------------------------------
/**
*/
ExpressionStatement::~ExpressionStatement()
{
}

} // namespace GPULang
