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
Function Mat2x3::scaleOperator;

Function Mat2x3::assignOperator;

Function Mat2x3::elementAccessOperatorInt;
Function Mat2x3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Mat2x3::Mat2x3()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 3;
    this->byteSize = 24;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(floatConstructor, f32x2x3, f32x2x3);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m02, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m12, f32);

    __IMPLEMENT_CTOR(vectorConstructor, f32x2x3, f32x2x3);
    __ADD_FUNCTION_PARAM(r0, f32x2);
    __ADD_FUNCTION_PARAM(r1, f32x2);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(multiplyOperator, operator*, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(multiplyAssignOperator, operator*=, f32x2x3, f32x2x3);

    __IMPLEMENT_FUNCTION_1(vectorTransformOperator, operator*, f32x2, f32x3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x2x3, f32);
    
    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, f32x2x3, f32x2x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32x2, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32x2, u32);
}

} // namespace GPULang
