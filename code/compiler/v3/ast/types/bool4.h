#pragma once
//------------------------------------------------------------------------------
/**
    Bool vector of 4

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace AnyFX
{

struct Bool4 : public Type
{
    ///constructor
    Bool4();

    Function ctorSingleValue;
    Function ctorXYZW;

    Function bracketOperatorInt;
    Function bracketOperatorUInt;
};

} // namespace AnyFX
