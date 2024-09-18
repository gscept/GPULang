#pragma once
//------------------------------------------------------------------------------
/**
    Float vector of 3

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Float3 : public Type
{
    /// constructor
    Float3();

    static Function ctor_XYZ;
    static Function ctor_f32x2_Z;
    static Function ctorSingleValue;

    static Function additionOperator;
    static Function subtractionOperator;
    static Function multiplicationOperator;
    static Function divisionOperator;
    static Function modOperator;
    static Function scaleOperator;

    static Function matrix34Mul;
    static Function matrix33Mul;
    static Function matrix32Mul;

    static Function additionAssignOperator;
    static Function subtractionAssignOperator;
    static Function multiplicationAssignOperator;
    static Function divisionAssignOperator;

    static Function elementAccessOperatorInt;
    static Function elementAccessOperatorUInt;
};

} // namespace GPULang
