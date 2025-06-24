//------------------------------------------------------------------------------
//  @file int2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int2.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Int2::Int2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Int;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Int2_ctor, i32x2, i32x2, Int2);
    __ADD_FUNCTION_PARAM(x, i32);
    __ADD_FUNCTION_PARAM(y, i32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, i32x2, i32x2, val, Int2);

    INT2_CTOR_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Int2_orOperator, operator|, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_andOperator, operator&, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_xorOperator, operator^, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_leftShiftOperator, operator<<, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_rightShiftOperator, operator>>, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_additionOperator, operator+, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_subtractionOperator, operator-, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_multiplicationOperator, operator*, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_divisionOperator, operator/, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_modOperator, operator%, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_scaleOperator, operator*, i32x2, i32);

    __IMPLEMENT_FUNCTION_1(Int2_orAssignOperator, operator|=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_andAssignOperator, operator&=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_xorAssignOperator, operator^=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_leftShiftAssignOperator, operator<<=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_rightShiftAssignOperator, operator>>=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_additionAssignOperator, operator+=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_subtractionAssignOperator, operator-=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_multiplicationAssignOperator, operator*=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_divisionAssignOperator, operator/=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_moduloAssignOperator, operator%=, i32x2, i32x2);

    __IMPLEMENT_FUNCTION_1(Int2_ltOperator, operator<, b8x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_lteOperator, operator<=, b8x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_gtOperator, operator>, b8x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_gteOperator, operator>=, b8x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_eOperator, operator==, b8x2, i32x2);
    __IMPLEMENT_FUNCTION_1(Int2_neOperator, operator!=, b8x2, i32x2);

    __IMPLEMENT_FUNCTION_1(Int2_elementAccessOperatorInt, operator[], i32, i32);
    __IMPLEMENT_FUNCTION_1(Int2_elementAccessOperatorUInt, operator[], i32, u32);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(i32, 2, swizzleMask)
    
    char colorMask[] = { 'r', 'g' };
    __IMPLEMENT_SWIZZLE(i32, 2, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
