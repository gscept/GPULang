//------------------------------------------------------------------------------
//  @file mat4x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat4x4.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat4x4::Mat4x4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x4x4");
    this->baseType = TypeCode::Float;
    this->rowSize = 4;
    this->columnSize = 4;
    this->byteSize = 64;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat4x4_identityConstructor, f32x4x4, Mat4x4);

    __IMPLEMENT_CTOR(Mat4x4_floatConstructor, f32x4x4, Mat4x4);
    __ADD_FUNCTION_PARAM(m00, Float);
    __ADD_FUNCTION_PARAM(m01, Float);
    __ADD_FUNCTION_PARAM(m02, Float);
    __ADD_FUNCTION_PARAM(m03, Float);
    __ADD_FUNCTION_PARAM(m10, Float);
    __ADD_FUNCTION_PARAM(m11, Float);
    __ADD_FUNCTION_PARAM(m12, Float);
    __ADD_FUNCTION_PARAM(m13, Float);
    __ADD_FUNCTION_PARAM(m20, Float);
    __ADD_FUNCTION_PARAM(m21, Float);
    __ADD_FUNCTION_PARAM(m22, Float);
    __ADD_FUNCTION_PARAM(m23, Float);
    __ADD_FUNCTION_PARAM(m30, Float);
    __ADD_FUNCTION_PARAM(m31, Float);
    __ADD_FUNCTION_PARAM(m32, Float);
    __ADD_FUNCTION_PARAM(m33, Float);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat4x4_vectorConstructor, f32x4x4, Mat4x4);
    __ADD_FUNCTION_PARAM(r0, Float4);
    __ADD_FUNCTION_PARAM(r1, Float4);
    __ADD_FUNCTION_PARAM(r2, Float4);
    __ADD_FUNCTION_PARAM(r3, Float4);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat4x4_additionOperator, operator+, Mat4x4, Mat4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_subtractionOperator, operator-, Mat4x4, Mat4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_multiplyOperator, operator*, Mat4x4, Mat4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_additionAssignOperator, operator+=, Mat4x4, Mat4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_subtractionAssignOperator, operator-=, Mat4x4, Mat4x4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_multiplyAssignOperator, operator*=, Mat4x4, Mat4x4);

    __IMPLEMENT_FUNCTION_1(Mat4x4_vectorTransformOperator, operator*, Float4, Float4);
    __IMPLEMENT_FUNCTION_1(Mat4x4_scaleOperator, operator*, Mat4x4, Float);

    __IMPLEMENT_FUNCTION_1(Mat4x4_assignOperator, operator=, Mat4x4, Mat4x4);

    __IMPLEMENT_FUNCTION_1(Mat4x4_elementAccessOperatorInt, operator[], Float4, Int);
    __IMPLEMENT_FUNCTION_1(Mat4x4_elementAccessOperatorUInt, operator[], Float4, UInt);
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
