//------------------------------------------------------------------------------
//  @file variable.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "variable.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Variable::Variable()
{
    this->symbolType = VariableType;
    this->resolved = &this->variableResolved;
    this->type = UndefinedType;
    this->valueExpression = nullptr;

    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(this->resolved);
    varResolved->typeSymbol = nullptr;
    varResolved->accessBits.bits = 0x0;
    varResolved->accessBits.flags.readAccess = true; // Implicitly set read access to true
    varResolved->parameterBits.bits = 0x0;
    varResolved->usageBits.bits = 0x0;
    varResolved->group = __Resolved::NOT_BOUND;
    varResolved->binding = __Resolved::NOT_BOUND;
    varResolved->inBinding = 0xF;
    varResolved->outBinding = 0xF;
    varResolved->byteSize = 0;
    varResolved->structureOffset = 0;
    varResolved->elementPadding = 0;
    varResolved->startPadding = 0;
    varResolved->visibilityBits.bits = 0x0;
    varResolved->storage = Storage::Default;
    this->thisResolved = Symbol::Resolved(this);
}

//------------------------------------------------------------------------------
/**
*/
Variable::~Variable()
{

}

//------------------------------------------------------------------------------
/**
*/
void
Variable::SetupAsBuiltinParameter()
{
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(this->resolved);
    varResolved->accessBits.flags.readAccess = true; // Implicitly set read access to true
    varResolved->byteSize = varResolved->typeSymbol->byteSize;
    varResolved->storage = Storage::Default;
    varResolved->usageBits.flags.isParameter = true;
}

} // namespace GPULang
