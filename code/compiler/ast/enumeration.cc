//------------------------------------------------------------------------------
//  @file enumeration.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "enumeration.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Enumeration::Enumeration()
{
    this->symbolType = Symbol::EnumerationType;
    this->category = Type::Category::EnumCategory;
    this->resolved = new Enumeration::__Resolved;
    this->baseType = TypeCode::UInt;
}
} // namespace GPULang
