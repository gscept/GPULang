#pragma once
//------------------------------------------------------------------------------
/**
    Bool vector of 3

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Bool3 : public Type
{
    /// constructor
    Bool3();

    Function ctor_XYZ;
    Function ctorSingleValue;

    Function elementAccessOperatorInt;
    Function elementAccessOperatorUInt;
};

} // namespace GPULang
