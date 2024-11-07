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

    static Function ctor_XYZ;
    static Function ctorSingleValue;
    static Function elementAccessOperatorInt;
    static Function elementAccessOperatorUInt;
};

} // namespace GPULang
