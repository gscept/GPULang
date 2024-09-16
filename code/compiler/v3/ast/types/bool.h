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

    Function ctor_UInt;
    Function ctor_Int;

    Function orOperator;
    Function andOperator;
    Function xorOperator;
    Function orAssignOperator;
    Function andAssignOperator;
    Function xorAssignOperator;
    Function ororOperator;
    Function andandOperator;

    Function eOperator;
    Function neOperator;

    Function boolAssignOperator;
    Function intAssignOperator;
    Function uintAssignOperator;
};

} // namespace GPULang
