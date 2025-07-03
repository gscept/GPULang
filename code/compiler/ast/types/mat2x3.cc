//------------------------------------------------------------------------------
//  @file mat2x3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "mat2x3.h"
#include "builtins.h"

namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Mat2x3::Mat2x3()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("f32x2x3");
    this->baseType = TypeCode::Float;
    this->rowSize = 2;
    this->columnSize = 3;
    this->byteSize = 24;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(Mat2x3_floatConstructor, f32x2x3, Mat2x3);
    __ADD_FUNCTION_PARAM(m00, Float);
    __ADD_FUNCTION_PARAM(m01, Float);
    __ADD_FUNCTION_PARAM(m02, Float);
    __ADD_FUNCTION_PARAM(m10, Float);
    __ADD_FUNCTION_PARAM(m11, Float);
    __ADD_FUNCTION_PARAM(m12, Float);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_CTOR(Mat2x3_vectorConstructor, f32x2x3, Mat2x3);
    __ADD_FUNCTION_PARAM(r0, Float2);
    __ADD_FUNCTION_PARAM(r1, Float2);
    __ADD_CONSTRUCTOR();

    __IMPLEMENT_FUNCTION_1(Mat2x3_additionOperator, operator+, Mat2x3, Mat2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_subtractionOperator, operator-, Mat2x3, Mat2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_multiplyOperator, operator*, Mat2x3, Mat2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_additionAssignOperator, operator+=, Mat2x3, Mat2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_subtractionAssignOperator, operator-=, Mat2x3, Mat2x3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_multiplyAssignOperator, operator*=, Mat2x3, Mat2x3);

    __IMPLEMENT_FUNCTION_1(Mat2x3_vectorTransformOperator, operator*, Float2, Float3);
    __IMPLEMENT_FUNCTION_1(Mat2x3_scaleOperator, operator*, Mat2x3, Float);
    
    __IMPLEMENT_FUNCTION_1(Mat2x3_assignOperator, operator=, Mat2x3, Mat2x3);

    __IMPLEMENT_FUNCTION_1(Mat2x3_elementAccessOperatorInt, operator[], Float2, Int);
    __IMPLEMENT_FUNCTION_1(Mat2x3_elementAccessOperatorUInt, operator[], Float2, UInt);
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
