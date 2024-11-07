#pragma once
//------------------------------------------------------------------------------
/**
    Discard statement

    (C) 2024 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
namespace GPULang
{
struct DiscardStatement : public Statement
{
    /// Constructor
    DiscardStatement();
};

}