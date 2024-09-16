//------------------------------------------------------------------------------
//  @file mat4x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat4x3.h"
namespace GPULang
{

Function Mat4x3::floatConstructor;
Function Mat4x3::vectorConstructor;

Function Mat4x3::additionOperator;
Function Mat4x3::subtractionOperator;
Function Mat4x3::multiplyOperator;
Function Mat4x3::additionAssignOperator;
Function Mat4x3::subtractionAssignOperator;
Function Mat4x3::multiplyAssignOperator;

Function Mat4x3::vectorTransformOperator;

Function Mat4x3::assignOperator;

Function Mat4x3::elementAccessOperatorInt;
Function Mat4x3::elementAccessOperatorUInt;


//------------------------------------------------------------------------------
/**
*/
Mat4x3::Mat4x3()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 4;
    this->columnSize = 3;
    this->byteSize = 48;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(floatConstructor, float4x3, float4x3);
    __ADD_FUNCTION_PARAM(m00, float);
    __ADD_FUNCTION_PARAM(m01, float);
    __ADD_FUNCTION_PARAM(m02, float);
    __ADD_FUNCTION_PARAM(m10, float);
    __ADD_FUNCTION_PARAM(m11, float);
    __ADD_FUNCTION_PARAM(m12, float);
    __ADD_FUNCTION_PARAM(m20, float);
    __ADD_FUNCTION_PARAM(m21, float);
    __ADD_FUNCTION_PARAM(m22, float);
    __ADD_FUNCTION_PARAM(m30, float);
    __ADD_FUNCTION_PARAM(m31, float);
    __ADD_FUNCTION_PARAM(m32, float);

    __IMPLEMENT_GLOBAL(vectorConstructor, float4x3, float4x3);
    __ADD_FUNCTION_PARAM(r0, float3);
    __ADD_FUNCTION_PARAM(r1, float3);
    __ADD_FUNCTION_PARAM(r2, float3);
    __ADD_FUNCTION_PARAM(r3, float3);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float4x3, float4x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float4x3, float4x3);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, float4x3, float4x3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float4x3, float4x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float4x3, float4x3);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, float4x3, float4x3);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, float3, float3);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, float4x3, float4x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float2, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float2, uint);
}

} // namespace GPULang
