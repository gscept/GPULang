#pragma once
//------------------------------------------------------------------------------
/**
    Integer vector of 4

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

#define INT4_CTOR_LIST\
    X(Int4, ctor_SingleValue, i32, 1, 4, 4, None)\
    X(Int4, ctor_SingleUIntValue, u32, 1, 4, 4, UIntToInt)\
    X(Int4, ctor_SingleBoolValue, b8, 1, 4, 4, BoolToInt)\
    X(Int4, ctor_UInt4, u32x4, 1, 1, 4, UIntToInt)\
    X(Int4, ctor_Float4, f32x4, 1, 1, 4, FloatToUInt)\
    X(Int4, ctor_Bool4, b8x4, 1, 1, 4, BoolToUInt)\
    X(Int4, ctor_Int4, i32x4, 1, 1, 4, None)

#define INT4_CTOR2_LIST\
    X(Int4, ctor_Int3_Int, i32x3, i32)\
    X(Int4, ctor_Int_Int3, i32, i32x3)\
    X(Int4, ctor_Int2_Int2, i32x2, i32x2)
    
#define INT4_CTOR3_LIST\
    X(Int4, ctor_Int2_Int_Int, i32x2, i32, i32)\
    X(Int4, ctor_Int_Int_Int2, i32, i32, i32x2)\
    X(Int4, ctor_Int_Int2_Int, i32, i32x2, i32)

struct Int4 : public Type
{
    /// constructor
    Int4();

    static Function ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    static Function ctor;

    INT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    static Function ctor;

    INT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    static Function ctor;

    INT4_CTOR3_LIST
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
