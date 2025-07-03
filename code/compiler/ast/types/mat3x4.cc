//------------------------------------------------------------------------------
//  @file mat3x4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat3x4.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat3x4::Mat3x4()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x3x4");
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 4;
    this->byteSize = 48;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat3x4_floatConstructor, f32x3x4, Mat3x4);
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
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat3x4_vectorConstructor, f32x3x4, Mat3x4);
    __ADD_FUNCTION_PARAM(r0, Float3);
    __ADD_FUNCTION_PARAM(r1, Float3);
    __ADD_FUNCTION_PARAM(r2, Float3);
    __ADD_FUNCTION_PARAM(r3, Float3);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat3x4_additionOperator, operator+, Mat3x4, Mat3x4);
    __IMPLEMENT_FUNCTION_1(Mat3x4_subtractionOperator, operator-, Mat3x4, Mat3x4);
    __IMPLEMENT_FUNCTION_1(Mat3x4_multiplyOperator, operator*, Mat3x4, Mat3x4);
    __IMPLEMENT_FUNCTION_1(Mat3x4_additionAssignOperator, operator+=, Mat3x4, Mat3x4);
    __IMPLEMENT_FUNCTION_1(Mat3x4_subtractionAssignOperator, operator-=, Mat3x4, Mat3x4);
    __IMPLEMENT_FUNCTION_1(Mat3x4_multiplyAssignOperator, operator*=, Mat3x4, Mat3x4);

    __IMPLEMENT_FUNCTION_1(Mat3x4_vectorTransformOperator, operator*, Float3, Float4);
    __IMPLEMENT_FUNCTION_1(Mat3x4_scaleOperator, operator*, Mat3x4, Float);

    __IMPLEMENT_FUNCTION_1(Mat3x4_assignOperator, operator=, Mat3x4, Mat3x4);

    __IMPLEMENT_FUNCTION_1(Mat3x4_elementAccessOperatorInt, operator[], Float3, Int);
    __IMPLEMENT_FUNCTION_1(Mat3x4_elementAccessOperatorUInt, operator[], Float3, UInt);
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
