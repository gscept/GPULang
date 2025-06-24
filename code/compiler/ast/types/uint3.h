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
};
extern UInt3 UInt3Type;

#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

UINT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    extern Function type##_##ctor;

UINT3_CTOR2_LIST
#undef X

extern Function UInt3_ctor;

extern Function UInt3_andOperator;
extern Function UInt3_orOperator;
extern Function UInt3_xorOperator;
extern Function UInt3_leftShiftOperator;
extern Function UInt3_rightShiftOperator;
extern Function UInt3_additionOperator;
extern Function UInt3_subtractionOperator;
extern Function UInt3_multiplicationOperator;
extern Function UInt3_divisionOperator;
extern Function UInt3_modOperator;
extern Function UInt3_scaleOperator;

extern Function UInt3_andAssignOperator;
extern Function UInt3_orAssignOperator;
extern Function UInt3_xorAssignOperator;
extern Function UInt3_leftShiftAssignOperator;
extern Function UInt3_rightShiftAssignOperator;
extern Function UInt3_additionAssignOperator;
extern Function UInt3_subtractionAssignOperator;
extern Function UInt3_multiplicationAssignOperator;
extern Function UInt3_divisionAssignOperator;
extern Function UInt3_moduloAssignOperator;

extern Function UInt3_ltOperator;
extern Function UInt3_lteOperator;
extern Function UInt3_gtOperator;
extern Function UInt3_gteOperator;
extern Function UInt3_eOperator;
extern Function UInt3_neOperator;

extern Function UInt3_elementAccessOperatorInt;
extern Function UInt3_elementAccessOperatorUInt;

} // namespace GPULang
