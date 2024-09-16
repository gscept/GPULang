//------------------------------------------------------------------------------
//  @file mat3x2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat3x2.h"
namespace GPULang
{

Function Mat3x2::floatConstructor;
Function Mat3x2::vectorConstructor;

Function Mat3x2::additionOperator;
Function Mat3x2::subtractionOperator;
Function Mat3x2::multiplyOperator;
Function Mat3x2::additionAssignOperator;
Function Mat3x2::subtractionAssignOperator;
Function Mat3x2::multiplyAssignOperator;

Function Mat3x2::vectorTransformOperator;

Function Mat3x2::assignOperator;

Function Mat3x2::elementAccessOperatorInt;
Function Mat3x2::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Mat3x2::Mat3x2()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 2;
    this->byteSize = 24;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(floatConstructor, float3x2, float3x2);
    __ADD_FUNCTION_PARAM(m00, float);
    __ADD_FUNCTION_PARAM(m01, float);
    __ADD_FUNCTION_PARAM(m10, float);
    __ADD_FUNCTION_PARAM(m11, float);
    __ADD_FUNCTION_PARAM(m20, float);
    __ADD_FUNCTION_PARAM(m21, float);

    __IMPLEMENT_GLOBAL(vectorConstructor, float3x2, float3x2);
    __ADD_FUNCTION_PARAM(r0, float2);
    __ADD_FUNCTION_PARAM(r1, float2);
    __ADD_FUNCTION_PARAM(r2, float2);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float3x2, float3x2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float3x2, float3x2);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, float3x2, float3x2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float3x2, float3x2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float3x2, float3x2);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, float3x2, float3x2);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, float2, float2);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, float3x2, float3x2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float2, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float2, uint);

}

} // namespace GPULang
