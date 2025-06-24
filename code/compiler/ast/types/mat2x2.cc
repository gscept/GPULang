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
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 2;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat2x2_identityConstructor, f32x2x2, f32x2x2, Mat2x2);

    __IMPLEMENT_CTOR(Mat2x2_floatConstructor, f32x2x2, f32x2x2, Mat2x2);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat2x2_vectorConstructor, f32x2x2, f32x2x2, Mat2x2);
    __ADD_FUNCTION_PARAM(r0, f32x2);
    __ADD_FUNCTION_PARAM(r1, f32x2);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat2x2_additionOperator, operator+, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_subtractionOperator, operator-, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_multiplyOperator, operator*, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_additionAssignOperator, operator+=, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_subtractionAssignOperator, operator-=, f32x2x2, f32x2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_multiplyAssignOperator, operator*=, f32x2x2, f32x2x2);

    __IMPLEMENT_FUNCTION_1(Mat2x2_vectorTransformOperator, operator*, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_scaleOperator, operator*, f32x2x2, f32);

    __IMPLEMENT_FUNCTION_1(Mat2x2_assignOperator, operator=, f32x2x2, f32x2x2);

    __IMPLEMENT_FUNCTION_1(Mat2x2_elementAccessOperatorInt, operator[], f32x2, i32);
    __IMPLEMENT_FUNCTION_1(Mat2x2_elementAccessOperatorUInt, operator[], f32x2, u32);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
