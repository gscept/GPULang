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

#define FLOAT4_CTOR_LIST\
    X(Float4, ctor_SingleValue, f32, 1, 4, 4, None)\
    X(Float4, ctor_SingleUIntValue, u32, 1, 4, 4, UIntToFloat)\
    X(Float4, ctor_SingleIntValue, i32, 1, 4, 4, IntToFloat)\
    X(Float4, ctor_UInt4, u32x4, 1, 1, 4, UIntToFloat)\
    X(Float4, ctor_Int4, i32x4, 1, 1, 4, IntToFloat)\
    X(Float4, ctor_Float4, f32x4, 1, 1, 4, None)

#define FLOAT4_CTOR2_LIST\
    X(Float4, ctor_Float3_Float, f32x3, f32)\
    X(Float4, ctor_Float_Float3, f32, f32x3)\
    X(Float4, ctor_Float2_Float2, f32x2, f32x2)

#define FLOAT4_CTOR3_LIST\
    X(Float4, ctor_Float2_Float_Float, f32x2, f32, f32)\
    X(Float4, ctor_Float_Float_Float2, f32, f32, f32x2)\
    X(Float4, ctor_Float_Float2_Float, f32, f32x2, f32)


struct Float4 : public Type
{
    /// constructor
    Float4();

    static Function ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    static Function ctor;

    FLOAT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    static Function ctor;

    FLOAT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg3)\
    static Function ctor;

    FLOAT4_CTOR3_LIST
#undef X


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
