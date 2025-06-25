#pragma once
//------------------------------------------------------------------------------
/**
    Unsigned integer vector of 2

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

#define UINT2_CTOR_LIST\
    X(UInt2, ctor_SingleValue, UInt, 1, 2, 2, None)\
    X(UInt2, ctor_SingleIntValue, Int, 1, 2, 2, IntToUInt)\
    X(UInt2, ctor_SingleBoolValue, Bool, 1, 2, 2, BoolToUInt)\
    X(UInt2, ctor_Int2, Int2, 1, 1, 2, IntToUInt)\
    X(UInt2, ctor_Float2, Float2, 1, 1, 2, FloatToUInt)\
    X(UInt2, ctor_Bool2, Bool2, 1, 1, 2, BoolToUInt)\
    X(UInt2, ctor_UInt2, UInt2, 1, 1, 2, None)


struct UInt2 : public Type
{
    /// constructor
    UInt2();
};

#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

UINT2_CTOR_LIST
#undef X

extern UInt2 UInt2Type;
extern Function UInt2_ctor;
extern Function UInt2_andOperator;
extern Function UInt2_orOperator;
extern Function UInt2_xorOperator;
extern Function UInt2_leftShiftOperator;
extern Function UInt2_rightShiftOperator;
extern Function UInt2_additionOperator;
extern Function UInt2_subtractionOperator;
extern Function UInt2_multiplicationOperator;
extern Function UInt2_divisionOperator;
extern Function UInt2_modOperator;
extern Function UInt2_scaleOperator;

extern Function UInt2_andAssignOperator;
extern Function UInt2_orAssignOperator;
extern Function UInt2_xorAssignOperator;
extern Function UInt2_leftShiftAssignOperator;
extern Function UInt2_rightShiftAssignOperator;
extern Function UInt2_additionAssignOperator;
extern Function UInt2_subtractionAssignOperator;
extern Function UInt2_multiplicationAssignOperator;
extern Function UInt2_divisionAssignOperator;
extern Function UInt2_moduloAssignOperator;

extern Function UInt2_ltOperator;
extern Function UInt2_lteOperator;
extern Function UInt2_gtOperator;
extern Function UInt2_gteOperator;
extern Function UInt2_eOperator;
extern Function UInt2_neOperator;

extern Function UInt2_elementAccessOperatorInt;
extern Function UInt2_elementAccessOperatorUInt;

} // namespace GPULang
