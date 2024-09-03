//------------------------------------------------------------------------------
//  @file int2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int2.h"
namespace AnyFX
{

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

    __IMPLEMENT_FUNCTION(ctorXY, int2, int2);
    __ADD_FUNCTION_PARAM(x, int);
    __ADD_FUNCTION_PARAM(y, int);

    __IMPLEMENT_FUNCTION_1(ctorSingleValue, int2, int2, int);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, int2, int2);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, int2, int2);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, int2, int2);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, int2, int2);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, int2, int2);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, int2, int2);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, int2, int2);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, int2, int2);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, int2, int2);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, int2, int2);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, int2, int2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, int2, int2);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, int2, int2);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, int2, int2);
    __IMPLEMENT_FUNCTION_1(moduloOperator, operator%, int2, int2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, int2, int2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, int2, int2);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, int2, int2);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, int2, int2);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, int2, int2);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], int, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], int, uint);

    char swizzleMask[] = { 'x', 'y' };
    for (char x = 0; x < 2; x++)
    {
        __ADD_SWIZZLE(Format("%c", swizzleMask[x]));
        for (char y = 0; y < 2; y++)
        {
            __ADD_SWIZZLE(Format("%c%c", swizzleMask[x], swizzleMask[y]));
        }
    }
}

} // namespace AnyFX
