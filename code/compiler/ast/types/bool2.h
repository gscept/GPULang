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

    static Function ctor_XY;
    static Function ctorSingleValue;

    static Function ororOperator;
    static Function andandOperator;
    static Function eOperator;
    static Function neOperator;
    
    static Function elementAccessOperatorInt;
    static Function elementAccessOperatorUInt;
};

} // namespace GPULang
