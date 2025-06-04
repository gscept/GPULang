//------------------------------------------------------------------------------
//  @file uint3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint3.h"
namespace GPULang
{

Function UInt3::ctor;

#define X(type, ctor, val, args, splat, size, conversion)\
    Function type::ctor;

    UINT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type::ctor;

    UINT3_CTOR2_LIST
#undef X

Function UInt3::andOperator;
Function UInt3::orOperator;
Function UInt3::xorOperator;
Function UInt3::leftShiftOperator;
Function UInt3::rightShiftOperator;
Function UInt3::additionOperator;
Function UInt3::subtractionOperator;
Function UInt3::multiplicationOperator;
Function UInt3::divisionOperator;
Function UInt3::modOperator;
Function UInt3::scaleOperator;

Function UInt3::andAssignOperator;
Function UInt3::orAssignOperator;
Function UInt3::xorAssignOperator;
Function UInt3::leftShiftAssignOperator;
Function UInt3::rightShiftAssignOperator;
Function UInt3::additionAssignOperator;
Function UInt3::subtractionAssignOperator;
Function UInt3::multiplicationAssignOperator;
Function UInt3::divisionAssignOperator;
Function UInt3::moduloAssignOperator;

Function UInt3::ltOperator;
Function UInt3::lteOperator;
Function UInt3::gtOperator;
Function UInt3::gteOperator;
Function UInt3::eOperator;
Function UInt3::neOperator;

Function UInt3::elementAccessOperatorInt;
Function UInt3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
UInt3::UInt3()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::UInt;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(ctor, u32x3, u32x3);
    __ADD_FUNCTION_PARAM(x, u32);
    __ADD_FUNCTION_PARAM(y, u32);
    __ADD_FUNCTION_PARAM(z, u32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(ctor, u32x3, u32x3, val);

    UINT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(ctor, u32x3, u32x3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    UINT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, u32x3, u32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, u32x3, u32x3);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, u32x3, u32x3);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8x3, u32x3);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8x3, u32x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], u32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], u32, u32);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(u32, 3, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(u32, 3, colorMask)
}

} // namespace GPULang
