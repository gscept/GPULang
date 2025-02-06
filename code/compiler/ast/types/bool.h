#pragma once
//------------------------------------------------------------------------------
/**
    Bool type

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Bool : public Type
{
    /// constructor
    Bool();

    static Function ctor_UInt;
    static Function ctor_Int;

    static Function ororOperator;
    static Function andandOperator;
    static Function eOperator;
    static Function neOperator;
    
    static Function boolAssignOperator;
    static Function intAssignOperator;
    static Function uintAssignOperator;
};

} // namespace GPULang
