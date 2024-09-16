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
Function UInt2::fractOperator;
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

    __IMPLEMENT_GLOBAL(ctor_XY, uint2, uint2);
    __ADD_FUNCTION_PARAM(x, uint);
    __ADD_FUNCTION_PARAM(y, uint);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, uint2, uint2, uint);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(fractOperator, fract, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, uint2, int);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, uint2, uint2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], uint, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], uint, uint);

    char swizzleMask[] = { 'x', 'y' };
    for (char x = 0; x < 2; x++)
    {
        __ADD_SWIZZLE(uint, "%c", swizzleMask[x]);
        for (char y = 0; y < 2; y++)
        {
            __ADD_SWIZZLE(uint2, "%c%c", swizzleMask[x], swizzleMask[y]);
        }
    }
}

} // namespace GPULang
