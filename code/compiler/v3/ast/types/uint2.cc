//------------------------------------------------------------------------------
//  @file uint2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint2.h"
namespace GPULang
{

Function UInt2::ctor_XY;
Function UInt2::ctorSingleValue;

Function UInt2::andOperator;
Function UInt2::orOperator;
Function UInt2::xorOperator;
Function UInt2::leftShiftOperator;
Function UInt2::rightShiftOperator;
Function UInt2::additionOperator;
Function UInt2::subtractionOperator;
Function UInt2::multiplicationOperator;
Function UInt2::divisionOperator;
Function UInt2::modOperator;
Function UInt2::scaleOperator;

Function UInt2::andAssignOperator;
Function UInt2::orAssignOperator;
Function UInt2::xorAssignOperator;
Function UInt2::leftShiftAssignOperator;
Function UInt2::rightShiftAssignOperator;
Function UInt2::additionAssignOperator;
Function UInt2::subtractionAssignOperator;
Function UInt2::multiplicationAssignOperator;
Function UInt2::divisionAssignOperator;
Function UInt2::moduloAssignOperator;

Function UInt2::elementAccessOperatorInt;
Function UInt2::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
UInt2::UInt2()
{
    this->baseType = TypeCode::UInt;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(ctor_XY, u32x2, u32x2);
    __ADD_FUNCTION_PARAM(x, u32);
    __ADD_FUNCTION_PARAM(y, u32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, u32x2, u32x2, u32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, u32x2, u32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, u32x2, u32x2);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, u32x2, u32x2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], u32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], u32, u32);

    char swizzleMask[] = { 'x', 'y' };
    for (char x = 0; x < 2; x++)
    {
        __ADD_SWIZZLE(u32, "%c", swizzleMask[x]);
        for (char y = 0; y < 2; y++)
        {
            __ADD_SWIZZLE(u32x2, "%c%c", swizzleMask[x], swizzleMask[y]);
        }
    }
}

} // namespace GPULang
