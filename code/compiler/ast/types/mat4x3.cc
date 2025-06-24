//------------------------------------------------------------------------------
//  @file mat4x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat4x3.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Mat4x3::Mat4x3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 4;
    this->byteSize = 48;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat4x3_floatConstructor, f32x4x3, f32x4x3, Mat4x3);
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
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat4x3_vectorConstructor, f32x4x3, f32x4x3, Mat4x3);
    __ADD_FUNCTION_PARAM(r0, f32x4);
    __ADD_FUNCTION_PARAM(r1, f32x4);
    __ADD_FUNCTION_PARAM(r2, f32x4);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat4x3_additionOperator, operator+, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_subtractionOperator, operator-, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_multiplyOperator, operator*, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_additionAssignOperator, operator+=, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_subtractionAssignOperator, operator-=, f32x4x3, f32x4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_multiplyAssignOperator, operator*=, f32x4x3, f32x4x3);

    __IMPLEMENT_FUNCTION_1(Mat4x3_vectorTransformOperator, operator*, f32x4, f32x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_scaleOperator, operator*, f32x4x3, f32);

    __IMPLEMENT_FUNCTION_1(Mat4x3_assignOperator, operator=, f32x4x3, f32x4x3);

    __IMPLEMENT_FUNCTION_1(Mat4x3_elementAccessOperatorInt, operator[], f32x4, i32);
    __IMPLEMENT_FUNCTION_1(Mat4x3_elementAccessOperatorUInt, operator[], f32x4, u32);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
