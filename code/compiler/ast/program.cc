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
Program::Program()
{
    this->symbolType = ProgramType;
    this->resolved = Alloc<Program::__Resolved>();
    Program::__Resolved* progResolved = static_cast<Program::__Resolved*>(this->resolved);
    progResolved->usage.bits = 0x0;
    for (uint32_t i = 0; i < Program::__Resolved::ProgramEntryType::NumProgramEntries; i++)
        progResolved->mappings[i] = nullptr;
}

//------------------------------------------------------------------------------
/**
*/
Program::~Program()
{
    this->CleanupAnnotations();
    for (auto entry : this->entries)
        entry->~Expression();
}

static ConstantString NoProgramEntry = "";

//------------------------------------------------------------------------------
/**
*/
const Program::__Resolved::ProgramEntryType 
Program::__Resolved::StringToEntryType(const TransientString& str)
{
    auto it = programEntryTypeLookup.Find(str);
    if (it != programEntryTypeLookup.end())
        return it->second;
    else
        return Program::__Resolved::ProgramEntryType::InvalidProgramEntryType;

}

//------------------------------------------------------------------------------
/**
*/
const ConstantString&
Program::__Resolved::EntryTypeToString(const ProgramEntryType type)
{
    for (auto& it : programEntryTypeLookup)
    {
        if (it.second == type)
            return it.first;
    }
    return NoProgramEntry;
}

} // namespace GPULang
