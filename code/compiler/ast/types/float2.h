#pragma once
//------------------------------------------------------------------------------
/**
    Float vector of 2

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

#define FLOAT2_CTOR_LIST\
    X(Float2, ctor_SingleValue, f32, 1, 2, 2, None)\
    X(Float2, ctor_SingleUIntValue, u32, 1, 2, 2, UIntToFloat)\
    X(Float2, ctor_SingleIntValue, i32, 1, 2, 2, IntToFloat)\
    X(Float2, ctor_UInt2, u32x2, 1, 1, 2, UIntToFloat)\
    X(Float2, ctor_Int2, i32x2, 1, 1, 2, IntToFloat)\
    X(Float2, ctor_Float2, f32x2, 1, 1, 2, None)

struct Float2 : public Type
{
    /// constructor
    Float2();
};
extern Float2 Float2Type;

extern Function Float2_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

    FLOAT2_CTOR_LIST
#undef X

extern Function Float2_additionOperator;
extern Function Float2_subtractionOperator;
extern Function Float2_multiplicationOperator;
extern Function Float2_divisionOperator;
extern Function Float2_modOperator;
extern Function Float2_scaleOperator;

extern Function Float2_matrix24Mul;
extern Function Float2_matrix23Mul;
extern Function Float2_matrix22Mul;

extern Function Float2_additionAssignOperator;
extern Function Float2_subtractionAssignOperator;
extern Function Float2_multiplicationAssignOperator;
extern Function Float2_divisionAssignOperator;

extern Function Float2_ltOperator;
extern Function Float2_lteOperator;
extern Function Float2_gtOperator;
extern Function Float2_gteOperator;
extern Function Float2_eOperator;
extern Function Float2_neOperator;

extern Function Float2_elementAccessOperatorInt;
extern Function Float2_elementAccessOperatorUInt;

} // namespace GPULang
