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
ExpressionStatement::ExpressionStatement(FixedArray<Expression*> expressions)
    : expressions(expressions)
{
    this->symbolType = ExpressionStatementType;
    this->resolved = Alloc<ExpressionStatement::__Resolved>();
}

//------------------------------------------------------------------------------
/**
*/
ExpressionStatement::~ExpressionStatement()
{
    for (auto& expr : this->expressions)
        expr->~Expression();    
}

} // namespace GPULang
