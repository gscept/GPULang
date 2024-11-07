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

#define FLOAT3_CTOR_LIST\
    X(Float3, ctor_SingleValue, i32, 1, 3, 3, None)\
    X(Float3, ctor_UInt2, u32x3, 1, 1, 3, UIntToFloat)\
    X(Float3, ctor_Int2, i32x3, 1, 1, 3, IntToFloat)

#define FLOAT3_CTOR2_LIST\
    X(Float3, ctor_Float2_Float, f32x2, f32)\
    X(Float3, ctor_Float_Float2, f32, f32x2)

struct Float3 : public Type
{
    /// constructor
    Float3();

    static Function ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    static Function ctor;

    FLOAT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    static Function ctor;

    FLOAT3_CTOR2_LIST
#undef X

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
