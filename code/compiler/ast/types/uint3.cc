//------------------------------------------------------------------------------
//  @file uint3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint3.h"
#include "builtins.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UInt3::UInt3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("u32x3");
    this->baseType = TypeCode::UInt;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(UInt3_ctor, u32x3, UInt3);
    __ADD_FUNCTION_PARAM(x, UInt);
    __ADD_FUNCTION_PARAM(y, UInt);
    __ADD_FUNCTION_PARAM(z, UInt);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, u32x3, UInt3, val);

    UINT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, u32x3, UInt3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    UINT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(UInt3_orOperator, operator|, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_andOperator, operator&, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_xorOperator, operator^, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_leftShiftOperator, operator<<, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_rightShiftOperator, operator>>, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_additionOperator, operator+, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_subtractionOperator, operator-, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_multiplicationOperator, operator*, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_divisionOperator, operator/, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_modOperator, operator%, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_scaleOperator, operator*, UInt3, UInt);

    __IMPLEMENT_FUNCTION_1(UInt3_orAssignOperator, operator|=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_andAssignOperator, operator&=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_xorAssignOperator, operator^=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_leftShiftAssignOperator, operator<<=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_rightShiftAssignOperator, operator>>=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_additionAssignOperator, operator+=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_subtractionAssignOperator, operator-=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_multiplicationAssignOperator, operator*=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_divisionAssignOperator, operator/=, UInt3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_moduloAssignOperator, operator%=, UInt3, UInt3);

    __IMPLEMENT_FUNCTION_1(UInt3_ltOperator, operator<, Bool3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_lteOperator, operator<=, Bool3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_gtOperator, operator>, Bool3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_gteOperator, operator>=, Bool3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_eOperator, operator==, Bool3, UInt3);
    __IMPLEMENT_FUNCTION_1(UInt3_neOperator, operator!=, Bool3, UInt3);

    __IMPLEMENT_FUNCTION_1(UInt3_elementAccessOperatorInt, operator[], UInt, Int);
    __IMPLEMENT_FUNCTION_1(UInt3_elementAccessOperatorUInt, operator[], UInt, UInt);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(UInt, 3, swizzleMask);

    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(UInt, 3, colorMask);
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
