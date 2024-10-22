#pragma once
//------------------------------------------------------------------------------
/**
    Loader API loads an AnyFX binary

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <unordered_map>
#include <string>
#include "reflection/program.h"
#include "reflection/structure.h"
#include "reflection/variable.h"
#include "serialize.h"

namespace GPULang
{
using SamplerStateObject = Deserialize::SamplerState;
using VariableObject = Deserialize::Variable;
using StructureObject = Deserialize::Structure;
using ProgramObject = Deserialize::Program;
using RenderStateObject = Deserialize::RenderState;

namespace Deserialize
{
struct Deserializable;
}
struct Loader
{
    /// load from byte stream
    void Load(const char* data, const size_t length);
    /// Get symbol and cast to it's type
    template<typename T> T* Get(std::string name);

    std::unordered_map<std::string, GPULang::Deserialize::Deserializable*> nameToObject;
    std::vector<GPULang::Deserialize::Variable*> variables;
};

//------------------------------------------------------------------------------
/**
*/
template<typename T>
inline T* 
Loader::Get(std::string name)
{
    return static_cast<T*>(this->nameToObject[name.c_str()]);
}

} // namespace GPULang
