#pragma once
//------------------------------------------------------------------------------
/**
    Integer vector of 3

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

#define INT3_CTOR_LIST\
    X(Int3, ctor_SingleValue, i32, 1, 3, 3, None)\
    X(Int3, ctor_UInt3, u32x3, 1, 1, 3, UIntToInt)\
    X(Int3, ctor_Float3, f32x3, 1, 1, 3, FloatToUInt)\
    X(Int3, ctor_Int3, i32x3, 1, 1, 3, None)

#define INT3_CTOR2_LIST\
    X(Int3, ctor_Int2_Int, i32x2, i32)\
    X(Int3, ctor_Int_Int2, i32, i32x2)

struct Int3 : public Type
{
    /// constructor
    Int3();

    static Function ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    static Function ctor;

    INT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    static Function ctor;

    INT3_CTOR2_LIST
#undef X

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
