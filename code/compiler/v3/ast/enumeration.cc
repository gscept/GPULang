//------------------------------------------------------------------------------
//  @file enumeration.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "enumeration.h"
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
Enumeration::Enumeration()
{
    this->symbolType = Symbol::EnumerationType;
    this->category = Type::Category::EnumCategory;
    this->resolved = new Enumeration::__Resolved;
}
} // namespace AnyFX
