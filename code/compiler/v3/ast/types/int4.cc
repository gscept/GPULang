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

    __IMPLEMENT_GLOBAL(ctor_XYZW, i32x4, i32x4);
    __ADD_FUNCTION_PARAM(x, i32);
    __ADD_FUNCTION_PARAM(y, i32);
    __ADD_FUNCTION_PARAM(z, i32);
    __ADD_FUNCTION_PARAM(w, i32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, i32x4, i32x4, i32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_3_W, i32x4, i32x4);
    __ADD_FUNCTION_PARAM(xyz, i32x3);
    __ADD_FUNCTION_PARAM(w, i32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_2_ZW, i32x4, i32x4);
    __ADD_FUNCTION_PARAM(xy, i32x2);
    __ADD_FUNCTION_PARAM(z, i32);
    __ADD_FUNCTION_PARAM(w, i32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, i32x4, i32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, i32x4, i32x4);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, i32x4, i32x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], i32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], i32, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    for (char x = 0; x < 4; x++)
    {
        __ADD_SWIZZLE(i32, "%c", swizzleMask[x]);
        for (char y = 0; y < 4; y++)
        {
            __ADD_SWIZZLE(i32x2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 4; z++)
            {
                __ADD_SWIZZLE(i32x3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
                for (char w = 0; w < 4; w++)
                {
                    __ADD_SWIZZLE(i32x4, "%c%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z], swizzleMask[w]);
                }
            }
        }
    }
}

} // namespace GPULang
