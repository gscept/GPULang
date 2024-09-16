//------------------------------------------------------------------------------
//  @file float3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float3.h"
namespace GPULang
{

Function Float3::ctor_XYZ;
Function Float3::ctor_Float2_Z;
Function Float3::ctorSingleValue;
Function Float3::additionOperator;
Function Float3::subtractionOperator;
Function Float3::multiplicationOperator;
Function Float3::divisionOperator;
Function Float3::modOperator;
Function Float3::scaleOperator;
Function Float3::matrix34Mul;
Function Float3::matrix33Mul;
Function Float3::matrix32Mul;
Function Float3::additionAssignOperator;
Function Float3::subtractionAssignOperator;
Function Float3::multiplicationAssignOperator;
Function Float3::divisionAssignOperator;
Function Float3::elementAccessOperatorInt;
Function Float3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Float3::Float3()
{
    this->baseType = TypeCode::Float;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(ctor_XYZ, float3, float3);
    __ADD_FUNCTION_PARAM(x, float);
    __ADD_FUNCTION_PARAM(y, float);
    __ADD_FUNCTION_PARAM(z, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL(ctor_Float2_Z, float3, float3);
    __ADD_FUNCTION_PARAM(xy, float2);
    __ADD_FUNCTION_PARAM(z, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_GLOBAL_1(ctorSingleValue, float3, float3, float);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float3, float3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float3, float3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, float3, float3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, float3, float3);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, float3, float3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, float3, float);

    __IMPLEMENT_FUNCTION_1(matrix34Mul, operator*, float3, float3x4);
    __IMPLEMENT_FUNCTION_1(matrix33Mul, operator*, float3, float3x3);
    __IMPLEMENT_FUNCTION_1(matrix32Mul, operator*, float3, float3x2);

    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float3, float3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float3, float3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, float3, float3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, float3, float3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float, uint);

    char swizzleMask[] = { 'x', 'y', 'z' };
    for (char x = 0; x < 3; x++)
    {
        __ADD_SWIZZLE(float, "%c", swizzleMask[x]);
        for (char y = 0; y < 3; y++)
        {
            __ADD_SWIZZLE(float2, "%c%c", swizzleMask[x], swizzleMask[y]);
            for (char z = 0; z < 3; z++)
            {
                __ADD_SWIZZLE(float3, "%c%c%c", swizzleMask[x], swizzleMask[y], swizzleMask[z]);
            }
        }
    }
}

} // namespace GPULang
