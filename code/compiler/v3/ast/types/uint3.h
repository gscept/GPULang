#pragma once
//------------------------------------------------------------------------------
/**
    Unsigned integer vector of 3

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct UInt3 : public Type
{
    /// constructor
    UInt3();

    Function ctorXYZ;
    Function ctorSingleValue;

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

} // namespace GPULang
