//------------------------------------------------------------------------------
//  @file mat2x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x4.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat2x4::Mat2x4()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 4;
    this->byteSize = 32;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_FUNCTION(floatConstructor, float2x4, float2x4);
    __ADD_FUNCTION_PARAM(m00, float);
    __ADD_FUNCTION_PARAM(m01, float);
    __ADD_FUNCTION_PARAM(m02, float);
    __ADD_FUNCTION_PARAM(m03, float);
    __ADD_FUNCTION_PARAM(m10, float);
    __ADD_FUNCTION_PARAM(m11, float);
    __ADD_FUNCTION_PARAM(m12, float);
    __ADD_FUNCTION_PARAM(m13, float);

    __IMPLEMENT_FUNCTION(vectorConstructor, float2x4, float2x4);
    __ADD_FUNCTION_PARAM(r0, float4);
    __ADD_FUNCTION_PARAM(r1, float4);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float2x4, float2x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float2x4, float2x4);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, float2x4, float2x4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float2x4, float2x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float2x4, float2x4);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, float2x4, float2x4);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, float4, float4);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, float2x4, float2x4);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], float4, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], float4, uint);
}

} // namespace GPULang
