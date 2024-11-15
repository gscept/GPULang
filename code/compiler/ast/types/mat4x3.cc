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
Function Mat4x3::scaleOperator;

Function Mat4x3::assignOperator;

Function Mat4x3::elementAccessOperatorInt;
Function Mat4x3::elementAccessOperatorUInt;


//------------------------------------------------------------------------------
/**
*/
Mat4x3::Mat4x3()
{
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 4;
    this->byteSize = 48;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(floatConstructor, f32x4x3, f32x4x3);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m02, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m12, f32);
    __ADD_FUNCTION_PARAM(m20, f32);
    __ADD_FUNCTION_PARAM(m21, f32);
    __ADD_FUNCTION_PARAM(m22, f32);
    __ADD_FUNCTION_PARAM(m30, f32);
    __ADD_FUNCTION_PARAM(m31, f32);
    __ADD_FUNCTION_PARAM(m32, f32);

    __IMPLEMENT_CTOR(vectorConstructor, f32x4x3, f32x4x3);
    __ADD_FUNCTION_PARAM(r0, f32x4);
    __ADD_FUNCTION_PARAM(r1, f32x4);
    __ADD_FUNCTION_PARAM(r2, f32x4);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, f32x4x3, f32x4x3);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, f32x4, f32x3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x4x3, f32);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, f32x4x3, f32x4x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32x4, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32x4, u32);
}

} // namespace GPULang
