#pragma once
//------------------------------------------------------------------------------
/**
    Statement is the base class for any action that the code can execute

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "ast/symbol.h"
namespace GPULang
{

struct Statement : public Symbol
{

};

} // namespace GPULang
