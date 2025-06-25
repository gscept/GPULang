//------------------------------------------------------------------------------
//  @file uint4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint4.h"
#include "builtins.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UInt4::UInt4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("u32x4");
    this->baseType = TypeCode::UInt;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(UInt4_ctor, u32x4, UInt4);
    __ADD_FUNCTION_PARAM(x, UInt);
    __ADD_FUNCTION_PARAM(y, UInt);
    __ADD_FUNCTION_PARAM(z, UInt);
    __ADD_FUNCTION_PARAM(w, UInt);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, u32x4, UInt4, val);

    UINT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, u32x4, UInt4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    UINT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(type##_##ctor, u32x4, UInt4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)\
    __ADD_CONSTRUCTOR();

    UINT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(UInt4_orOperator, operator|, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_andOperator, operator&, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_xorOperator, operator^, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_leftShiftOperator, operator<<, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_rightShiftOperator, operator>>, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_additionOperator, operator+, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_subtractionOperator, operator-, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_multiplicationOperator, operator*, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_divisionOperator, operator/, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_modOperator, operator%, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_scaleOperator, operator*, UInt4, UInt);

    __IMPLEMENT_FUNCTION_1(UInt4_orAssignOperator, operator|=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_andAssignOperator, operator&=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_xorAssignOperator, operator^=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_leftShiftAssignOperator, operator<<=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_rightShiftAssignOperator, operator>>=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_additionAssignOperator, operator+=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_subtractionAssignOperator, operator-=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_multiplicationAssignOperator, operator*=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_divisionAssignOperator, operator/=, UInt4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_moduloAssignOperator, operator%=, UInt4, UInt4);

    __IMPLEMENT_FUNCTION_1(UInt4_ltOperator, operator<, Bool4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_lteOperator, operator<=, Bool4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_gtOperator, operator>, Bool4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_gteOperator, operator>=, Bool4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_eOperator, operator==, Bool4, UInt4);
    __IMPLEMENT_FUNCTION_1(UInt4_neOperator, operator!=, Bool4, UInt4);

    __IMPLEMENT_FUNCTION_1(UInt4_elementAccessOperatorInt, operator[], UInt, Int);
    __IMPLEMENT_FUNCTION_1(UInt4_elementAccessOperatorUInt, operator[], UInt, UInt);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(UInt, 4, swizzleMask);

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(UInt, 4, colorMask);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
