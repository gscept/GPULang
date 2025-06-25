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
    X(UInt4, ctor_SingleValue, UInt, 1, 4, 4, None)\
    X(UInt4, ctor_SingleIntValue, Int, 1, 4, 4, IntToUInt)\
    X(UInt4, ctor_SingleBoolValue, Bool, 1, 4, 4, BoolToUInt)\
    X(UInt4, ctor_Int4, Int4, 1, 1, 4, IntToUInt)\
    X(UInt4, ctor_Float4, Float4, 1, 1, 4, FloatToUInt)\
    X(UInt4, ctor_Bool4, Bool4, 1, 1, 4, BoolToUInt)\
    X(UInt4, ctor_UInt4, UInt4, 1, 1, 4, None)

#define UINT4_CTOR2_LIST\
    X(UInt4, ctor_UInt3_UInt, UInt3, UInt)\
    X(UInt4, ctor_UInt_UInt3, UInt, UInt3)\
    X(UInt4, ctor_UInt2_UInt2, UInt2, UInt2)\

#define UINT4_CTOR3_LIST\
    X(UInt4, ctor_UInt2_UInt_UInt, UInt2, UInt, UInt)\
    X(UInt4, ctor_UInt_UInt_UInt2, UInt, UInt, UInt2)\
    X(UInt4, ctor_UInt_UInt2_UInt, UInt, UInt2, UInt)

struct UInt4 : public Type
{
    /// constructor
    UInt4();
};
extern UInt4 UInt4Type;

#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

UINT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    extern Function type##_##ctor;

UINT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    extern Function type##_##ctor;

UINT4_CTOR3_LIST
#undef X


extern Function UInt4_ctor;

extern Function UInt4_andOperator;
extern Function UInt4_orOperator;
extern Function UInt4_xorOperator;
extern Function UInt4_leftShiftOperator;
extern Function UInt4_rightShiftOperator;
extern Function UInt4_additionOperator;
extern Function UInt4_subtractionOperator;
extern Function UInt4_multiplicationOperator;
extern Function UInt4_divisionOperator;
extern Function UInt4_modOperator;
extern Function UInt4_scaleOperator;

extern Function UInt4_andAssignOperator;
extern Function UInt4_orAssignOperator;
extern Function UInt4_xorAssignOperator;
extern Function UInt4_leftShiftAssignOperator;
extern Function UInt4_rightShiftAssignOperator;
extern Function UInt4_additionAssignOperator;
extern Function UInt4_subtractionAssignOperator;
extern Function UInt4_multiplicationAssignOperator;
extern Function UInt4_divisionAssignOperator;
extern Function UInt4_moduloAssignOperator;

extern Function UInt4_ltOperator;
extern Function UInt4_lteOperator;
extern Function UInt4_gtOperator;
extern Function UInt4_gteOperator;
extern Function UInt4_eOperator;
extern Function UInt4_neOperator;

extern Function UInt4_elementAccessOperatorInt;
extern Function UInt4_elementAccessOperatorUInt;

} // namespace GPULang
