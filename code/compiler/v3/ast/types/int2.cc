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

    __IMPLEMENT_GLOBAL(ctor_XY, int2, int2);
    __ADD_FUNCTION_PARAM(x, int);
    __ADD_FUNCTION_PARAM(y, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, int2, int2, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, int2, int2);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, int2, int2);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, int2, int2);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, int2, int2);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, int2, int2);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, int2, int2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, int2, int2);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, int2, int2);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, int2, int2);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, int2, int2);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, int2, int);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, int2, int2);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, int2, int2);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, int2, int2);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, int2, int2);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, int2, int2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, int2, int2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, int2, int2);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, int2, int2);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, int2, int2);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, int2, int2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], int, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], int, uint);

    char swizzleMask[] = { 'x', 'y' };
    for (char x = 0; x < 2; x++)
    {
        __ADD_SWIZZLE(int, "%c", swizzleMask[x]);
        for (char y = 0; y < 2; y++)
        {
            __ADD_SWIZZLE(int2, "%c%c", swizzleMask[x], swizzleMask[y]);
        }
    }
}

} // namespace GPULang
