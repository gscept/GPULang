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
    this->resolved = Alloc<ReturnStatement::__Resolved>();
}

//------------------------------------------------------------------------------
/**
*/
ReturnStatement::~ReturnStatement()
{
}

} // namespace GPULang
