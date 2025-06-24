//------------------------------------------------------------------------------
//  @file uint4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint4.h"
namespace GPULang
{



//------------------------------------------------------------------------------
/**
*/
UInt4::UInt4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::UInt;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(UInt4_ctor, u32x4, u32x4, UInt4);
    __ADD_FUNCTION_PARAM(x, u32);
    __ADD_FUNCTION_PARAM(y, u32);
    __ADD_FUNCTION_PARAM(z, u32);
    __ADD_FUNCTION_PARAM(w, u32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, u32x4, u32x4, val, UInt4);

    UINT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, u32x4, u32x4, UInt4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    UINT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(type##_##ctor, u32x4, u32x4, UInt4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)\
    __ADD_CONSTRUCTOR();

    UINT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(UInt4_orOperator, operator|, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_andOperator, operator&, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_xorOperator, operator^, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_leftShiftOperator, operator<<, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_rightShiftOperator, operator>>, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_additionOperator, operator+, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_subtractionOperator, operator-, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_multiplicationOperator, operator*, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_divisionOperator, operator/, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_modOperator, operator%, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_scaleOperator, operator*, u32x4, u32);

    __IMPLEMENT_FUNCTION_1(UInt4_orAssignOperator, operator|=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_andAssignOperator, operator&=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_xorAssignOperator, operator^=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_leftShiftAssignOperator, operator<<=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_rightShiftAssignOperator, operator>>=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_additionAssignOperator, operator+=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_subtractionAssignOperator, operator-=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_multiplicationAssignOperator, operator*=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_divisionAssignOperator, operator/=, u32x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_moduloAssignOperator, operator%=, u32x4, u32x4);

    __IMPLEMENT_FUNCTION_1(UInt4_ltOperator, operator<, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_lteOperator, operator<=, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_gtOperator, operator>, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_gteOperator, operator>=, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_eOperator, operator==, b8x4, u32x4);
    __IMPLEMENT_FUNCTION_1(UInt4_neOperator, operator!=, b8x4, u32x4);

    __IMPLEMENT_FUNCTION_1(UInt4_elementAccessOperatorInt, operator[], u32, i32);
    __IMPLEMENT_FUNCTION_1(UInt4_elementAccessOperatorUInt, operator[], u32, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(u32, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(u32, 4, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
