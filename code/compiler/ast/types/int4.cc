//------------------------------------------------------------------------------
//  @file int4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int4.h"
namespace GPULang
{

Function Int4::ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    Function type::ctor;

    INT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type::ctor;

    INT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    Function type::ctor;

    INT4_CTOR3_LIST
#undef X


Function Int4::andOperator;
Function Int4::orOperator;
Function Int4::xorOperator;
Function Int4::leftShiftOperator;
Function Int4::rightShiftOperator;
Function Int4::additionOperator;
Function Int4::subtractionOperator;
Function Int4::multiplicationOperator;
Function Int4::divisionOperator;
Function Int4::modOperator;
Function Int4::scaleOperator;

Function Int4::andAssignOperator;
Function Int4::orAssignOperator;
Function Int4::xorAssignOperator;
Function Int4::leftShiftAssignOperator;
Function Int4::rightShiftAssignOperator;
Function Int4::additionAssignOperator;
Function Int4::subtractionAssignOperator;
Function Int4::multiplicationAssignOperator;
Function Int4::divisionAssignOperator;
Function Int4::moduloAssignOperator;

Function Int4::ltOperator;
Function Int4::lteOperator;
Function Int4::gtOperator;
Function Int4::gteOperator;
Function Int4::eOperator;
Function Int4::neOperator;

Function Int4::elementAccessOperatorInt;
Function Int4::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Int4::Int4()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Int;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(ctor, i32x4, i32x4);
    __ADD_FUNCTION_PARAM(x, i32);
    __ADD_FUNCTION_PARAM(y, i32);
    __ADD_FUNCTION_PARAM(z, i32);
    __ADD_FUNCTION_PARAM(w, i32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(ctor, i32x4, i32x4, val);

    INT4_CTOR_LIST
#undef X


#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(ctor, i32x4, i32x4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    INT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(ctor, i32x4, i32x4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)\
    __ADD_CONSTRUCTOR();

    INT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, i32x4, i32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, i32x4, i32x4);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8x4, i32x4);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8x4, i32x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], i32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], i32, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(i32, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(i32, 4, colorMask)

}

} // namespace GPULang
