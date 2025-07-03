//------------------------------------------------------------------------------
//  @file float4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float4.h"
#include "builtins.h"

namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Float4::Float4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x4");
    this->baseType = TypeCode::Float;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Float4_ctor, f32x4, Float4);
    __ADD_FUNCTION_PARAM(x, Float);
    __ADD_FUNCTION_PARAM(y, Float);
    __ADD_FUNCTION_PARAM(z, Float);
    __ADD_FUNCTION_PARAM(w, Float);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, f32x4, Float4, val);

    FLOAT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, f32x4, Float4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    FLOAT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(type##_##ctor, f32x4, Float4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)\
    __ADD_CONSTRUCTOR();

    FLOAT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Float4_additionOperator, operator+, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_subtractionOperator, operator-, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_multiplicationOperator, operator*, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_divisionOperator, operator/, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_modOperator, operator%, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_scaleOperator, operator*, Float4, Float);

    __IMPLEMENT_FUNCTION_1(Float4_matrix44Mul, operator*, Float4, Mat4x4);
    __IMPLEMENT_FUNCTION_1(Float4_matrix43Mul, operator*, Float4, Mat4x3);
    __IMPLEMENT_FUNCTION_1(Float4_matrix42Mul, operator*, Float4, Mat4x2);

    __IMPLEMENT_FUNCTION_1(Float4_additionAssignOperator, operator+=, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_subtractionAssignOperator, operator-=, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_multiplicationAssignOperator, operator*=, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_divisionAssignOperator, operator/=, Float4, Float4);

    __IMPLEMENT_FUNCTION_1(Float4_ltOperator, operator<, Bool4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_lteOperator, operator<=, Bool4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_gtOperator, operator>, Bool4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_gteOperator, operator>=, Bool4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_eOperator, operator==, Bool4, Float4);
    __IMPLEMENT_FUNCTION_1(Float4_neOperator, operator!=, Bool4, Float4);

    __IMPLEMENT_FUNCTION_1(Float4_elementAccessOperatorInt, operator[], Float, Int);
    __IMPLEMENT_FUNCTION_1(Float4_elementAccessOperatorUInt, operator[], Float, UInt);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(Float, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(Float, 4, colorMask)
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
