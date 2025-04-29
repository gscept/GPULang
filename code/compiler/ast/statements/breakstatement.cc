//------------------------------------------------------------------------------
//  @file breakstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "breakstatement.h"
namespace GPULang 
{

//------------------------------------------------------------------------------
/**
*/
BreakStatement::BreakStatement()
{
    this->symbolType = BreakStatementType;
    this->resolved = Alloc<BreakStatement::__Resolved>();
}

} // namespace GPULang
