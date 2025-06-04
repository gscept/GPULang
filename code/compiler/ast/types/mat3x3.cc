//------------------------------------------------------------------------------
//  @file mat3x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat3x3.h"
namespace GPULang
{

Function Mat3x3::identityConstructor;
Function Mat3x3::floatConstructor;
Function Mat3x3::vectorConstructor;

Function Mat3x3::additionOperator;
Function Mat3x3::subtractionOperator;
Function Mat3x3::multiplyOperator;
Function Mat3x3::additionAssignOperator;
Function Mat3x3::subtractionAssignOperator;
Function Mat3x3::multiplyAssignOperator;

Function Mat3x3::vectorTransformOperator;
Function Mat3x3::scaleOperator;

Function Mat3x3::assignOperator;

Function Mat3x3::elementAccessOperatorInt;
Function Mat3x3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Mat3x3::Mat3x3()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 3;
    this->byteSize = 36;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(identityConstructor, f32x3x3, f32x3x3);

    __IMPLEMENT_CTOR(floatConstructor, f32x3x3, f32x3x3);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m02, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m12, f32);
    __ADD_FUNCTION_PARAM(m20, f32);
    __ADD_FUNCTION_PARAM(m21, f32);
    __ADD_FUNCTION_PARAM(m22, f32);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(vectorConstructor, f32x3x3, f32x3x3);
    __ADD_FUNCTION_PARAM(r0, f32x3);
    __ADD_FUNCTION_PARAM(r1, f32x3);
    __ADD_FUNCTION_PARAM(r2, f32x3);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x3x3, f32x3x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x3x3, f32x3x3);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, f32x3x3, f32x3x3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x3x3, f32x3x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x3x3, f32x3x3);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, f32x3x3, f32x3x3);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x3x3, f32);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, f32x3x3, f32x3x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32x3, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32x3, u32);
}

} // namespace GPULang
