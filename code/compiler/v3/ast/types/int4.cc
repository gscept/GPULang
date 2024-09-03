//------------------------------------------------------------------------------
//  @file int4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int4.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Int4::Int4()
{
    this->baseType = TypeCode::Int;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_FUNCTION(ctorXYZW, int4, int4);
    __ADD_FUNCTION_PARAM(x, int);
    __ADD_FUNCTION_PARAM(y, int);
    __ADD_FUNCTION_PARAM(z, int);
    __ADD_FUNCTION_PARAM(w, int);

    __IMPLEMENT_FUNCTION_1(ctorSingleValue, int4, int4, int);

    __IMPLEMENT_FUNCTION(ctorInt3W, int4, int4);
    __ADD_FUNCTION_PARAM(xyz, int3);
    __ADD_FUNCTION_PARAM(w, int);

    __IMPLEMENT_FUNCTION(ctorInt2ZW, int4, int4);
    __ADD_FUNCTION_PARAM(xy, int2);
    __ADD_FUNCTION_PARAM(z, int);
    __ADD_FUNCTION_PARAM(w, int);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, int4, int4);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, int4, int4);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, int4, int4);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, int4, int4);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, int4, int4);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, int4, int4);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, int4, int4);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, int4, int4);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, int4, int4);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, int4, int4);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, int4, int4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, int4, int4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, int4, int4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, int4, int4);
    __IMPLEMENT_FUNCTION_1(moduloOperator, operator%, int4, int4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, int4, int4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, int4, int4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, int4, int4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, int4, int4);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, int4, int4);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], int, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], int, uint);

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
