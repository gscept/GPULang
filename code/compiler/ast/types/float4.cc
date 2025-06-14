//------------------------------------------------------------------------------
//  @file float4.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float4.h"
namespace GPULang
{

Function Float4::ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    Function type::ctor;

    FLOAT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type::ctor;

    FLOAT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    Function type::ctor;

    FLOAT4_CTOR3_LIST
#undef X

Function Float4::additionOperator;
Function Float4::subtractionOperator;
Function Float4::multiplicationOperator;
Function Float4::divisionOperator;
Function Float4::modOperator;
Function Float4::scaleOperator;
Function Float4::matrix44Mul;
Function Float4::matrix43Mul;
Function Float4::matrix42Mul;
Function Float4::additionAssignOperator;
Function Float4::subtractionAssignOperator;
Function Float4::multiplicationAssignOperator;
Function Float4::divisionAssignOperator;
Function Float4::elementAccessOperatorInt;
Function Float4::elementAccessOperatorUInt;

Function Float4::ltOperator;
Function Float4::lteOperator;
Function Float4::gtOperator;
Function Float4::gteOperator;
Function Float4::eOperator;
Function Float4::neOperator;

//------------------------------------------------------------------------------
/**
*/
Float4::Float4()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Float;
    this->columnSize = 4;
    this->rowSize = 1;
    this->byteSize = 16;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(ctor, f32x4, f32x4);
    __ADD_FUNCTION_PARAM(x, f32);
    __ADD_FUNCTION_PARAM(y, f32);
    __ADD_FUNCTION_PARAM(z, f32);
    __ADD_FUNCTION_PARAM(w, f32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(ctor, f32x4, f32x4, val);

    FLOAT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(ctor, f32x4, f32x4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    FLOAT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    __IMPLEMENT_CTOR(ctor, f32x4, f32x4)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_FUNCTION_PARAM(arg_2, arg2)\
    __ADD_CONSTRUCTOR();

    FLOAT4_CTOR3_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, f32x4, f32);

    __IMPLEMENT_FUNCTION_1(matrix44Mul, operator*, f32x4, f32x4x4);
    __IMPLEMENT_FUNCTION_1(matrix43Mul, operator*, f32x4, f32x4x3);
    __IMPLEMENT_FUNCTION_1(matrix42Mul, operator*, f32x4, f32x4x2);

    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, f32x4, f32x4);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, f32x4, f32x4);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8x4, f32x4);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8x4, f32x4);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], f32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], f32, u32);

    char swizzleMask[] = { 'x', 'y', 'z', 'w' };
    __IMPLEMENT_SWIZZLE(f32, 4, swizzleMask)

    char colorMask[] = { 'r', 'g', 'b', 'a' };
    __IMPLEMENT_SWIZZLE(f32, 4, colorMask)
}

} // namespace GPULang
