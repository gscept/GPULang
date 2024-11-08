#pragma once
//------------------------------------------------------------------------------
/**
    Integer vector of 2

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

#define INT2_CTOR_LIST\
    X(Int2, ctor_SingleValue, i32, 1, 2, 2, None)\
    X(Int2, ctor_UInt2, u32x2, 1, 1, 2, UIntToInt)\
    X(Int2, ctor_Float2, f32x2, 1, 1, 2, FloatToUInt)\
    X(Int2, ctor_Int2, i32x2, 1, 1, 2, None)

struct Int2 : public Type
{
    /// constructor
    Int2();

    static Function ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    static Function ctor;

    INT2_CTOR_LIST
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
