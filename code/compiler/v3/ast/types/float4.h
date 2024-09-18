#pragma once
//------------------------------------------------------------------------------
/**
    Float vector of 4

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Float4 : public Type
{
    /// constructor
    Float4();

    static Function ctor_XYZW;
    static Function ctorSingleValue;
    static Function ctor_f32x3_W;
    static Function ctor_f32x2_ZW;

    static Function additionOperator;
    static Function subtractionOperator;
    static Function multiplicationOperator;
    static Function divisionOperator;
    static Function modOperator;
    static Function scaleOperator;

    static Function matrix44Mul;
    static Function matrix43Mul;
    static Function matrix42Mul;

    static Function additionAssignOperator;
    static Function subtractionAssignOperator;
    static Function multiplicationAssignOperator;
    static Function divisionAssignOperator;

    static Function elementAccessOperatorInt;
    static Function elementAccessOperatorUInt;
};

} // namespace GPULang
