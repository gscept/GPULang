//------------------------------------------------------------------------------
//  @file float2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float2.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Float2::Float2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Float2_ctor, f32x2, f32x2, Float2);
    __ADD_FUNCTION_PARAM(x, f32);
    __ADD_FUNCTION_PARAM(y, f32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, f32x2, f32x2, val, Float2);

    FLOAT2_CTOR_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Float2_additionOperator, operator+, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_subtractionOperator, operator-, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_multiplicationOperator, operator*, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_divisionOperator, operator/, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_modOperator, operator%, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_scaleOperator, operator*, f32x2, f32);
    __IMPLEMENT_FUNCTION_1(Float2_additionAssignOperator, operator+=, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_subtractionAssignOperator, operator-=, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_multiplicationAssignOperator, operator*=, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_divisionAssignOperator, operator/=, f32x2, f32x2);

    __IMPLEMENT_FUNCTION_1(Float2_ltOperator, operator<, b8x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_lteOperator, operator<=, b8x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_gtOperator, operator>, b8x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_gteOperator, operator>=, b8x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_eOperator, operator==, b8x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Float2_neOperator, operator!=, b8x2, f32x2);

    __IMPLEMENT_FUNCTION_1(Float2_matrix24Mul, operator*, f32x2, f32x2x4);
    __IMPLEMENT_FUNCTION_1(Float2_matrix23Mul, operator*, f32x2, f32x2x3);
    __IMPLEMENT_FUNCTION_1(Float2_matrix22Mul, operator*, f32x2, f32x2x2);

    __IMPLEMENT_FUNCTION_1(Float2_elementAccessOperatorInt, operator[], f32, i32);
    __IMPLEMENT_FUNCTION_1(Float2_elementAccessOperatorUInt, operator[], f32, u32);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(f32, 2, swizzleMask)

    char colorMask[] = { 'r', 'g' };
    __IMPLEMENT_SWIZZLE(f32, 2, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
