//------------------------------------------------------------------------------
//  @file float3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float3.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Float3::Float3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x3");
    this->baseType = TypeCode::Float;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Float3_ctor, f32x3, Float3);
    __ADD_FUNCTION_PARAM(x, Float);
    __ADD_FUNCTION_PARAM(y, Float);
    __ADD_FUNCTION_PARAM(z, Float);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, f32x3, Float3, val);

    FLOAT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, f32x3, Float3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    FLOAT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Float3_additionOperator, operator+, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_subtractionOperator, operator-, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_multiplicationOperator, operator*, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_divisionOperator, operator/, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_modOperator, operator%, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_scaleOperator, operator*, Float3, Float);

    __IMPLEMENT_FUNCTION_1(Float3_matrix34Mul, operator*, Float3, Mat3x4);
    __IMPLEMENT_FUNCTION_1(Float3_matrix33Mul, operator*, Float3, Mat3x3);
    __IMPLEMENT_FUNCTION_1(Float3_matrix32Mul, operator*, Float3, Mat3x2);

    __IMPLEMENT_FUNCTION_1(Float3_additionAssignOperator, operator+=, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_subtractionAssignOperator, operator-=, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_multiplicationAssignOperator, operator*=, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_divisionAssignOperator, operator/=, Float3, Float3);

    __IMPLEMENT_FUNCTION_1(Float3_ltOperator, operator<, Bool3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_lteOperator, operator<=, Bool3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_gtOperator, operator>, Bool3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_gteOperator, operator>=, Bool3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_eOperator, operator==, Bool3, Float3);
    __IMPLEMENT_FUNCTION_1(Float3_neOperator, operator!=, Bool3, Float3);

    __IMPLEMENT_FUNCTION_1(Float3_elementAccessOperatorInt, operator[], Float, Int);
    __IMPLEMENT_FUNCTION_1(Float3_elementAccessOperatorUInt, operator[], Float, UInt);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(Float, 3, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(Float, 3, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
