//------------------------------------------------------------------------------
//  @file mat2x2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x2.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat2x2::Mat2x2()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 2;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_FUNCTION(floatConstructor, float2x2, float2x2);
    __ADD_FUNCTION_PARAM(m00, float);
    __ADD_FUNCTION_PARAM(m01, float);
    __ADD_FUNCTION_PARAM(m10, float);
    __ADD_FUNCTION_PARAM(m11, float);

    __IMPLEMENT_FUNCTION(vectorConstructor, float2x2, float2x2);
    __ADD_FUNCTION_PARAM(r0, float2);
    __ADD_FUNCTION_PARAM(r1, float2);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float2x2, float2x2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float2x2, float2x2);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, float2x2, float2x2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float2x2, float2x2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float2x2, float2x2);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, float2x2, float2x2);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, float2, float2);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, float2x2, float2x2);

    __IMPLEMENT_FUNCTION_1(bracketOperatorInt, operator[], float2, int);
    __IMPLEMENT_FUNCTION_1(bracketOperatorUInt, operator[], float2, uint);
}

} // namespace GPULang
