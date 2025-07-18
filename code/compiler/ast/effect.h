#pragma once
//------------------------------------------------------------------------------
/**
    AST for the entry effect

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "symbol.h"
#include <vector>
namespace GPULang
{

struct Effect
{
    PinnedArray<Symbol*> symbols = 0xFFFFFF;
    void Destroy();
};

} // namespace GPULang
