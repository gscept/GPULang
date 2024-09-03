#pragma once
//------------------------------------------------------------------------------
/**
    Loader API loads an AnyFX binary

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <map>
#include <string>
#include "reflection/program.h"
#include "reflection/structure.h"
#include "reflection/variable.h"
namespace GPULang
{

namespace Deserialize
{
struct Deserializable;
}
struct Loader
{
    /// destructor
    ~Loader();

    /// load from byte stream
    void Load(const char* data, const size_t length);

    std::map<const char*, GPULang::Deserialize::Deserializable*> nameToObject;
};


} // namespace GPULang
