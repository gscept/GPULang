//------------------------------------------------------------------------------
//  @file uint2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint2.h"
namespace AnyFX
{

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

    __IMPLEMENT_FUNCTION(ctorXY, uint2, uint2);
    __ADD_FUNCTION_PARAM(x, uint);
    __ADD_FUNCTION_PARAM(y, uint);

    __IMPLEMENT_FUNCTION_1(ctorSingleValue, uint2, uint2, uint);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(moduloOperator, operator%, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, uint2, uint2);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, uint2, uint2);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], uint, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], uint, uint);

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
