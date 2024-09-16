//------------------------------------------------------------------------------
//  @file uint3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint3.h"
namespace GPULang
{

Function UInt3::ctor_XYZ;
Function UInt3::ctorSingleValue;

Function UInt3::andOperator;
Function UInt3::orOperator;
Function UInt3::xorOperator;
Function UInt3::leftShiftOperator;
Function UInt3::rightShiftOperator;
Function UInt3::additionOperator;
Function UInt3::subtractionOperator;
Function UInt3::multiplicationOperator;
Function UInt3::divisionOperator;
Function UInt3::modOperator;
Function UInt3::fractOperator;
Function UInt3::scaleOperator;

Function UInt3::andAssignOperator;
Function UInt3::orAssignOperator;
Function UInt3::xorAssignOperator;
Function UInt3::leftShiftAssignOperator;
Function UInt3::rightShiftAssignOperator;
Function UInt3::additionAssignOperator;
Function UInt3::subtractionAssignOperator;
Function UInt3::multiplicationAssignOperator;
Function UInt3::divisionAssignOperator;
Function UInt3::moduloAssignOperator;

Function UInt3::elementAccessOperatorInt;
Function UInt3::elementAccessOperatorUInt;

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

    __IMPLEMENT_GLOBAL(ctor_XYZ, uint3, uint3);
    __ADD_FUNCTION_PARAM(x, uint);
    __ADD_FUNCTION_PARAM(y, uint);
    __ADD_FUNCTION_PARAM(z, uint);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, uint3, uint3, uint);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(fractOperator, fract, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, uint3, int);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, uint3, uint3);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, uint3, uint3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], uint, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], uint, uint);

    char swizzleMask[] = { 'x', 'y', 'z' };
    for (char x = 0; x < 3; x++)
    {
        __ADD_SWIZZLE(uint, "%c", swizzleMask[x]);
        for (char y = 0; y < 3; y++)
        {
            __ADD_SWIZZLE(uint2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 3; z++)
            {
                __ADD_SWIZZLE(uint3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
            }
        }
    }
}

} // namespace GPULang
