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

    __IMPLEMENT_CTOR(floatConstructor, f32x3x2, f32x3x2);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m20, f32);
    __ADD_FUNCTION_PARAM(m21, f32);

    __IMPLEMENT_CTOR(vectorConstructor, f32x3x2, f32x3x2);
    __ADD_FUNCTION_PARAM(r0, f32x3);
    __ADD_FUNCTION_PARAM(r1, f32x3);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, f32x3x2, f32x3x2);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, f32x3, f32x2);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, f32x3x2, f32x3x2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32x3, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32x3, u32);

}

} // namespace GPULang
