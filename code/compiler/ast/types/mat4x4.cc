//------------------------------------------------------------------------------
//  @file mat4x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat4x4.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat4x4::Mat4x4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->rowSize = 4;
    this->columnSize = 4;
    this->byteSize = 64;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat4x4_identityConstructor, f32x4x4, f32x4x4, Mat4x4);

    __IMPLEMENT_CTOR(Mat4x4_floatConstructor, f32x4x4, f32x4x4, Mat4x4);
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
    __ADD_FUNCTION_PARAM(m30, f32);
    __ADD_FUNCTION_PARAM(m31, f32);
    __ADD_FUNCTION_PARAM(m32, f32);
    __ADD_FUNCTION_PARAM(m33, f32);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat4x4_vectorConstructor, f32x4x4, f32x4x4, Mat4x4);
    __ADD_FUNCTION_PARAM(r0, f32x4);
    __ADD_FUNCTION_PARAM(r1, f32x4);
    __ADD_FUNCTION_PARAM(r2, f32x4);
    __ADD_FUNCTION_PARAM(r3, f32x4);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat4x4_additionOperator, operator+, f32x4x4, f32x4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_subtractionOperator, operator-, f32x4x4, f32x4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_multiplyOperator, operator*, f32x4x4, f32x4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_additionAssignOperator, operator+=, f32x4x4, f32x4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_subtractionAssignOperator, operator-=, f32x4x4, f32x4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_multiplyAssignOperator, operator*=, f32x4x4, f32x4x4);

    __IMPLEMENT_FUNCTION_1(Mat4x4_vectorTransformOperator, operator*, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_scaleOperator, operator*, f32x4x4, f32);

    __IMPLEMENT_FUNCTION_1(Mat4x4_assignOperator, operator=, f32x4x4, f32x4x4);

    __IMPLEMENT_FUNCTION_1(Mat4x4_elementAccessOperatorInt, operator[], f32x4, i32);
    __IMPLEMENT_FUNCTION_1(Mat4x4_elementAccessOperatorUInt, operator[], f32x4, u32);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
