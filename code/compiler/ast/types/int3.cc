//------------------------------------------------------------------------------
//  @file int3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int3.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Int3::Int3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("i32x3");
    this->baseType = TypeCode::Int;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Int3_ctor, i32x3, Int3);
    __ADD_FUNCTION_PARAM(x, Int);
    __ADD_FUNCTION_PARAM(y, Int);
    __ADD_FUNCTION_PARAM(z, Int);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, i32x3, Int3, val);

    INT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(type##_##ctor, i32x3, Int3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    INT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Int3_orOperator, operator|, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_andOperator, operator&, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_xorOperator, operator^, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_leftShiftOperator, operator<<, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_rightShiftOperator, operator>>, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_additionOperator, operator+, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_subtractionOperator, operator-, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_multiplicationOperator, operator*, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_divisionOperator, operator/, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_modOperator, operator%, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_scaleOperator, operator*, Int3, Int);

    __IMPLEMENT_FUNCTION_1(Int3_orAssignOperator, operator|=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_andAssignOperator, operator&=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_xorAssignOperator, operator^=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_leftShiftAssignOperator, operator<<=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_rightShiftAssignOperator, operator>>=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_additionAssignOperator, operator+=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_subtractionAssignOperator, operator-=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_multiplicationAssignOperator, operator*=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_divisionAssignOperator, operator/=, Int3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_moduloAssignOperator, operator%=, Int3, Int3);

    __IMPLEMENT_FUNCTION_1(Int3_ltOperator, operator<, Bool3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_lteOperator, operator<=, Bool3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_gtOperator, operator>, Bool3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_gteOperator, operator>=, Bool3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_eOperator, operator==, Bool3, Int3);
    __IMPLEMENT_FUNCTION_1(Int3_neOperator, operator!=, Bool3, Int3);

    __IMPLEMENT_FUNCTION_1(Int3_elementAccessOperatorInt, operator[], Int, Int);
    __IMPLEMENT_FUNCTION_1(Int3_elementAccessOperatorUInt, operator[], Int, UInt);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(Int, 3, swizzleMask);
    
    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(Int, 3, colorMask);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
