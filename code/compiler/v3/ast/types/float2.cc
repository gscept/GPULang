//------------------------------------------------------------------------------
//  @file float2.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float2.h"
namespace GPULang
{

Function Float2::ctor;

#define X(type, ctor, val, args, splat, size, conversion)\
    Function type::ctor;

    FLOAT2_CTOR_LIST
#undef X

Function Float2::additionOperator;
Function Float2::subtractionOperator;
Function Float2::multiplicationOperator;
Function Float2::divisionOperator;
Function Float2::modOperator;
Function Float2::scaleOperator;
Function Float2::matrix24Mul;
Function Float2::matrix23Mul;
Function Float2::matrix22Mul;
Function Float2::additionAssignOperator;
Function Float2::subtractionAssignOperator;
Function Float2::multiplicationAssignOperator;
Function Float2::divisionAssignOperator;
Function Float2::elementAccessOperatorInt;
Function Float2::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Float2::Float2()
{
    this->baseType = TypeCode::Float;
    this->columnSize = 2;
    this->rowSize = 1;
    this->byteSize = 8;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(ctor, f32x2, f32x2);
    __ADD_FUNCTION_PARAM(x, f32);
    __ADD_FUNCTION_PARAM(y, f32);

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(ctor, f32x2, f32x2, val);

    FLOAT2_CTOR_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, f32x2, f32x2);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, f32x2, f32x2);

    __IMPLEMENT_FUNCTION_1(matrix24Mul, operator*, f32x2, f32x2x4);
    __IMPLEMENT_FUNCTION_1(matrix23Mul, operator*, f32x2, f32x2x3);
    __IMPLEMENT_FUNCTION_1(matrix22Mul, operator*, f32x2, f32x2x2);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32, u32);

    char swizzleMask[] = { 'x', 'y' };
    __IMPLEMENT_SWIZZLE(f32, 2, swizzleMask)
}

} // namespace GPULang
