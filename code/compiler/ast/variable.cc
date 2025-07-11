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
    if (SYMBOL_STATIC_ALLOC)
        this->resolved = StaticAlloc<Variable::__Resolved>();
    else
        this->resolved = Alloc<Variable::__Resolved>();
    this->type = Type::FullType{ ConstantString("") };
    this->valueExpression = nullptr;

    Variable::__Resolved* typeResolved = static_cast<Variable::__Resolved*>(this->resolved);
    typeResolved->typeSymbol = nullptr;
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
    this->CleanupAnnotations();
    this->CleanupAttributes();
    if (this->valueExpression != nullptr)
        this->valueExpression->~Expression();
    for (auto modifier : this->type.modifierValues)
        if (modifier != nullptr)
            modifier->~Expression();
}



} // namespace GPULang
