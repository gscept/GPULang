#pragma once
//------------------------------------------------------------------------------
/**
    Unsigned integer vector of 4

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

#define UINT4_CTOR_LIST\
    X(UInt4, ctor_SingleValue, u32, 1, 4, 4, None)\
    X(UInt4, ctor_SingleIntValue, i32, 1, 4, 4, IntToUInt)\
    X(UInt4, ctor_Int4, i32x4, 1, 1, 4, IntToUInt)\
    X(UInt4, ctor_Float4, f32x4, 1, 1, 4, FloatToUInt)\
    X(UInt4, ctor_UInt4, u32x4, 1, 1, 4, None)

#define UINT4_CTOR2_LIST\
    X(UInt4, ctor_UInt3_UInt, u32x3, u32)\
    X(UInt4, ctor_UInt_UInt3, u32, u32x3)\
    X(UInt4, ctor_UInt2_UInt2, u32x2, u32x2)\

#define UINT4_CTOR3_LIST\
    X(UInt4, ctor_UInt2_UInt_UInt, u32x2, u32, u32)\
    X(UInt4, ctor_UInt_UInt_UInt2, u32, u32, u32x2)\
    X(UInt4, ctor_UInt_UInt2_UInt, u32, u32x2, u32)

struct UInt4 : public Type
{
    /// constructor
    UInt4();

#define X(type, ctor, val, args, splat, size, conversion)\
    static Function ctor;

    UINT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    static Function ctor;

    UINT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    static Function ctor;

    UINT4_CTOR3_LIST
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
