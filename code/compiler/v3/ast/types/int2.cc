//------------------------------------------------------------------------------
//  @file int2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int2.h"
namespace GPULang
{

Function Int2::ctor_XY;
Function Int2::ctorSingleValue;

Function Int2::andOperator;
Function Int2::orOperator;
Function Int2::xorOperator;
Function Int2::leftShiftOperator;
Function Int2::rightShiftOperator;
Function Int2::additionOperator;
Function Int2::subtractionOperator;
Function Int2::multiplicationOperator;
Function Int2::divisionOperator;
Function Int2::modOperator;
Function Int2::scaleOperator;

Function Int2::andAssignOperator;
Function Int2::orAssignOperator;
Function Int2::xorAssignOperator;
Function Int2::leftShiftAssignOperator;
Function Int2::rightShiftAssignOperator;
Function Int2::additionAssignOperator;
Function Int2::subtractionAssignOperator;
Function Int2::multiplicationAssignOperator;
Function Int2::divisionAssignOperator;
Function Int2::moduloAssignOperator;

Function Int2::elementAccessOperatorInt;
Function Int2::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Int2::Int2()
{
    this->baseType = TypeCode::Int;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(ctor_XY, i32x2, i32x2);
    __ADD_FUNCTION_PARAM(x, i32);
    __ADD_FUNCTION_PARAM(y, i32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, i32x2, i32x2, i32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, i32x2, i32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, i32x2, i32x2);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, i32x2, i32x2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], i32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], i32, u32);

    char swizzleMask[] = { 'x', 'y' };
    for (char x = 0; x < 2; x++)
    {
        __ADD_SWIZZLE(i32, "%c", swizzleMask[x]);
        for (char y = 0; y < 2; y++)
        {
            __ADD_SWIZZLE(i32x2, "%c%c", swizzleMask[x], swizzleMask[y]);
        }
    }
}

} // namespace GPULang
