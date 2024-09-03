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
}

//------------------------------------------------------------------------------
/**
*/
ReturnStatement::~ReturnStatement()
{
    delete this->returnValue;
}

} // namespace GPULang
