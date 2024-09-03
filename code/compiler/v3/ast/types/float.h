#pragma once
//------------------------------------------------------------------------------
/**
    Float type

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace AnyFX
{

struct Float : public Type
{
    Float();

    Function ctorUInt;
    Function ctorInt;

    Function additionOperator;
    Function subtractionOperator;
    Function multiplicationOperator;
    Function divisionOperator;
    Function additionAssignOperator;
    Function subtractionAssignOperator;
    Function multiplicationAssignOperator;
    Function divisionAssignOperator;

    Function ltOperator;
    Function lteOperator;
    Function gtOperator;
    Function gteOperator;
    Function eOperator;
    Function neOperator;

    Function intAssignOperator;
    Function uintAssignOperator;

};

} // namespace AnyFX
