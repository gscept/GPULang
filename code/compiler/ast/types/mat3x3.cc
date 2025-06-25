//------------------------------------------------------------------------------
//  @file mat3x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat3x3.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Mat3x3::Mat3x3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x3x3");
    this->baseType = TypeCode::Float;
    this->rowSize = 3;
    this->columnSize = 3;
    this->byteSize = 36;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat3x3_identityConstructor, f32x3x3, Mat3x3);

    __IMPLEMENT_CTOR(Mat3x3_floatConstructor, f32x3x3, Mat3x3);
    __ADD_FUNCTION_PARAM(m00, Float);
    __ADD_FUNCTION_PARAM(m01, Float);
    __ADD_FUNCTION_PARAM(m02, Float);
    __ADD_FUNCTION_PARAM(m10, Float);
    __ADD_FUNCTION_PARAM(m11, Float);
    __ADD_FUNCTION_PARAM(m12, Float);
    __ADD_FUNCTION_PARAM(m20, Float);
    __ADD_FUNCTION_PARAM(m21, Float);
    __ADD_FUNCTION_PARAM(m22, Float);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat3x3_vectorConstructor, f32x3x3, Mat3x3);
    __ADD_FUNCTION_PARAM(r0, Float3);
    __ADD_FUNCTION_PARAM(r1, Float3);
    __ADD_FUNCTION_PARAM(r2, Float3);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat3x3_additionOperator, operator+, Mat3x3, Mat3x3);
    __IMPLEMENT_FUNCTION_1(Mat3x3_subtractionOperator, operator-, Mat3x3, Mat3x3);
    __IMPLEMENT_FUNCTION_1(Mat3x3_multiplyOperator, operator*, Mat3x3, Mat3x3);
    __IMPLEMENT_FUNCTION_1(Mat3x3_additionAssignOperator, operator+=, Mat3x3, Mat3x3);
    __IMPLEMENT_FUNCTION_1(Mat3x3_subtractionAssignOperator, operator-=, Mat3x3, Mat3x3);
    __IMPLEMENT_FUNCTION_1(Mat3x3_multiplyAssignOperator, operator*=, Mat3x3, Mat3x3);

    __IMPLEMENT_FUNCTION_1(Mat3x3_vectorTransformOperator, operator*, Float3, Float3);
    __IMPLEMENT_FUNCTION_1(Mat3x3_scaleOperator, operator*, Mat3x3, Float);

    __IMPLEMENT_FUNCTION_1(Mat3x3_assignOperator, operator=, Mat3x3, Mat3x3);

    __IMPLEMENT_FUNCTION_1(Mat3x3_elementAccessOperatorInt, operator[], Float3, Int);
    __IMPLEMENT_FUNCTION_1(Mat3x3_elementAccessOperatorUInt, operator[], Float3, UInt);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
