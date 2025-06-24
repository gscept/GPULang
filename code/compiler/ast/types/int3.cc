//------------------------------------------------------------------------------
//  @file int3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int3.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Int3::Int3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Int;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Int3_ctor, i32x3, i32x3, Int3);
    __ADD_FUNCTION_PARAM(x, i32);
    __ADD_FUNCTION_PARAM(y, i32);
    __ADD_FUNCTION_PARAM(z, i32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, i32x3, i32x3, val, Int3);

    INT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, i32x3, i32x3, Int3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    INT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Int3_orOperator, operator|, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_andOperator, operator&, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_xorOperator, operator^, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_leftShiftOperator, operator<<, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_rightShiftOperator, operator>>, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_additionOperator, operator+, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_subtractionOperator, operator-, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_multiplicationOperator, operator*, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_divisionOperator, operator/, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_modOperator, operator%, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_scaleOperator, operator*, i32x3, i32);

    __IMPLEMENT_FUNCTION_1(Int3_orAssignOperator, operator|=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_andAssignOperator, operator&=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_xorAssignOperator, operator^=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_leftShiftAssignOperator, operator<<=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_rightShiftAssignOperator, operator>>=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_additionAssignOperator, operator+=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_subtractionAssignOperator, operator-=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_multiplicationAssignOperator, operator*=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_divisionAssignOperator, operator/=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_moduloAssignOperator, operator%=, i32x3, i32x3);

    __IMPLEMENT_FUNCTION_1(Int3_ltOperator, operator<, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_lteOperator, operator<=, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_gtOperator, operator>, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_gteOperator, operator>=, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_eOperator, operator==, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(Int3_neOperator, operator!=, b8x3, i32x3);

    __IMPLEMENT_FUNCTION_1(Int3_elementAccessOperatorInt, operator[], i32, i32);
    __IMPLEMENT_FUNCTION_1(Int3_elementAccessOperatorUInt, operator[], i32, u32);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(i32, 3, swizzleMask)
    
    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(i32, 3, colorMask)

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
