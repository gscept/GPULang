//------------------------------------------------------------------------------
//  @file program.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "program.h"
namespace GPULang 
{

//------------------------------------------------------------------------------
/**
*/
ProgramInstance::ProgramInstance()
{
    this->symbolType = ProgramInstanceType;
    this->resolved = Alloc<ProgramInstance::__Resolved>();
    ProgramInstance::__Resolved* progResolved = static_cast<ProgramInstance::__Resolved*>(this->resolved);
    progResolved->usage.bits = 0x0;
    for (uint32_t i = 0; i < ProgramInstance::__Resolved::EntryType::NumProgramEntries; i++)
        progResolved->mappings[i] = nullptr;
}

//------------------------------------------------------------------------------
/**
*/
ProgramInstance::~ProgramInstance()
{
    this->CleanupAnnotations();
    for (auto entry : this->entries)
        entry->~Expression();
}

static ConstantString NoProgramEntry = "";

//------------------------------------------------------------------------------
/**
*/
const ProgramInstance::__Resolved::EntryType
ProgramInstance::__Resolved::StringToEntryType(const TransientString& str)
{
    auto it = programInstanceEntryTypeLookup.Find(str);
    if (it != programInstanceEntryTypeLookup.end())
        return it->second;
    else
        return ProgramInstance::__Resolved::EntryType::InvalidProgramEntryType;

}

//------------------------------------------------------------------------------
/**
*/
const ConstantString&
ProgramInstance::__Resolved::EntryTypeToString(const EntryType type)
{
    for (auto& it : programInstanceEntryTypeLookup)
    {
        if (it.second == type)
            return it.first;
    }
    return NoProgramEntry;
}

} // namespace GPULang
