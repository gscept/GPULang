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
};
extern Float4 Float4Type;

extern Function Float4_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

FLOAT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    extern Function type##_##ctor;

FLOAT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg3)\
    extern Function type##_##ctor;

FLOAT4_CTOR3_LIST
#undef X


extern Function Float4_additionOperator;
extern Function Float4_subtractionOperator;
extern Function Float4_multiplicationOperator;
extern Function Float4_divisionOperator;
extern Function Float4_modOperator;
extern Function Float4_scaleOperator;

extern Function Float4_matrix44Mul;
extern Function Float4_matrix43Mul;
extern Function Float4_matrix42Mul;

extern Function Float4_additionAssignOperator;
extern Function Float4_subtractionAssignOperator;
extern Function Float4_multiplicationAssignOperator;
extern Function Float4_divisionAssignOperator;

extern Function Float4_ltOperator;
extern Function Float4_lteOperator;
extern Function Float4_gtOperator;
extern Function Float4_gteOperator;
extern Function Float4_eOperator;
extern Function Float4_neOperator;

extern Function Float4_elementAccessOperatorInt;
extern Function Float4_elementAccessOperatorUInt;

} // namespace GPULang
