//------------------------------------------------------------------------------
//  @file mat4x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat4x4.h"
namespace GPULang
{

Function Mat4x4::floatConstructor;
Function Mat4x4::vectorConstructor;

Function Mat4x4::additionOperator;
Function Mat4x4::subtractionOperator;
Function Mat4x4::multiplyOperator;
Function Mat4x4::additionAssignOperator;
Function Mat4x4::subtractionAssignOperator;
Function Mat4x4::multiplyAssignOperator;

Function Mat4x4::vectorTransformOperator;

Function Mat4x4::assignOperator;

Function Mat4x4::elementAccessOperatorInt;
Function Mat4x4::elementAccessOperatorUInt;


//------------------------------------------------------------------------------
/**
*/
Mat4x4::Mat4x4()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 4;
    this->columnSize = 4;
    this->byteSize = 64;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(floatConstructor, float4x4, float4x4);
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
    __ADD_FUNCTION_PARAM(m30, float);
    __ADD_FUNCTION_PARAM(m31, float);
    __ADD_FUNCTION_PARAM(m32, float);
    __ADD_FUNCTION_PARAM(m33, float);

    __IMPLEMENT_GLOBAL(vectorConstructor, float4x4, float4x4);
    __ADD_FUNCTION_PARAM(r0, float4);
    __ADD_FUNCTION_PARAM(r1, float4);
    __ADD_FUNCTION_PARAM(r2, float4);
    __ADD_FUNCTION_PARAM(r3, float4);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float4x4, float4x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float4x4, float4x4);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, float4x4, float4x4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float4x4, float4x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float4x4, float4x4);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, float4x4, float4x4);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, float4, float4);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, float4x4, float4x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float2, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float2, uint);
}

} // namespace GPULang
