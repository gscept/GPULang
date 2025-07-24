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
    if (this->returnValue != nullptr)
        this->returnValue->~Expression();
}

constexpr StaticArray terminationTypeLookup =
std::array {
    "return"_c,
    "discard"_c,
    "ray_ignore"_c,
    "ray_terminate"_c
};
//------------------------------------------------------------------------------
/**
*/
const ConstantString&
TerminateStatement::TerminationTypeToString(TerminationType type)
{
    return terminationTypeLookup[(unsigned)type];
}
} // namespace GPULang
