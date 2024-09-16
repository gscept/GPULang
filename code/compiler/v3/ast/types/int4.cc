//------------------------------------------------------------------------------
//  @file int4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int4.h"
namespace GPULang
{

Function Int4::ctor_XYZW;
Function Int4::ctorSingleValue;
Function Int4::ctor_3_W;
Function Int4::ctor_2_ZW;

Function Int4::andOperator;
Function Int4::orOperator;
Function Int4::xorOperator;
Function Int4::leftShiftOperator;
Function Int4::rightShiftOperator;
Function Int4::additionOperator;
Function Int4::subtractionOperator;
Function Int4::multiplicationOperator;
Function Int4::divisionOperator;
Function Int4::modOperator;
Function Int4::scaleOperator;

Function Int4::andAssignOperator;
Function Int4::orAssignOperator;
Function Int4::xorAssignOperator;
Function Int4::leftShiftAssignOperator;
Function Int4::rightShiftAssignOperator;
Function Int4::additionAssignOperator;
Function Int4::subtractionAssignOperator;
Function Int4::multiplicationAssignOperator;
Function Int4::divisionAssignOperator;
Function Int4::moduloAssignOperator;

Function Int4::elementAccessOperatorInt;
Function Int4::elementAccessOperatorUInt;

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

    __IMPLEMENT_GLOBAL(ctor_XYZW, int4, int4);
    __ADD_FUNCTION_PARAM(x, int);
    __ADD_FUNCTION_PARAM(y, int);
    __ADD_FUNCTION_PARAM(z, int);
    __ADD_FUNCTION_PARAM(w, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, int4, int4, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_3_W, int4, int4);
    __ADD_FUNCTION_PARAM(xyz, int3);
    __ADD_FUNCTION_PARAM(w, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_2_ZW, int4, int4);
    __ADD_FUNCTION_PARAM(xy, int2);
    __ADD_FUNCTION_PARAM(z, int);
    __ADD_FUNCTION_PARAM(w, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, int4, int4);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, int4, int4);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, int4, int4);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, int4, int4);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, int4, int4);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, int4, int4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, int4, int4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, int4, int4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, int4, int4);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, int4, int4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, int4, int);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, int4, int4);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, int4, int4);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, int4, int4);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, int4, int4);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, int4, int4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, int4, int4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, int4, int4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, int4, int4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, int4, int4);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, int4, int4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], int, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], int, uint);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    for (char x = 0; x < 4; x++)
    {
        __ADD_SWIZZLE(int, "%c", swizzleMask[x]);
        for (char y = 0; y < 4; y++)
        {
            __ADD_SWIZZLE(int2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 4; z++)
            {
                __ADD_SWIZZLE(int3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
                for (char w = 0; w < 4; w++)
                {
                    __ADD_SWIZZLE(int4, "%c%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z], swizzleMask[w]);
                }
            }
        }
    }
}

} // namespace GPULang
