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
    X(Int3, ctor_SingleUIntValue, u32, 1, 3, 3, UIntToInt)\
    X(Int3, ctor_SingleBoolValue, b8, 1, 3, 3, BoolToInt)\
    X(Int3, ctor_UInt3, u32x3, 1, 1, 3, UIntToInt)\
    X(Int3, ctor_Float3, f32x3, 1, 1, 3, FloatToInt)\
    X(Int3, ctor_Bool3, b8x3, 1, 1, 3, BoolToInt)\
    X(Int3, ctor_Int3, i32x3, 1, 1, 3, None)

#define INT3_CTOR2_LIST\
    X(Int3, ctor_Int2_Int, i32x2, i32)\
    X(Int3, ctor_Int_Int2, i32, i32x2)

struct Int3 : public Type
{
    /// constructor
    Int3();
};
extern Int3 Int3Type;

extern Function Int3_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

INT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    extern Function type##_##ctor;

INT3_CTOR2_LIST
#undef X

extern Function Int3_andOperator;
extern Function Int3_orOperator;
extern Function Int3_xorOperator;
extern Function Int3_leftShiftOperator;
extern Function Int3_rightShiftOperator;
extern Function Int3_additionOperator;
extern Function Int3_subtractionOperator;
extern Function Int3_multiplicationOperator;
extern Function Int3_divisionOperator;
extern Function Int3_modOperator;
extern Function Int3_scaleOperator;

extern Function Int3_andAssignOperator;
extern Function Int3_orAssignOperator;
extern Function Int3_xorAssignOperator;
extern Function Int3_leftShiftAssignOperator;
extern Function Int3_rightShiftAssignOperator;
extern Function Int3_additionAssignOperator;
extern Function Int3_subtractionAssignOperator;
extern Function Int3_multiplicationAssignOperator;
extern Function Int3_divisionAssignOperator;
extern Function Int3_moduloAssignOperator;

extern Function Int3_ltOperator;
extern Function Int3_lteOperator;
extern Function Int3_gtOperator;
extern Function Int3_gteOperator;
extern Function Int3_eOperator;
extern Function Int3_neOperator;

extern Function Int3_elementAccessOperatorInt;
extern Function Int3_elementAccessOperatorUInt;

} // namespace GPULang
