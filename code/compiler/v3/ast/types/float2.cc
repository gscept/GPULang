//------------------------------------------------------------------------------
//  @file float2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float2.h"
namespace GPULang
{

Function Float2::ctor_XY;
Function Float2::ctorSingleValue;
Function Float2::additionOperator;
Function Float2::subtractionOperator;
Function Float2::multiplicationOperator;
Function Float2::divisionOperator;
Function Float2::modOperator;
Function Float2::scaleOperator;
Function Float2::matrix24Mul;
Function Float2::matrix23Mul;
Function Float2::matrix22Mul;
Function Float2::additionAssignOperator;
Function Float2::subtractionAssignOperator;
Function Float2::multiplicationAssignOperator;
Function Float2::divisionAssignOperator;
Function Float2::elementAccessOperatorInt;
Function Float2::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Float2::Float2()
{
    this->baseType = TypeCode::Float;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(ctor_XY, float2, float2);
    __ADD_FUNCTION_PARAM(x, float);
    __ADD_FUNCTION_PARAM(y, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, float2, float2, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float2, float2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float2, float2);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, float2, float2);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, float2, float2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float2, float2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float2, float2);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, float2, float2);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, float2, float2);

    __IMPLEMENT_FUNCTION_1(matrix24Mul, operator*, float3, float2x4);
    __IMPLEMENT_FUNCTION_1(matrix23Mul, operator*, float3, float2x3);
    __IMPLEMENT_FUNCTION_1(matrix22Mul, operator*, float3, float2x2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float, uint);

    char swizzleMask[] = { 'x', 'y' };
    for (char x = 0; x < 2; x++)
    {
        __ADD_SWIZZLE(float, "%c", swizzleMask[x]);
        for (char y = 0; y < 2; y++)
        {
            __ADD_SWIZZLE(float2, "%c%c", swizzleMask[x], swizzleMask[y]);
        }
    }
}

} // namespace GPULang
