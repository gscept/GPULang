//------------------------------------------------------------------------------
//  @file int3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int3.h"
namespace GPULang
{

Function Int3::ctor_XYZ;
Function Int3::ctorSingleValue;

Function Int3::andOperator;
Function Int3::orOperator;
Function Int3::xorOperator;
Function Int3::leftShiftOperator;
Function Int3::rightShiftOperator;
Function Int3::additionOperator;
Function Int3::subtractionOperator;
Function Int3::multiplicationOperator;
Function Int3::divisionOperator;
Function Int3::modOperator;
Function Int3::scaleOperator;

Function Int3::andAssignOperator;
Function Int3::orAssignOperator;
Function Int3::xorAssignOperator;
Function Int3::leftShiftAssignOperator;
Function Int3::rightShiftAssignOperator;
Function Int3::additionAssignOperator;
Function Int3::subtractionAssignOperator;
Function Int3::multiplicationAssignOperator;
Function Int3::divisionAssignOperator;
Function Int3::moduloAssignOperator;

Function Int3::elementAccessOperatorInt;
Function Int3::elementAccessOperatorUInt;

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

    __IMPLEMENT_GLOBAL(ctor_XYZ, int3, int3);
    __ADD_FUNCTION_PARAM(x, int);
    __ADD_FUNCTION_PARAM(y, int);
    __ADD_FUNCTION_PARAM(z, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, int3, int3, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, int3, int3);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, int3, int3);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, int3, int3);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, int3, int3);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, int3, int3);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, int3, int3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, int3, int3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, int3, int3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, int3, int3);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, int3, int3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, int3, int);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, int3, int3);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, int3, int3);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, int3, int3);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, int3, int3);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, int3, int3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, int3, int3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, int3, int3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, int3, int3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, int3, int3);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, int3, int3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], int, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], int, uint);

    char swizzleMask[] = { 'x', 'y', 'z' };
    for (char x = 0; x < 3; x++)
    {
        __ADD_SWIZZLE(int, "%c", swizzleMask[x]);
        for (char y = 0; y < 3; y++)
        {
            __ADD_SWIZZLE(int2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 3; z++)
            {
                __ADD_SWIZZLE(int3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
            }
        }
    }
}

} // namespace GPULang
