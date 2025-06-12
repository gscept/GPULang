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
    Generate();
    /// Destructor
    virtual ~Generate();
    
    struct __Resolved : public Symbol::__Resolved
    {
        
    }* thisResolved;
    
    PinnedArray<Symbol*> symbols;
};

}
