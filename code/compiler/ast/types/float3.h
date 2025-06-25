#pragma once
//------------------------------------------------------------------------------
/**
    Float vector of 3

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

#define FLOAT3_CTOR_LIST\
    X(Float3, ctor_SingleValue, Float, 1, 3, 3, None)\
    X(Float3, ctor_SingleUIntValue, UInt, 1, 3, 3, UIntToFloat)\
    X(Float3, ctor_SingleIntValue, Int, 1, 3, 3, IntToFloat)\
    X(Float3, ctor_UInt2, UInt3, 1, 1, 3, UIntToFloat)\
    X(Float3, ctor_Int2, Int3, 1, 1, 3, IntToFloat)\
    X(Float3, ctor_Float3, Float3, 1, 1, 3, None)

#define FLOAT3_CTOR2_LIST\
    X(Float3, ctor_Float2_Float, Float2, Float)\
    X(Float3, ctor_Float_Float2, Float, Float2)

struct Float3 : public Type
{
    /// constructor
    Float3();
};

extern Float3 Float3Type;

extern Function Float3_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    extern Function type##_##ctor;

    FLOAT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    extern Function type##_##ctor;

    FLOAT3_CTOR2_LIST
#undef X

extern Function Float3_additionOperator;
extern Function Float3_subtractionOperator;
extern Function Float3_multiplicationOperator;
extern Function Float3_divisionOperator;
extern Function Float3_modOperator;
extern Function Float3_scaleOperator;

extern Function Float3_matrix34Mul;
extern Function Float3_matrix33Mul;
extern Function Float3_matrix32Mul;

extern Function Float3_additionAssignOperator;
extern Function Float3_subtractionAssignOperator;
extern Function Float3_multiplicationAssignOperator;
extern Function Float3_divisionAssignOperator;

extern Function Float3_ltOperator;
extern Function Float3_lteOperator;
extern Function Float3_gtOperator;
extern Function Float3_gteOperator;
extern Function Float3_eOperator;
extern Function Float3_neOperator;

extern Function Float3_elementAccessOperatorInt;
extern Function Float3_elementAccessOperatorUInt;

} // namespace GPULang
