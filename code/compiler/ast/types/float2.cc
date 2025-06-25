//------------------------------------------------------------------------------
//  @file float2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float2.h"
#include "builtins.h"

namespace GPULang
{
//------------------------------------------------------------------------------
/**
*/
Float2::Float2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x2");
    this->baseType = TypeCode::Float;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Float2_ctor, f32x2, Float2);
    __ADD_FUNCTION_PARAM(x, Float);
    __ADD_FUNCTION_PARAM(y, Float);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, f32x2, Float2, val);

    FLOAT2_CTOR_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Float2_additionOperator, operator+, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_subtractionOperator, operator-, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_multiplicationOperator, operator*, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_divisionOperator, operator/, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_modOperator, operator%, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_scaleOperator, operator*, Float2, Float);
    __IMPLEMENT_FUNCTION_1(Float2_additionAssignOperator, operator+=, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_subtractionAssignOperator, operator-=, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_multiplicationAssignOperator, operator*=, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_divisionAssignOperator, operator/=, Float2, Float2);

    __IMPLEMENT_FUNCTION_1(Float2_ltOperator, operator<, Bool2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_lteOperator, operator<=, Bool2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_gtOperator, operator>, Bool2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_gteOperator, operator>=, Bool2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_eOperator, operator==, Bool2, Float2);
    __IMPLEMENT_FUNCTION_1(Float2_neOperator, operator!=, Bool2, Float2);

    __IMPLEMENT_FUNCTION_1(Float2_matrix24Mul, operator*, Float2, Mat2x4);
    __IMPLEMENT_FUNCTION_1(Float2_matrix23Mul, operator*, Float2, Mat2x3);
    __IMPLEMENT_FUNCTION_1(Float2_matrix22Mul, operator*, Float2, Mat2x2);

    __IMPLEMENT_FUNCTION_1(Float2_elementAccessOperatorInt, operator[], Float, Int);
    __IMPLEMENT_FUNCTION_1(Float2_elementAccessOperatorUInt, operator[], Float, UInt);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(Float, 2, swizzleMask)

    char colorMask[] = { 'r', 'g' };
    __IMPLEMENT_SWIZZLE(Float, 2, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
