//------------------------------------------------------------------------------
//  @file uint4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint4.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
UInt4::UInt4()
{
    this->baseType = TypeCode::UInt;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_FUNCTION(ctorXYZW, uint4, uint4);
    __ADD_FUNCTION_PARAM(x, uint);
    __ADD_FUNCTION_PARAM(y, uint);
    __ADD_FUNCTION_PARAM(z, uint);
    __ADD_FUNCTION_PARAM(w, uint);

    __IMPLEMENT_FUNCTION_1(ctorSingleValue, uint4, uint4, uint);

    __IMPLEMENT_FUNCTION(ctorUInt3W, uint4, uint4);
    __ADD_FUNCTION_PARAM(xyz, uint3);
    __ADD_FUNCTION_PARAM(w, uint);

    __IMPLEMENT_FUNCTION(ctorUInt2ZW, uint4, uint4);
    __ADD_FUNCTION_PARAM(xy, uint2);
    __ADD_FUNCTION_PARAM(z, uint);
    __ADD_FUNCTION_PARAM(w, uint);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(moduloOperator, operator%, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, uint4, uint4);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], uint, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], uint, uint);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    for (char x = 0; x < 4; x++)
    {
        __ADD_SWIZZLE(Format("%c", swizzleMask[x]));
        for (char y = 0; y < 4; y++)
        {
            __ADD_SWIZZLE(Format("%c%c", swizzleMask[x], swizzleMask[y]));
            for (char z = 0; z < 4; z++)
            {
                __ADD_SWIZZLE(Format("%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]));
                for (char w = 0; w < 4; w++)
                {
                    __ADD_SWIZZLE(Format("%c%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z], swizzleMask[w]));
                }
            }
        }
    }
}

} // namespace GPULang
