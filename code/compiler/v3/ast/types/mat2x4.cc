//------------------------------------------------------------------------------
//  @file mat2x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x4.h"
namespace GPULang
{

Function Mat2x4::floatConstructor;
Function Mat2x4::vectorConstructor;

Function Mat2x4::additionOperator;
Function Mat2x4::subtractionOperator;
Function Mat2x4::multiplyOperator;
Function Mat2x4::additionAssignOperator;
Function Mat2x4::subtractionAssignOperator;
Function Mat2x4::multiplyAssignOperator;

Function Mat2x4::vectorTransformOperator;

Function Mat2x4::assignOperator;

Function Mat2x4::elementAccessOperatorInt;
Function Mat2x4::elementAccessOperatorUInt;

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

    __IMPLEMENT_GLOBAL(floatConstructor, f32x2x4, f32x2x4);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m02, f32);
    __ADD_FUNCTION_PARAM(m03, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m12, f32);
    __ADD_FUNCTION_PARAM(m13, f32);

    __IMPLEMENT_GLOBAL(vectorConstructor, f32x2x4, f32x2x4);
    __ADD_FUNCTION_PARAM(r0, f32x2);
    __ADD_FUNCTION_PARAM(r1, f32x2);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, f32x2x4, f32x2x4);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, f32x2, f32x2);

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, f32x2x4, f32x2x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32x2, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32x2, u32);
}

} // namespace GPULang
