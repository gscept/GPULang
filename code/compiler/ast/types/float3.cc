//------------------------------------------------------------------------------
//  @file float3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float3.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Float3::Float3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Float3_ctor, f32x3, f32x3, Float3);
    __ADD_FUNCTION_PARAM(x, f32);
    __ADD_FUNCTION_PARAM(y, f32);
    __ADD_FUNCTION_PARAM(z, f32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, f32x3, f32x3, val, Float3);

    FLOAT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, f32x3, f32x3, Float3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    FLOAT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Float3_additionOperator, operator+, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_subtractionOperator, operator-, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_multiplicationOperator, operator*, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_divisionOperator, operator/, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_modOperator, operator%, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_scaleOperator, operator*, f32x3, f32);

    __IMPLEMENT_FUNCTION_1(Float3_matrix34Mul, operator*, f32x3, f32x3x4);
    __IMPLEMENT_FUNCTION_1(Float3_matrix33Mul, operator*, f32x3, f32x3x3);
    __IMPLEMENT_FUNCTION_1(Float3_matrix32Mul, operator*, f32x3, f32x3x2);

    __IMPLEMENT_FUNCTION_1(Float3_additionAssignOperator, operator+=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_subtractionAssignOperator, operator-=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_multiplicationAssignOperator, operator*=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_divisionAssignOperator, operator/=, f32x3, f32x3);

    __IMPLEMENT_FUNCTION_1(Float3_ltOperator, operator<, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_lteOperator, operator<=, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_gtOperator, operator>, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_gteOperator, operator>=, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_eOperator, operator==, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(Float3_neOperator, operator!=, b8x3, f32x3);

    __IMPLEMENT_FUNCTION_1(Float3_elementAccessOperatorInt, operator[], f32, i32);
    __IMPLEMENT_FUNCTION_1(Float3_elementAccessOperatorUInt, operator[], f32, u32);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(f32, 3, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(f32, 3, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
