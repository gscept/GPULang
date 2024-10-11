//------------------------------------------------------------------------------
//  @file continuestatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "continuestatement.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
ContinueStatement::ContinueStatement()
{
    this->symbolType = ContinueStatementType;
    this->resolved = new ContinueStatement::__Resolved;
}

} // namespace GPULang
