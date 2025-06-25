//------------------------------------------------------------------------------
//  @file mat4x2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat4x2.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat4x2::Mat4x2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x4x2");
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 4;
    this->byteSize = 32;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat4x2_floatConstructor, f32x4x2, Mat4x2);
    __ADD_FUNCTION_PARAM(m00, Float);
    __ADD_FUNCTION_PARAM(m01, Float);
    __ADD_FUNCTION_PARAM(m10, Float);
    __ADD_FUNCTION_PARAM(m11, Float);
    __ADD_FUNCTION_PARAM(m20, Float);
    __ADD_FUNCTION_PARAM(m21, Float);
    __ADD_FUNCTION_PARAM(m30, Float);
    __ADD_FUNCTION_PARAM(m31, Float);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat4x2_vectorConstructor, f32x4x2, Mat4x2);
    __ADD_FUNCTION_PARAM(r0, Float4);
    __ADD_FUNCTION_PARAM(r1, Float4);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat4x2_additionOperator, operator+, Mat4x2, Mat4x2);
    __IMPLEMENT_FUNCTION_1(Mat4x2_subtractionOperator, operator-, Mat4x2, Mat4x2);
    __IMPLEMENT_FUNCTION_1(Mat4x2_multiplyOperator, operator*, Mat4x2, Mat4x2);
    __IMPLEMENT_FUNCTION_1(Mat4x2_additionAssignOperator, operator+=, Mat4x2, Mat4x2);
    __IMPLEMENT_FUNCTION_1(Mat4x2_subtractionAssignOperator, operator-=, Mat4x2, Mat4x2);
    __IMPLEMENT_FUNCTION_1(Mat4x2_multiplyAssignOperator, operator*=, Mat4x2, Mat4x2);

    __IMPLEMENT_FUNCTION_1(Mat4x2_vectorTransformOperator, operator*, Float4, Float2);
    __IMPLEMENT_FUNCTION_1(Mat4x2_scaleOperator, operator*, Mat4x2, Float);

    __IMPLEMENT_FUNCTION_1(Mat4x2_assignOperator, operator=, Mat4x2, Mat4x2);

    __IMPLEMENT_FUNCTION_1(Mat4x2_elementAccessOperatorInt, operator[], Float4, Int);
    __IMPLEMENT_FUNCTION_1(Mat4x2_elementAccessOperatorUInt, operator[], Float4, UInt);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
