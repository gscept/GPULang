//------------------------------------------------------------------------------
//  @file uint3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint3.h"
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
UInt3::UInt3()
{
    this->baseType = TypeCode::UInt;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_FUNCTION(ctorXYZ, uint3, uint3);
    __ADD_FUNCTION_PARAM(x, uint);
    __ADD_FUNCTION_PARAM(y, uint);
    __ADD_FUNCTION_PARAM(z, uint);

    __IMPLEMENT_FUNCTION_1(ctorSingleValue, uint3, uint3, uint);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(moduloOperator, operator%, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, uint3, uint3);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], uint, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], uint, uint);

    char swizzleMask[] = { 'x', 'y', 'z' };
    for (char x = 0; x < 3; x++)
    {
        __ADD_SWIZZLE(Format("%c", swizzleMask[x]));
        for (char y = 0; y < 3; y++)
        {
            __ADD_SWIZZLE(Format("%c%c", swizzleMask[x], swizzleMask[y]));
            for (char z = 0; z < 3; z++)
            {
                __ADD_SWIZZLE(Format("%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]));
            }
        }
    }
}

} // namespace AnyFX
