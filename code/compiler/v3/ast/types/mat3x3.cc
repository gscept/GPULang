//------------------------------------------------------------------------------
//  @file mat3x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat3x3.h"
namespace GPULang
{

Function Mat3x3::floatConstructor;
Function Mat3x3::vectorConstructor;

Function Mat3x3::additionOperator;
Function Mat3x3::subtractionOperator;
Function Mat3x3::multiplyOperator;
Function Mat3x3::additionAssignOperator;
Function Mat3x3::subtractionAssignOperator;
Function Mat3x3::multiplyAssignOperator;

Function Mat3x3::vectorTransformOperator;

Function Mat3x3::assignOperator;

Function Mat3x3::elementAccessOperatorInt;
Function Mat3x3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Mat3x3::Mat3x3()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 3;
    this->byteSize = 36;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(floatConstructor, float3x3, float3x3);
    __ADD_FUNCTION_PARAM(m00, float);
    __ADD_FUNCTION_PARAM(m01, float);
    __ADD_FUNCTION_PARAM(m02, float);
    __ADD_FUNCTION_PARAM(m10, float);
    __ADD_FUNCTION_PARAM(m11, float);
    __ADD_FUNCTION_PARAM(m12, float);
    __ADD_FUNCTION_PARAM(m20, float);
    __ADD_FUNCTION_PARAM(m21, float);
    __ADD_FUNCTION_PARAM(m22, float);

    __IMPLEMENT_GLOBAL(vectorConstructor, float3x3, float3x3);
    __ADD_FUNCTION_PARAM(r0, float3);
    __ADD_FUNCTION_PARAM(r1, float3);
    __ADD_FUNCTION_PARAM(r2, float3);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float3x3, float3x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float3x3, float3x3);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, float3x3, float3x3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float3x3, float3x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float3x3, float3x3);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, float3x3, float3x3);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, float3, float3);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, float3x3, float3x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float3, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float3, uint);
}

} // namespace GPULang
