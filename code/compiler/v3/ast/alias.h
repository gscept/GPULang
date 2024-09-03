#pragma once
//------------------------------------------------------------------------------
/**
    Aliases define one type as the name of another

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <string>
#include "symbol.h"
namespace GPULang 
{

struct Alias : public Symbol
{
    /// constructor
    Alias();

    std::string name;
    std::string type;

    /// trigger an end-of-parsing event, this is where objects can setup their compiler contextual states
    bool EndOfParse(Compiler* compiler);
};

} // namespace GPULang
