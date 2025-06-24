//------------------------------------------------------------------------------
//  @file mat3x2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat3x2.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat3x2::Mat3x2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 2;
    this->byteSize = 24;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat3x2_floatConstructor, f32x3x2, f32x3x2, Mat3x2);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m20, f32);
    __ADD_FUNCTION_PARAM(m21, f32);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat3x2_vectorConstructor, f32x3x2, f32x3x2, Mat3x2);
    __ADD_FUNCTION_PARAM(r0, f32x3);
    __ADD_FUNCTION_PARAM(r1, f32x3);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat3x2_additionOperator, operator+, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_subtractionOperator, operator-, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_multiplyOperator, operator*, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_additionAssignOperator, operator+=, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_subtractionAssignOperator, operator-=, f32x3x2, f32x3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_multiplyAssignOperator, operator*=, f32x3x2, f32x3x2);

    __IMPLEMENT_FUNCTION_1(Mat3x2_vectorTransformOperator, operator*, f32x3, f32x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_scaleOperator, operator*, f32x3x2, f32);

    __IMPLEMENT_FUNCTION_1(Mat3x2_assignOperator, operator=, f32x3x2, f32x3x2);

    __IMPLEMENT_FUNCTION_1(Mat3x2_elementAccessOperatorInt, operator[], f32x3, i32);
    __IMPLEMENT_FUNCTION_1(Mat3x2_elementAccessOperatorUInt, operator[], f32x3, u32);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
