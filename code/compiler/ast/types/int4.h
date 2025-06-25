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
    X(Int4, ctor_SingleValue, Int, 1, 4, 4, None)\
    X(Int4, ctor_SingleUIntValue, UInt, 1, 4, 4, UIntToInt)\
    X(Int4, ctor_SingleBoolValue, Bool, 1, 4, 4, BoolToInt)\
    X(Int4, ctor_UInt4, UInt4, 1, 1, 4, UIntToInt)\
    X(Int4, ctor_Float4, Float4, 1, 1, 4, FloatToInt)\
    X(Int4, ctor_Bool4, Bool4, 1, 1, 4, BoolToInt)\
    X(Int4, ctor_Int4, Int4, 1, 1, 4, None)

#define INT4_CTOR2_LIST\
    X(Int4, ctor_Int3_Int, Int3, Int)\
    X(Int4, ctor_Int_Int3, Int, Int3)\
    X(Int4, ctor_Int2_Int2, Int2, Int2)
    
#define INT4_CTOR3_LIST\
    X(Int4, ctor_Int2_Int_Int, Int2, Int, Int)\
    X(Int4, ctor_Int_Int_Int2, Int, Int, Int2)\
    X(Int4, ctor_Int_Int2_Int, Int, Int2, Int)

struct Int4 : public Type
{
    /// constructor
    Int4();
};
extern Int4 Int4Type;

extern Function Int4_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

INT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    extern Function type##_##ctor;

INT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    extern Function type##_##ctor;

INT4_CTOR3_LIST
#undef X

extern Function Int4_andOperator;
extern Function Int4_orOperator;
extern Function Int4_xorOperator;
extern Function Int4_leftShiftOperator;
extern Function Int4_rightShiftOperator;
extern Function Int4_additionOperator;
extern Function Int4_subtractionOperator;
extern Function Int4_multiplicationOperator;
extern Function Int4_divisionOperator;
extern Function Int4_modOperator;
extern Function Int4_scaleOperator;

extern Function Int4_andAssignOperator;
extern Function Int4_orAssignOperator;
extern Function Int4_xorAssignOperator;
extern Function Int4_leftShiftAssignOperator;
extern Function Int4_rightShiftAssignOperator;
extern Function Int4_additionAssignOperator;
extern Function Int4_subtractionAssignOperator;
extern Function Int4_multiplicationAssignOperator;
extern Function Int4_divisionAssignOperator;
extern Function Int4_moduloAssignOperator;

extern Function Int4_ltOperator;
extern Function Int4_lteOperator;
extern Function Int4_gtOperator;
extern Function Int4_gteOperator;
extern Function Int4_eOperator;
extern Function Int4_neOperator;

extern Function Int4_elementAccessOperatorInt;
extern Function Int4_elementAccessOperatorUInt;

} // namespace GPULang
