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

    static Function ctor_XYZW;
    static Function ctorSingleValue;
    
    static Function ororOperator;
    static Function andandOperator;
    static Function eOperator;
    static Function neOperator;
    
    static Function elementAccessOperatorInt;
    static Function elementAccessOperatorUInt;
};

} // namespace GPULang
