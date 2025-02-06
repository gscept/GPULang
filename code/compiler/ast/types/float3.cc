//------------------------------------------------------------------------------
//  @file float3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float3.h"
namespace GPULang
{

Function Float3::ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    Function type::ctor;

    FLOAT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type::ctor;

    FLOAT3_CTOR2_LIST
#undef X

Function Float3::additionOperator;
Function Float3::subtractionOperator;
Function Float3::multiplicationOperator;
Function Float3::divisionOperator;
Function Float3::modOperator;
Function Float3::scaleOperator;
Function Float3::matrix34Mul;
Function Float3::matrix33Mul;
Function Float3::matrix32Mul;
Function Float3::additionAssignOperator;
Function Float3::subtractionAssignOperator;
Function Float3::multiplicationAssignOperator;
Function Float3::divisionAssignOperator;
Function Float3::elementAccessOperatorInt;
Function Float3::elementAccessOperatorUInt;

Function Float3::ltOperator;
Function Float3::lteOperator;
Function Float3::gtOperator;
Function Float3::gteOperator;
Function Float3::eOperator;
Function Float3::neOperator;

//------------------------------------------------------------------------------
/**
*/
Float3::Float3()
{
    this->baseType = TypeCode::Float;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(ctor, f32x3, f32x3);
    __ADD_FUNCTION_PARAM(x, f32);
    __ADD_FUNCTION_PARAM(y, f32);
    __ADD_FUNCTION_PARAM(z, f32);

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(ctor, f32x3, f32x3, val);

    FLOAT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(ctor, f32x3, f32x3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)

    FLOAT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x3, f32);

    __IMPLEMENT_FUNCTION_1(matrix34Mul, operator*, f32x3, f32x3x4);
    __IMPLEMENT_FUNCTION_1(matrix33Mul, operator*, f32x3, f32x3x3);
    __IMPLEMENT_FUNCTION_1(matrix32Mul, operator*, f32x3, f32x3x2);

    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, f32x3, f32x3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, f32x3, f32x3);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8x3, f32x3);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8x3, f32x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32, u32);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(f32, 3, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(f32, 3, colorMask)
}

} // namespace GPULang
