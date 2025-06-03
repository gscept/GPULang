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
Function Mat3x4::scaleOperator;

Function Mat3x4::assignOperator;

Function Mat3x4::elementAccessOperatorInt;
Function Mat3x4::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Mat3x4::Mat3x4()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 4;
    this->byteSize = 48;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(floatConstructor, f32x3x4, f32x3x4);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m02, f32);
    __ADD_FUNCTION_PARAM(m03, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m12, f32);
    __ADD_FUNCTION_PARAM(m13, f32);
    __ADD_FUNCTION_PARAM(m20, f32);
    __ADD_FUNCTION_PARAM(m21, f32);
    __ADD_FUNCTION_PARAM(m22, f32);
    __ADD_FUNCTION_PARAM(m23, f32);

    __IMPLEMENT_CTOR(vectorConstructor, f32x3x4, f32x3x4);
    __ADD_FUNCTION_PARAM(r0, f32x3);
    __ADD_FUNCTION_PARAM(r1, f32x3);
    __ADD_FUNCTION_PARAM(r2, f32x3);
    __ADD_FUNCTION_PARAM(r3, f32x3);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x3x4, f32x3x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x3x4, f32x3x4);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, f32x3x4, f32x3x4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x3x4, f32x3x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x3x4, f32x3x4);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, f32x3x4, f32x3x4);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, f32x3, f32x4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x3x4, f32);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, f32x3x4, f32x3x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32x3, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32x3, u32);
}

} // namespace GPULang
