#pragma once
//------------------------------------------------------------------------------
/**
    Float vector of 2

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

#define FLOAT2_CTOR_LIST\
    X(Float2, ctor_SingleValue, f32, 1, 2, 2, None)\
    X(Float2, ctor_SingleUIntValue, u32, 1, 2, 2, UIntToFloat)\
    X(Float2, ctor_SingleIntValue, i32, 1, 2, 2, IntToFloat)\
    X(Float2, ctor_UInt2, u32x2, 1, 1, 2, UIntToFloat)\
    X(Float2, ctor_Int2, i32x2, 1, 1, 2, IntToFloat)\
    X(Float2, ctor_Float2, f32x2, 1, 1, 2, None)

struct Float2 : public Type
{
    /// constructor
    Float2();

    static Function ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    static Function ctor;

    FLOAT2_CTOR_LIST
#undef X

    static Function additionOperator;
    static Function subtractionOperator;
    static Function multiplicationOperator;
    static Function divisionOperator;
    static Function modOperator;
    static Function scaleOperator;

    static Function matrix24Mul;
    static Function matrix23Mul;
    static Function matrix22Mul;

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

    static Function elementAccessOperatorInt;
    static Function elementAccessOperatorUInt;
};

} // namespace GPULang
