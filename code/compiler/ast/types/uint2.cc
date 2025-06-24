//------------------------------------------------------------------------------
//  @file uint2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint2.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UInt2::UInt2()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_TYPE()
    this->baseType = TypeCode::UInt;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(UInt2_ctor, u32x2, u32x2, UInt2);
    __ADD_FUNCTION_PARAM(x, u32);
    __ADD_FUNCTION_PARAM(y, u32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, u32x2, u32x2, val, UInt2);

    UINT2_CTOR_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(UInt2_orOperator, operator|, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_andOperator, operator&, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_xorOperator, operator^, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_leftShiftOperator, operator<<, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_rightShiftOperator, operator>>, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_additionOperator, operator+, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_subtractionOperator, operator-, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_multiplicationOperator, operator*, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_divisionOperator, operator/, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_modOperator, operator%, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_scaleOperator, operator*, u32x2, u32);

    __IMPLEMENT_FUNCTION_1(UInt2_orAssignOperator, operator|=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_andAssignOperator, operator&=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_xorAssignOperator, operator^=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_leftShiftAssignOperator, operator<<=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_rightShiftAssignOperator, operator>>=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_additionAssignOperator, operator+=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_subtractionAssignOperator, operator-=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_multiplicationAssignOperator, operator*=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_divisionAssignOperator, operator/=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_moduloAssignOperator, operator%=, u32x2, u32x2);

    __IMPLEMENT_FUNCTION_1(UInt2_ltOperator, operator<, b8x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_lteOperator, operator<=, b8x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_gtOperator, operator>, b8x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_gteOperator, operator>=, b8x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_eOperator, operator==, b8x2, u32x2);
    __IMPLEMENT_FUNCTION_1(UInt2_neOperator, operator!=, b8x2, u32x2);

    __IMPLEMENT_FUNCTION_1(UInt2_elementAccessOperatorInt, operator[], u32, i32);
    __IMPLEMENT_FUNCTION_1(UInt2_elementAccessOperatorUInt, operator[], u32, u32);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(u32, 2, swizzleMask)

    char colorMask[] = { 'r', 'g' };
    __IMPLEMENT_SWIZZLE(u32, 2, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
