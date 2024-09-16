//------------------------------------------------------------------------------
//  @file uint4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint4.h"
namespace GPULang
{

Function UInt4::ctor_XYZW;
Function UInt4::ctorSingleValue;
Function UInt4::ctor_3_W;
Function UInt4::ctor_2_ZW;

Function UInt4::andOperator;
Function UInt4::orOperator;
Function UInt4::xorOperator;
Function UInt4::leftShiftOperator;
Function UInt4::rightShiftOperator;
Function UInt4::additionOperator;
Function UInt4::subtractionOperator;
Function UInt4::multiplicationOperator;
Function UInt4::divisionOperator;
Function UInt4::modOperator;
Function UInt4::fractOperator;
Function UInt4::scaleOperator;

Function UInt4::andAssignOperator;
Function UInt4::orAssignOperator;
Function UInt4::xorAssignOperator;
Function UInt4::leftShiftAssignOperator;
Function UInt4::rightShiftAssignOperator;
Function UInt4::additionAssignOperator;
Function UInt4::subtractionAssignOperator;
Function UInt4::multiplicationAssignOperator;
Function UInt4::divisionAssignOperator;
Function UInt4::moduloAssignOperator;

Function UInt4::elementAccessOperatorInt;
Function UInt4::elementAccessOperatorUInt;

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

    __IMPLEMENT_GLOBAL(ctor_XYZW, uint4, uint4);
    __ADD_FUNCTION_PARAM(x, uint);
    __ADD_FUNCTION_PARAM(y, uint);
    __ADD_FUNCTION_PARAM(z, uint);
    __ADD_FUNCTION_PARAM(w, uint);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, uint4, uint4, uint);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_3_W, uint4, uint4);
    __ADD_FUNCTION_PARAM(xyz, uint3);
    __ADD_FUNCTION_PARAM(w, uint);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_2_ZW, uint4, uint4);
    __ADD_FUNCTION_PARAM(xy, uint2);
    __ADD_FUNCTION_PARAM(z, uint);
    __ADD_FUNCTION_PARAM(w, uint);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(fractOperator, fract, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, uint4, int);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, uint4, uint4);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, uint4, uint4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], uint, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], uint, uint);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    for (char x = 0; x < 4; x++)
    {
        __ADD_SWIZZLE(uint, "%c", swizzleMask[x]);
        for (char y = 0; y < 4; y++)
        {
            __ADD_SWIZZLE(uint2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 4; z++)
            {
                __ADD_SWIZZLE(uint3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
                for (char w = 0; w < 4; w++)
                {
                    __ADD_SWIZZLE(uint4, "%c%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z], swizzleMask[w]);
                }
            }
        }
    }
}

} // namespace GPULang
