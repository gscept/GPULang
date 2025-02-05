//------------------------------------------------------------------------------
//  @file terminatestatement.cc
//  @copyright (C) 2025 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "terminatestatement.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
TerminateStatement::TerminateStatement(Expression* returnValue, TerminationType type)
    : returnValue(returnValue)
{
    this->symbolType = TerminateStatementType;
    this->type = type;
    this->resolved = Alloc<TerminateStatement::__Resolved>();
}

//------------------------------------------------------------------------------
/**
*/
TerminateStatement::~TerminateStatement()
{
}

const std::vector<std::string> terminationTypeLookup =
{
    "return",
    "discard",
    "ray_ignore",
    "ray_terminate"
};
//------------------------------------------------------------------------------
/**
*/
std::string
TerminateStatement::TerminationTypeToString(TerminationType type)
{
    return terminationTypeLookup[(unsigned)type];
}
} // namespace GPULang
