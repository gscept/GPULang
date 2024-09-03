#pragma once
//------------------------------------------------------------------------------
/**
    Integer type

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Int : public Type
{
    /// constructor
    Int();

    Function ctorUInt;

    Function andOperator;
    Function orOperator;
    Function xorOperator;
    Function leftShiftOperator;
    Function rightShiftOperator;
    Function andAssignOperator;
    Function orAssignOperator;
    Function xorAssignOperator;
    Function leftShiftAssignOperator;
    Function rightShiftAssignOperator;

    Function ororOperator;
    Function andandOperator;

    Function additionOperator;
    Function subtractionOperator;
    Function multiplicationOperator;
    Function divisionOperator;
    Function moduloOperator;
    Function additionAssignOperator;
    Function subtractionAssignOperator;
    Function multiplicationAssignOperator;
    Function divisionAssignOperator;
    Function moduloAssignOperator;

    Function ltOperator;
    Function lteOperator;
    Function gtOperator;
    Function gteOperator;
    Function eOperator;
    Function neOperator;

    Function assignOperator;
};

} // namespace GPULang
