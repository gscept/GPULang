//------------------------------------------------------------------------------
//  @file int4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int4.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Int4::Int4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("i32x4");
    this->baseType = TypeCode::Int;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Int4_ctor, i32x4, Int4);
    __ADD_FUNCTION_PARAM(x, Int);
    __ADD_FUNCTION_PARAM(y, Int);
    __ADD_FUNCTION_PARAM(z, Int);
    __ADD_FUNCTION_PARAM(w, Int);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, i32x4, Int4, val);

    INT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, i32x4, Int4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    INT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(type##_##ctor, i32x4, Int4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)\
    __ADD_CONSTRUCTOR();

    INT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Int4_orOperator, operator|, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_andOperator, operator&, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_xorOperator, operator^, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_leftShiftOperator, operator<<, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_rightShiftOperator, operator>>, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_additionOperator, operator+, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_subtractionOperator, operator-, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_multiplicationOperator, operator*, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_divisionOperator, operator/, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_modOperator, operator%, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_scaleOperator, operator*, Int4, Int);

    __IMPLEMENT_FUNCTION_1(Int4_orAssignOperator, operator|=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_andAssignOperator, operator&=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_xorAssignOperator, operator^=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_leftShiftAssignOperator, operator<<=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_rightShiftAssignOperator, operator>>=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_additionAssignOperator, operator+=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_subtractionAssignOperator, operator-=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_multiplicationAssignOperator, operator*=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_divisionAssignOperator, operator/=, Int4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_moduloAssignOperator, operator%=, Int4, Int4);

    __IMPLEMENT_FUNCTION_1(Int4_ltOperator, operator<, Bool4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_lteOperator, operator<=, Bool4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_gtOperator, operator>, Bool4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_gteOperator, operator>=, Bool4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_eOperator, operator==, Bool4, Int4);
    __IMPLEMENT_FUNCTION_1(Int4_neOperator, operator!=, Bool4, Int4);

    __IMPLEMENT_FUNCTION_1(Int4_elementAccessOperatorInt, operator[], Int, Int);
    __IMPLEMENT_FUNCTION_1(Int4_elementAccessOperatorUInt, operator[], Int, UInt);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(Int, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(Int, 4, colorMask)
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
