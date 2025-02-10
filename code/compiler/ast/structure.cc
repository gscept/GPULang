//------------------------------------------------------------------------------
//  @file structure.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "structure.h"
#include "compiler.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Structure::Structure()
{
    this->symbolType = StructureType;
    this->baseType = TypeCode::InvalidType;
    this->resolved = new Structure::__Resolved;
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
}

} // namespace GPULang
