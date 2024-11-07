#pragma once
//------------------------------------------------------------------------------
/**
    Float type

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Float : public Type
{
    Float();

    static Function ctor_UInt;
    static Function ctor_Int;

    static Function additionOperator;
    static Function subtractionOperator;
    static Function multiplicationOperator;
    static Function divisionOperator;
    static Function modOperator;

    static Function additionAssignOperator;
    static Function subtractionAssignOperator;
    static Function multiplicationAssignOperator;
    static Function divisionAssignOperator;

    static Function ltOperator;
    static Function lteOperator;
    static Function gtOperator;
    static Function gteOperator;
    static Function eOperator;
    static Function neOperator;

    static Function intAssignOperator;
    static Function uintAssignOperator;

};

} // namespace GPULang
