//------------------------------------------------------------------------------
//  @file float4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float4.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Float4::Float4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Float4_ctor, f32x4, f32x4, Float4);
    __ADD_FUNCTION_PARAM(x, f32);
    __ADD_FUNCTION_PARAM(y, f32);
    __ADD_FUNCTION_PARAM(z, f32);
    __ADD_FUNCTION_PARAM(w, f32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, f32x4, f32x4, val, Float4);

    FLOAT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, f32x4, f32x4, Float4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    FLOAT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(type##_##ctor, f32x4, f32x4, Float4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)\
    __ADD_CONSTRUCTOR();

    FLOAT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Float4_additionOperator, operator+, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_subtractionOperator, operator-, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_multiplicationOperator, operator*, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_divisionOperator, operator/, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_modOperator, operator%, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_scaleOperator, operator*, f32x4, f32);

    __IMPLEMENT_FUNCTION_1(Float4_matrix44Mul, operator*, f32x4, f32x4x4);
    __IMPLEMENT_FUNCTION_1(Float4_matrix43Mul, operator*, f32x4, f32x4x3);
    __IMPLEMENT_FUNCTION_1(Float4_matrix42Mul, operator*, f32x4, f32x4x2);

    __IMPLEMENT_FUNCTION_1(Float4_additionAssignOperator, operator+=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_subtractionAssignOperator, operator-=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_multiplicationAssignOperator, operator*=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_divisionAssignOperator, operator/=, f32x4, f32x4);

    __IMPLEMENT_FUNCTION_1(Float4_ltOperator, operator<, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_lteOperator, operator<=, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_gtOperator, operator>, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_gteOperator, operator>=, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_eOperator, operator==, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Float4_neOperator, operator!=, b8x4, f32x4);

    __IMPLEMENT_FUNCTION_1(Float4_elementAccessOperatorInt, operator[], f32, i32);
    __IMPLEMENT_FUNCTION_1(Float4_elementAccessOperatorUInt, operator[], f32, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(f32, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(f32, 4, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
