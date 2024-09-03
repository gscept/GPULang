#pragma once
//------------------------------------------------------------------------------
/**
    Bool vector of 2

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Bool2 : public Type
{
    /// constructor
    Bool2();

    Function ctorXY;
    Function ctorSingleValue;

    Function bracketOperatorInt;
    Function bracketOperatorUInt;
};

} // namespace GPULang
