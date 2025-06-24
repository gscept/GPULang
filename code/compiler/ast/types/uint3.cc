//------------------------------------------------------------------------------
//  @file uint3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint3.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UInt3::UInt3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::UInt;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(UInt3_ctor, u32x3, u32x3, UInt3);
    __ADD_FUNCTION_PARAM(x, u32);
    __ADD_FUNCTION_PARAM(y, u32);
    __ADD_FUNCTION_PARAM(z, u32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, u32x3, u32x3, val, UInt3);

    UINT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, u32x3, u32x3, UInt3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    UINT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(UInt3_orOperator, operator|, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_andOperator, operator&, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_xorOperator, operator^, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_leftShiftOperator, operator<<, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_rightShiftOperator, operator>>, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_additionOperator, operator+, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_subtractionOperator, operator-, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_multiplicationOperator, operator*, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_divisionOperator, operator/, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_modOperator, operator%, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_scaleOperator, operator*, u32x3, u32);

    __IMPLEMENT_FUNCTION_1(UInt3_orAssignOperator, operator|=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_andAssignOperator, operator&=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_xorAssignOperator, operator^=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_leftShiftAssignOperator, operator<<=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_rightShiftAssignOperator, operator>>=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_additionAssignOperator, operator+=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_subtractionAssignOperator, operator-=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_multiplicationAssignOperator, operator*=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_divisionAssignOperator, operator/=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_moduloAssignOperator, operator%=, u32x3, u32x3);

    __IMPLEMENT_FUNCTION_1(UInt3_ltOperator, operator<, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_lteOperator, operator<=, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_gtOperator, operator>, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_gteOperator, operator>=, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_eOperator, operator==, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(UInt3_neOperator, operator!=, b8x3, u32x3);

    __IMPLEMENT_FUNCTION_1(UInt3_elementAccessOperatorInt, operator[], u32, i32);
    __IMPLEMENT_FUNCTION_1(UInt3_elementAccessOperatorUInt, operator[], u32, u32);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(u32, 3, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(u32, 3, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
