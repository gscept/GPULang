#pragma once
//------------------------------------------------------------------------------
/**
    Aliases define one type as the name of another

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <string>
#include "expressions/expression.h"

namespace GPULang 
{

struct Alias : public Expression
{
    /// constructor
    Alias();

    Symbol::Location nameLocation, typeLocation;
    FixedString name, type;

    /// trigger an end-of-parsing event, this is where objects can setup their compiler contextual states
    bool Resolve(Compiler* compiler) override;
};

} // namespace GPULang
