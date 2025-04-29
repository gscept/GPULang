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
    if (SYMBOL_STATIC_ALLOC)
        this->resolved = StaticAlloc<Enumeration::__Resolved>();
    else
        this->resolved = Alloc<Enumeration::__Resolved>();
    this->baseType = TypeCode::UInt;
}

//------------------------------------------------------------------------------
/**
*/
Enumeration::~Enumeration()
{
    for (auto val : this->values)
        if (val != nullptr)
            val->~Expression();

    for (auto sym : this->symbols)
        sym->~Symbol();
    for (auto sym : this->globals)
        sym->~Symbol();
    for (auto sym : this->staticSymbols)
        sym->~Symbol();

    for (auto modifier : this->type.modifierValues)
        if (modifier != nullptr)
            modifier->~Expression();

}
} // namespace GPULang
