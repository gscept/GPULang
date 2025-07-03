//------------------------------------------------------------------------------
//  @file mat2x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x4.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat2x4::Mat2x4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->name = ConstantString("f32x2x4");
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 4;
    this->byteSize = 32;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat2x4_floatConstructor, f32x2x4, Mat2x4);
    __ADD_FUNCTION_PARAM(m00, Float);
    __ADD_FUNCTION_PARAM(m01, Float);
    __ADD_FUNCTION_PARAM(m02, Float);
    __ADD_FUNCTION_PARAM(m03, Float);
    __ADD_FUNCTION_PARAM(m10, Float);
    __ADD_FUNCTION_PARAM(m11, Float);
    __ADD_FUNCTION_PARAM(m12, Float);
    __ADD_FUNCTION_PARAM(m13, Float);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat2x4_vectorConstructor, f32x2x4, Mat2x4);
    __ADD_FUNCTION_PARAM(r0, Float2);
    __ADD_FUNCTION_PARAM(r1, Float2);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat2x4_additionOperator, operator+, Mat2x4, Mat2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_subtractionOperator, operator-, Mat2x4, Mat2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_multiplyOperator, operator*, Mat2x4, Mat2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_additionAssignOperator, operator+=, Mat2x4, Mat2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_subtractionAssignOperator, operator-=, Mat2x4, Mat2x4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_multiplyAssignOperator, operator*=, Mat2x4, Mat2x4);

    __IMPLEMENT_FUNCTION_1(Mat2x4_vectorTransformOperator, operator*, Float2, Float4);
    __IMPLEMENT_FUNCTION_1(Mat2x4_scaleOperator, operator*, Mat2x4, Float);

    __IMPLEMENT_FUNCTION_1(Mat2x4_assignOperator, operator=, Mat2x4, Mat2x4);

    __IMPLEMENT_FUNCTION_1(Mat2x4_elementAccessOperatorInt, operator[], Float2, Int);
    __IMPLEMENT_FUNCTION_1(Mat2x4_elementAccessOperatorUInt, operator[], Float2, UInt);
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
