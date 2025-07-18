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
    this->CleanupAnnotations();
    this->CleanupAttributes();
    if (this->valueExpression != nullptr)
        this->valueExpression->~Expression();
    for (auto modifier : this->type.modifierValues)
        if (modifier != nullptr)
            modifier->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
void
Variable::SetupAsBuiltinParameter()
{
    Variable::__Resolved* varResolved = static_cast<Variable::__Resolved*>(this->resolved);
    varResolved->type = this->type;
    varResolved->accessBits.flags.readAccess = true; // Implicitly set read access to true
    varResolved->byteSize = varResolved->typeSymbol->byteSize;
    varResolved->storage = Storage::Default;
    varResolved->usageBits.flags.isParameter = true;
}

} // namespace GPULang
