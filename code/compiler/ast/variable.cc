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
    this->resolved = Alloc<Variable::__Resolved>();
    this->type = Type::FullType{ "" };
    this->valueExpression = nullptr;

    Variable::__Resolved* typeResolved = static_cast<Variable::__Resolved*>(this->resolved);
    typeResolved->accessBits.bits = 0x0;
    typeResolved->parameterBits.bits = 0x0;
    typeResolved->usageBits.bits = 0x0;
    typeResolved->group = __Resolved::NOT_BOUND;
    typeResolved->binding = __Resolved::NOT_BOUND;
    typeResolved->inBinding = 0xF;
    typeResolved->outBinding = 0xF;
    typeResolved->byteSize = 0;
    typeResolved->structureOffset = 0;
    typeResolved->elementPadding = 0;
    typeResolved->startPadding = 0;
    typeResolved->visibilityBits.bits = 0x0;
    this->thisResolved = Symbol::Resolved(this);
}

//------------------------------------------------------------------------------
/**
*/
Variable::~Variable()
{
}



} // namespace GPULang
