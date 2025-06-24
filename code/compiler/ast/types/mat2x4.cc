//------------------------------------------------------------------------------
//  @file mat2x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x4.h"
namespace GPULang
{



//------------------------------------------------------------------------------
/**
*/
Mat2x4::Mat2x4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 4;
    this->byteSize = 32;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat2x4_floatConstructor, f32x2x4, f32x2x4, Mat2x4);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m02, f32);
    __ADD_FUNCTION_PARAM(m03, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m12, f32);
    __ADD_FUNCTION_PARAM(m13, f32);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat2x4_vectorConstructor, f32x2x4, f32x2x4, Mat2x4);
    __ADD_FUNCTION_PARAM(r0, f32x2);
    __ADD_FUNCTION_PARAM(r1, f32x2);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat2x4_additionOperator, operator+, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_subtractionOperator, operator-, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_multiplyOperator, operator*, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_additionAssignOperator, operator+=, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_subtractionAssignOperator, operator-=, f32x2x4, f32x2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_multiplyAssignOperator, operator*=, f32x2x4, f32x2x4);

    __IMPLEMENT_FUNCTION_1(Mat2x4_vectorTransformOperator, operator*, f32x2, f32x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_scaleOperator, operator*, f32x2x4, f32);

    __IMPLEMENT_FUNCTION_1(Mat2x4_assignOperator, operator=, f32x2x4, f32x2x4);

    __IMPLEMENT_FUNCTION_1(Mat2x4_elementAccessOperatorInt, operator[], f32x2, i32);
    __IMPLEMENT_FUNCTION_1(Mat2x4_elementAccessOperatorUInt, operator[], f32x2, u32);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
