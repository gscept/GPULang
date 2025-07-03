//------------------------------------------------------------------------------
//  @file int2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int2.h"
#include "builtins.h"

namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Int2::Int2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("i32x2");
    this->baseType = TypeCode::Int;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Int2_ctor, i32x2, Int2);
    __ADD_FUNCTION_PARAM(x, Int);
    __ADD_FUNCTION_PARAM(y, Int);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(type##_##ctor, i32x2, Int2, val);

    INT2_CTOR_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(Int2_orOperator, operator|, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_andOperator, operator&, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_xorOperator, operator^, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_leftShiftOperator, operator<<, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_rightShiftOperator, operator>>, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_additionOperator, operator+, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_subtractionOperator, operator-, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_multiplicationOperator, operator*, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_divisionOperator, operator/, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_modOperator, operator%, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_scaleOperator, operator*, Int2, Int);

    __IMPLEMENT_FUNCTION_1(Int2_orAssignOperator, operator|=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_andAssignOperator, operator&=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_xorAssignOperator, operator^=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_leftShiftAssignOperator, operator<<=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_rightShiftAssignOperator, operator>>=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_additionAssignOperator, operator+=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_subtractionAssignOperator, operator-=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_multiplicationAssignOperator, operator*=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_divisionAssignOperator, operator/=, Int2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_moduloAssignOperator, operator%=, Int2, Int2);

    __IMPLEMENT_FUNCTION_1(Int2_ltOperator, operator<, Bool2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_lteOperator, operator<=, Bool2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_gtOperator, operator>, Bool2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_gteOperator, operator>=, Bool2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_eOperator, operator==, Bool2, Int2);
    __IMPLEMENT_FUNCTION_1(Int2_neOperator, operator!=, Bool2, Int2);

    __IMPLEMENT_FUNCTION_1(Int2_elementAccessOperatorInt, operator[], Int, Int);
    __IMPLEMENT_FUNCTION_1(Int2_elementAccessOperatorUInt, operator[], Int, UInt);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(Int, 2, swizzleMask);
    
    char colorMask[] = { 'r', 'g' };
    __IMPLEMENT_SWIZZLE(Int, 2, colorMask);
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
