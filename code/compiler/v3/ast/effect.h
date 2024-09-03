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
    ~Effect()
    {
        for (Symbol* sym : symbols)
            delete sym;
    }
    std::vector<Symbol*> symbols;
};

} // namespace GPULang
