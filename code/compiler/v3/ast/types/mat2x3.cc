//------------------------------------------------------------------------------
//  @file mat2x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x3.h"
namespace GPULang
{

Function Mat2x3::floatConstructor;
Function Mat2x3::vectorConstructor;

Function Mat2x3::additionOperator;
Function Mat2x3::subtractionOperator;
Function Mat2x3::multiplyOperator;
Function Mat2x3::additionAssignOperator;
Function Mat2x3::subtractionAssignOperator;
Function Mat2x3::multiplyAssignOperator;

Function Mat2x3::vectorTransformOperator;

Function Mat2x3::assignOperator;

Function Mat2x3::elementAccessOperatorInt;
Function Mat2x3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Mat2x3::Mat2x3()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 3;
    this->byteSize = 24;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL(floatConstructor, float2x3, float2x3);
    __ADD_FUNCTION_PARAM(m00, float);
    __ADD_FUNCTION_PARAM(m01, float);
    __ADD_FUNCTION_PARAM(m02, float);
    __ADD_FUNCTION_PARAM(m10, float);
    __ADD_FUNCTION_PARAM(m11, float);
    __ADD_FUNCTION_PARAM(m12, float);

    __IMPLEMENT_GLOBAL(vectorConstructor, float2x3, float2x3);
    __ADD_FUNCTION_PARAM(r0, float3);
    __ADD_FUNCTION_PARAM(r1, float3);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float2x3, float2x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float2x3, float2x3);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, float2x3, float2x3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float2x3, float2x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float2x3, float2x3);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, float2x3, float2x3);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, float3, float3);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, float2x3, float2x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], float3, int);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], float3, uint);
}

} // namespace GPULang
