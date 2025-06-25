//------------------------------------------------------------------------------
//  @file mat3x2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat3x2.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat3x2::Mat3x2()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x3x2");
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 2;
    this->byteSize = 24;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat3x2_floatConstructor, f32x3x2, Mat3x2);
    __ADD_FUNCTION_PARAM(m00, Float);
    __ADD_FUNCTION_PARAM(m01, Float);
    __ADD_FUNCTION_PARAM(m10, Float);
    __ADD_FUNCTION_PARAM(m11, Float);
    __ADD_FUNCTION_PARAM(m20, Float);
    __ADD_FUNCTION_PARAM(m21, Float);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat3x2_vectorConstructor, f32x3x2, Mat3x2);
    __ADD_FUNCTION_PARAM(r0, Float3);
    __ADD_FUNCTION_PARAM(r1, Float3);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat3x2_additionOperator, operator+, Mat3x2, Mat3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_subtractionOperator, operator-, Mat3x2, Mat3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_multiplyOperator, operator*, Mat3x2, Mat3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_additionAssignOperator, operator+=, Mat3x2, Mat3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_subtractionAssignOperator, operator-=, Mat3x2, Mat3x2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_multiplyAssignOperator, operator*=, Mat3x2, Mat3x2);

    __IMPLEMENT_FUNCTION_1(Mat3x2_vectorTransformOperator, operator*, Float3, Float2);
    __IMPLEMENT_FUNCTION_1(Mat3x2_scaleOperator, operator*, Mat3x2, Float);

    __IMPLEMENT_FUNCTION_1(Mat3x2_assignOperator, operator=, Mat3x2, Mat3x2);

    __IMPLEMENT_FUNCTION_1(Mat3x2_elementAccessOperatorInt, operator[], Float3, Int);
    __IMPLEMENT_FUNCTION_1(Mat3x2_elementAccessOperatorUInt, operator[], Float3, UInt);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
