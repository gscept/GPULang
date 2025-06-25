//------------------------------------------------------------------------------
//  @file mat4x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat4x3.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat4x3::Mat4x3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x4x3");
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 4;
    this->byteSize = 48;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat4x3_floatConstructor, f32x4x3, Mat4x3);
    __ADD_FUNCTION_PARAM(m00, Float);
    __ADD_FUNCTION_PARAM(m01, Float);
    __ADD_FUNCTION_PARAM(m02, Float);
    __ADD_FUNCTION_PARAM(m10, Float);
    __ADD_FUNCTION_PARAM(m11, Float);
    __ADD_FUNCTION_PARAM(m12, Float);
    __ADD_FUNCTION_PARAM(m20, Float);
    __ADD_FUNCTION_PARAM(m21, Float);
    __ADD_FUNCTION_PARAM(m22, Float);
    __ADD_FUNCTION_PARAM(m30, Float);
    __ADD_FUNCTION_PARAM(m31, Float);
    __ADD_FUNCTION_PARAM(m32, Float);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat4x3_vectorConstructor, f32x4x3, Mat4x3);
    __ADD_FUNCTION_PARAM(r0, Float4);
    __ADD_FUNCTION_PARAM(r1, Float4);
    __ADD_FUNCTION_PARAM(r2, Float4);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat4x3_additionOperator, operator+, Mat4x3, Mat4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_subtractionOperator, operator-, Mat4x3, Mat4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_multiplyOperator, operator*, Mat4x3, Mat4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_additionAssignOperator, operator+=, Mat4x3, Mat4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_subtractionAssignOperator, operator-=, Mat4x3, Mat4x3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_multiplyAssignOperator, operator*=, Mat4x3, Mat4x3);

    __IMPLEMENT_FUNCTION_1(Mat4x3_vectorTransformOperator, operator*, Float4, Float3);
    __IMPLEMENT_FUNCTION_1(Mat4x3_scaleOperator, operator*, Mat4x3, Float);

    __IMPLEMENT_FUNCTION_1(Mat4x3_assignOperator, operator=, Mat4x3, Mat4x3);

    __IMPLEMENT_FUNCTION_1(Mat4x3_elementAccessOperatorInt, operator[], Float4, Int);
    __IMPLEMENT_FUNCTION_1(Mat4x3_elementAccessOperatorUInt, operator[], Float4, UInt);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
