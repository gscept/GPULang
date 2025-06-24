//------------------------------------------------------------------------------
//  @file int4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int4.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Int4::Int4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Int;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Int4_ctor, i32x4, i32x4, Int4);
    __ADD_FUNCTION_PARAM(x, i32);
    __ADD_FUNCTION_PARAM(y, i32);
    __ADD_FUNCTION_PARAM(z, i32);
    __ADD_FUNCTION_PARAM(w, i32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, i32x4, i32x4, val, Int4);

    INT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, i32x4, i32x4, Int4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    INT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(type##_##ctor, i32x4, i32x4, Int4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)\
    __ADD_CONSTRUCTOR();

    INT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Int4_orOperator, operator|, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_andOperator, operator&, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_xorOperator, operator^, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_leftShiftOperator, operator<<, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_rightShiftOperator, operator>>, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_additionOperator, operator+, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_subtractionOperator, operator-, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_multiplicationOperator, operator*, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_divisionOperator, operator/, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_modOperator, operator%, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_scaleOperator, operator*, i32x4, i32);

    __IMPLEMENT_FUNCTION_1(Int4_orAssignOperator, operator|=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_andAssignOperator, operator&=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_xorAssignOperator, operator^=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_leftShiftAssignOperator, operator<<=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_rightShiftAssignOperator, operator>>=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_additionAssignOperator, operator+=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_subtractionAssignOperator, operator-=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_multiplicationAssignOperator, operator*=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_divisionAssignOperator, operator/=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_moduloAssignOperator, operator%=, i32x4, i32x4);

    __IMPLEMENT_FUNCTION_1(Int4_ltOperator, operator<, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_lteOperator, operator<=, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_gtOperator, operator>, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_gteOperator, operator>=, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_eOperator, operator==, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(Int4_neOperator, operator!=, b8x4, i32x4);

    __IMPLEMENT_FUNCTION_1(Int4_elementAccessOperatorInt, operator[], i32, i32);
    __IMPLEMENT_FUNCTION_1(Int4_elementAccessOperatorUInt, operator[], i32, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(i32, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(i32, 4, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
