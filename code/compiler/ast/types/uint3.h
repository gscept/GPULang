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

#define UINT3_CTOR_LIST\
    X(UInt3, ctor_SingleValue, u32, 1, 3, 3, None)\
    X(UInt3, ctor_SingleIntValue, i32, 1, 3, 3, IntToUInt)\
    X(UInt3, ctor_SingleBoolValue, b8, 1, 3, 3, BoolToUInt)\
    X(UInt3, ctor_Int3, i32x3, 1, 1, 3, IntToUInt)\
    X(UInt3, ctor_Float3, f32x3, 1, 1, 3, FloatToUInt)\
    X(UInt3, ctor_Bool3, b8x3, 1, 1, 3, BoolToUInt)\
    X(UInt3, ctor_UInt3, u32x3, 1, 1, 3, None)

#define UINT3_CTOR2_LIST\
    X(UInt3, ctor_UInt2_UInt, u32x2, u32)\
    X(UInt3, ctor_UInt_UInt2, u32, u32x2)

struct UInt3 : public Type
{
    /// constructor
    UInt3();

#define X(type, ctor, val, args, splat, size, conversion)\
    static Function ctor;

    UINT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    static Function ctor;

    UINT3_CTOR2_LIST
#undef X

    static Function ctor;

    static Function andOperator;
    static Function orOperator;
    static Function xorOperator;
    static Function leftShiftOperator;
    static Function rightShiftOperator;
    static Function additionOperator;
    static Function subtractionOperator;
    static Function multiplicationOperator;
    static Function divisionOperator;
    static Function modOperator;
    static Function scaleOperator;

    static Function andAssignOperator;
    static Function orAssignOperator;
    static Function xorAssignOperator;
    static Function leftShiftAssignOperator;
    static Function rightShiftAssignOperator;
    static Function additionAssignOperator;
    static Function subtractionAssignOperator;
    static Function multiplicationAssignOperator;
    static Function divisionAssignOperator;
    static Function moduloAssignOperator;

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
