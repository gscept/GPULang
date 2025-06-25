//------------------------------------------------------------------------------
//  @file uint2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint2.h"
#include "builtins.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UInt2::UInt2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("u32x2");
    this->baseType = TypeCode::UInt;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(UInt2_ctor, u32x2, UInt2);
    __ADD_FUNCTION_PARAM(x, UInt);
    __ADD_FUNCTION_PARAM(y, UInt);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, u32x2, UInt2, val);

    UINT2_CTOR_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(UInt2_orOperator, operator|, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_andOperator, operator&, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_xorOperator, operator^, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_leftShiftOperator, operator<<, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_rightShiftOperator, operator>>, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_additionOperator, operator+, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_subtractionOperator, operator-, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_multiplicationOperator, operator*, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_divisionOperator, operator/, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_modOperator, operator%, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_scaleOperator, operator*, UInt2, UInt);

    __IMPLEMENT_FUNCTION_1(UInt2_orAssignOperator, operator|=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_andAssignOperator, operator&=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_xorAssignOperator, operator^=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_leftShiftAssignOperator, operator<<=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_rightShiftAssignOperator, operator>>=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_additionAssignOperator, operator+=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_subtractionAssignOperator, operator-=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_multiplicationAssignOperator, operator*=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_divisionAssignOperator, operator/=, UInt2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_moduloAssignOperator, operator%=, UInt2, UInt2);

    __IMPLEMENT_FUNCTION_1(UInt2_ltOperator, operator<, Bool2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_lteOperator, operator<=, Bool2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_gtOperator, operator>, Bool2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_gteOperator, operator>=, Bool2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_eOperator, operator==, Bool2, UInt2);
    __IMPLEMENT_FUNCTION_1(UInt2_neOperator, operator!=, Bool2, UInt2);

    __IMPLEMENT_FUNCTION_1(UInt2_elementAccessOperatorInt, operator[], UInt, Int);
    __IMPLEMENT_FUNCTION_1(UInt2_elementAccessOperatorUInt, operator[], UInt, UInt);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(UInt, 2, swizzleMask);

    char colorMask[] = { 'r', 'g' };
    __IMPLEMENT_SWIZZLE(UInt, 2, colorMask);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
