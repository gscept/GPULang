//------------------------------------------------------------------------------
//  @file mat2x2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x2.h"
#include "builtins.h"

namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Mat2x2::Mat2x2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x2x2");
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 2;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat2x2_identityConstructor, f32x2x2, Mat2x2);

    __IMPLEMENT_CTOR(Mat2x2_floatConstructor, f32x2x2, Mat2x2);
    __ADD_FUNCTION_PARAM(m00, Float);
    __ADD_FUNCTION_PARAM(m01, Float);
    __ADD_FUNCTION_PARAM(m10, Float);
    __ADD_FUNCTION_PARAM(m11, Float);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat2x2_vectorConstructor, f32x2x2, Mat2x2);
    __ADD_FUNCTION_PARAM(r0, Float2);
    __ADD_FUNCTION_PARAM(r1, Float2);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat2x2_additionOperator, operator+, Mat2x2, Mat2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_subtractionOperator, operator-, Mat2x2, Mat2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_multiplyOperator, operator*, Mat2x2, Mat2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_additionAssignOperator, operator+=, Mat2x2, Mat2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_subtractionAssignOperator, operator-=, Mat2x2, Mat2x2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_multiplyAssignOperator, operator*=, Mat2x2, Mat2x2);

    __IMPLEMENT_FUNCTION_1(Mat2x2_vectorTransformOperator, operator*, Float2, Float2);
    __IMPLEMENT_FUNCTION_1(Mat2x2_scaleOperator, operator*, Mat2x2, Float);

    __IMPLEMENT_FUNCTION_1(Mat2x2_assignOperator, operator=, Mat2x2, Mat2x2);

    __IMPLEMENT_FUNCTION_1(Mat2x2_elementAccessOperatorInt, operator[], Float2, Int);
    __IMPLEMENT_FUNCTION_1(Mat2x2_elementAccessOperatorUInt, operator[], Float2, UInt);
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
