//------------------------------------------------------------------------------
//  @file structure.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "structure.h"
#include "compiler.h"
#include "function.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Structure::Structure()
{
    this->symbolType = StructureType;
    this->baseType = TypeCode::InvalidType;
    this->resolved = Alloc<Structure::__Resolved>();
    this->category = Type::Category::UserTypeCategory;
    this->arraySizeExpression = nullptr;
    this->isArray = false;

    Structure::__Resolved* typeResolved = static_cast<Structure::__Resolved*>(this->resolved);
    typeResolved->usageFlags.bits = 0x0;
    typeResolved->accessBits.bits = 0x0;
    typeResolved->byteSize = 0;
    typeResolved->baseAlignment = 0;
    typeResolved->packMembers = false;
    typeResolved->storageFunction = nullptr;
    typeResolved->loadFunction = nullptr;
}

//------------------------------------------------------------------------------
/**
*/
Structure::~Structure()
{
    this->CleanupAnnotations();
    this->CleanupAttributes();
    for (auto sym : this->symbols)
        sym->~Symbol();
    if (this->arraySizeExpression != nullptr)
        this->arraySizeExpression->~Expression();
}

//------------------------------------------------------------------------------
/**
*/
Structure::__Resolved::~__Resolved()
{
    if (this->storageFunction)
        this->storageFunction->~Function();
    if (this->loadFunction)
        this->loadFunction->~Function();
}

} // namespace GPULang
