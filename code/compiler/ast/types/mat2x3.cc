//------------------------------------------------------------------------------
//  @file mat2x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x3.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Mat2x3::Mat2x3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 3;
    this->byteSize = 24;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat2x3_floatConstructor, f32x2x3, f32x2x3, Mat2x3);
    __ADD_FUNCTION_PARAM(m00, f32);
    __ADD_FUNCTION_PARAM(m01, f32);
    __ADD_FUNCTION_PARAM(m02, f32);
    __ADD_FUNCTION_PARAM(m10, f32);
    __ADD_FUNCTION_PARAM(m11, f32);
    __ADD_FUNCTION_PARAM(m12, f32);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat2x3_vectorConstructor, f32x2x3, f32x2x3, Mat2x3);
    __ADD_FUNCTION_PARAM(r0, f32x2);
    __ADD_FUNCTION_PARAM(r1, f32x2);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat2x3_additionOperator, operator+, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_subtractionOperator, operator-, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_multiplyOperator, operator*, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_additionAssignOperator, operator+=, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_subtractionAssignOperator, operator-=, f32x2x3, f32x2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_multiplyAssignOperator, operator*=, f32x2x3, f32x2x3);

    __IMPLEMENT_FUNCTION_1(Mat2x3_vectorTransformOperator, operator*, f32x2, f32x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_scaleOperator, operator*, f32x2x3, f32);
    
    __IMPLEMENT_FUNCTION_1(Mat2x3_assignOperator, operator=, f32x2x3, f32x2x3);

    __IMPLEMENT_FUNCTION_1(Mat2x3_elementAccessOperatorInt, operator[], f32x2, i32);
    __IMPLEMENT_FUNCTION_1(Mat2x3_elementAccessOperatorUInt, operator[], f32x2, u32);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
