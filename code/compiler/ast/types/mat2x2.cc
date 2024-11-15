//------------------------------------------------------------------------------
//  @file mat2x2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x2.h"
namespace GPULang
{

Function Mat2x2::floatConstructor;
Function Mat2x2::vectorConstructor;

Function Mat2x2::additionOperator;
Function Mat2x2::subtractionOperator;
Function Mat2x2::multiplyOperator;
Function Mat2x2::additionAssignOperator;
Function Mat2x2::subtractionAssignOperator;
Function Mat2x2::multiplyAssignOperator;

Function Mat2x2::vectorTransformOperator;
Function Mat2x2::scaleOperator;

Function Mat2x2::assignOperator;

Function Mat2x2::elementAccessOperatorInt;
Function Mat2x2::elementAccessOperatorUInt;
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

    __IMPLEMENT_CTOR(floatConstructor, f32x2x2, f32x2x2);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);

    __IMPLEMENT_CTOR(vectorConstructor, f32x2x2, f32x2x2);
    __ADD_FUNCTION_PARAM(r0, f32x2);
    __ADD_FUNCTION_PARAM(r1, f32x2);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, f32x2x2, f32x2x2);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x2x2, f32);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, f32x2x2, f32x2x2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32x2, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32x2, u32);
}

} // namespace GPULang
