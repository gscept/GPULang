#pragma once
//------------------------------------------------------------------------------
/**
    Scope of code to be generated

    @copyright
    (C) 2025 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "intrinsics.h"
#include "symbol.h"

namespace GPULang
{

struct Generate : public Symbol
{
    /// Constructor
    Generate(const PinnedArray<Symbol*>& symbols);
    /// Destructor
    virtual ~Generate();
    
    struct __Resolved : public Symbol::__Resolved
    {
        PinnedArray<Symbol*> generatedSymbols = 0xFFFF;
    }* thisResolved;
    
    PinnedArray<Symbol*> symbols;
};

}
