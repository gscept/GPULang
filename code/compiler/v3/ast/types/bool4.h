#pragma once
//------------------------------------------------------------------------------
/**
    Bool vector of 4

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Bool4 : public Type
{
    ///constructor
    Bool4();

    Function ctor_XYZW;
    Function ctorSingleValue;

    Function elementAccessOperatorInt;
    Function elementAccessOperatorUInt;
};

} // namespace GPULang
