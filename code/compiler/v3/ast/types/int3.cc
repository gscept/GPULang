//------------------------------------------------------------------------------
//  @file int3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int3.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Int3::Int3()
{
    this->baseType = TypeCode::Int;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_FUNCTION(ctorXYZ, int3, int3);
    __ADD_FUNCTION_PARAM(x, int);
    __ADD_FUNCTION_PARAM(y, int);
    __ADD_FUNCTION_PARAM(z, int);

    __IMPLEMENT_FUNCTION_1(ctorSingleValue, int3, int3, int);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, int3, int3);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, int3, int3);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, int3, int3);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, int3, int3);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, int3, int3);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, int3, int3);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, int3, int3);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, int3, int3);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, int3, int3);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, int3, int3);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, int3, int3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, int3, int3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, int3, int3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, int3, int3);
    __IMPLEMENT_FUNCTION_1(moduloOperator, operator%, int3, int3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, int3, int3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, int3, int3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, int3, int3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, int3, int3);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, int3, int3);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], int, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], int, uint);

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

} // namespace GPULang
