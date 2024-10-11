//------------------------------------------------------------------------------
//  @file returnstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "returnstatement.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
ReturnStatement::ReturnStatement(Expression* returnValue)
    : returnValue(returnValue)
{
    this->symbolType = ReturnStatementType;
    this->resolved = new ReturnStatement::__Resolved;
}

//------------------------------------------------------------------------------
/**
*/
ReturnStatement::~ReturnStatement()
{
    delete this->returnValue;
}

} // namespace GPULang
