//------------------------------------------------------------------------------
//  @file discardstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "discardstatement.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
DiscardStatement::DiscardStatement()
{
    this->symbolType = DiscardStatementType;
    this->resolved = new DiscardStatement::__Resolved;
}

} // namespace GPULang
