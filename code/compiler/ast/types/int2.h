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
    X(Int2, ctor_SingleUIntValue, u32, 1, 2, 2, UIntToInt)\
    X(Int2, ctor_SingleBoolValue, b8, 1, 2, 2, BoolToInt)\
    X(Int2, ctor_UInt2, u32x2, 1, 1, 2, UIntToInt)\
    X(Int2, ctor_Float2, f32x2, 1, 1, 2, FloatToInt)\
    X(Int2, ctor_Bool2, b8x2, 1, 1, 2, BoolToInt)\
    X(Int2, ctor_Int2, i32x2, 1, 1, 2, None)

struct Int2 : public Type
{
    /// constructor
    Int2();
};
extern Int2 Int2Type;

extern Function Int2_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

INT2_CTOR_LIST
#undef X

extern Function Int2_andOperator;
extern Function Int2_orOperator;
extern Function Int2_xorOperator;
extern Function Int2_leftShiftOperator;
extern Function Int2_rightShiftOperator;
extern Function Int2_additionOperator;
extern Function Int2_subtractionOperator;
extern Function Int2_multiplicationOperator;
extern Function Int2_divisionOperator;
extern Function Int2_modOperator;
extern Function Int2_scaleOperator;

extern Function Int2_andAssignOperator;
extern Function Int2_orAssignOperator;
extern Function Int2_xorAssignOperator;
extern Function Int2_leftShiftAssignOperator;
extern Function Int2_rightShiftAssignOperator;
extern Function Int2_additionAssignOperator;
extern Function Int2_subtractionAssignOperator;
extern Function Int2_multiplicationAssignOperator;
extern Function Int2_divisionAssignOperator;
extern Function Int2_moduloAssignOperator;

extern Function Int2_ltOperator;
extern Function Int2_lteOperator;
extern Function Int2_gtOperator;
extern Function Int2_gteOperator;
extern Function Int2_eOperator;
extern Function Int2_neOperator;

extern Function Int2_elementAccessOperatorInt;
extern Function Int2_elementAccessOperatorUInt;

} // namespace GPULang
