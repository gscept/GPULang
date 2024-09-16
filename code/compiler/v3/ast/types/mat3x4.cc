//------------------------------------------------------------------------------
//  @file mat3x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat3x4.h"
namespace GPULang
{

Function Mat3x4::floatConstructor;
Function Mat3x4::vectorConstructor;

Function Mat3x4::additionOperator;
Function Mat3x4::subtractionOperator;
Function Mat3x4::multiplyOperator;
Function Mat3x4::additionAssignOperator;
Function Mat3x4::subtractionAssignOperator;
Function Mat3x4::multiplyAssignOperator;

Function Mat3x4::vectorTransformOperator;

Function Mat3x4::assignOperator;

Function Mat3x4::elementAccessOperatorInt;
Function Mat3x4::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Mat3x4::Mat3x4()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 4;
    this->byteSize = 48;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(floatConstructor, float3x4, float3x4);
    __ADD_FUNCTION_PARAM(m00, float);
    __ADD_FUNCTION_PARAM(m01, float);
    __ADD_FUNCTION_PARAM(m02, float);
    __ADD_FUNCTION_PARAM(m03, float);
    __ADD_FUNCTION_PARAM(m10, float);
    __ADD_FUNCTION_PARAM(m11, float);
    __ADD_FUNCTION_PARAM(m12, float);
    __ADD_FUNCTION_PARAM(m13, float);
    __ADD_FUNCTION_PARAM(m20, float);
    __ADD_FUNCTION_PARAM(m21, float);
    __ADD_FUNCTION_PARAM(m22, float);
    __ADD_FUNCTION_PARAM(m23, float);

    __IMPLEMENT_GLOBAL(vectorConstructor, float3x4, float3x4);
    __ADD_FUNCTION_PARAM(r0, float4);
    __ADD_FUNCTION_PARAM(r1, float4);
    __ADD_FUNCTION_PARAM(r2, float4);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float3x4, float3x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float3x4, float3x4);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, float3x4, float3x4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float3x4, float3x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float3x4, float3x4);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, float3x4, float3x4);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, float4, float4);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, float3x4, float3x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float2, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float2, uint);
}

} // namespace GPULang
