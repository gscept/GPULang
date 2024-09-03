#pragma once
//------------------------------------------------------------------------------
/**
    Integer vector of 4

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace AnyFX
{

struct Int4 : public Type
{
    /// constructor
    Int4();

    Function ctorXYZW;
    Function ctorSingleValue;
    Function ctorInt3W;
    Function ctorInt2ZW;

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

    Function bracketOperatorInt;
    Function bracketOperatorUInt;
};

} // namespace AnyFX
