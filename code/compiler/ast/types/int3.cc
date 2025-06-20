//------------------------------------------------------------------------------
//  @file int3.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int3.h"
namespace GPULang
{

Function Int3::ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    Function type::ctor;

    INT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type::ctor;

    INT3_CTOR2_LIST
#undef X

Function Int3::andOperator;
Function Int3::orOperator;
Function Int3::xorOperator;
Function Int3::leftShiftOperator;
Function Int3::rightShiftOperator;
Function Int3::additionOperator;
Function Int3::subtractionOperator;
Function Int3::multiplicationOperator;
Function Int3::divisionOperator;
Function Int3::modOperator;
Function Int3::scaleOperator;

Function Int3::andAssignOperator;
Function Int3::orAssignOperator;
Function Int3::xorAssignOperator;
Function Int3::leftShiftAssignOperator;
Function Int3::rightShiftAssignOperator;
Function Int3::additionAssignOperator;
Function Int3::subtractionAssignOperator;
Function Int3::multiplicationAssignOperator;
Function Int3::divisionAssignOperator;
Function Int3::moduloAssignOperator;

Function Int3::ltOperator;
Function Int3::lteOperator;
Function Int3::gtOperator;
Function Int3::gteOperator;
Function Int3::eOperator;
Function Int3::neOperator;

Function Int3::elementAccessOperatorInt;
Function Int3::elementAccessOperatorUInt;

//------------------------------------------------------------------------------
/**
*/
Int3::Int3()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Int;
    this->columnSize = 3;
    this->rowSize = 1;
    this->byteSize = 12;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR(ctor, i32x3, i32x3);
    __ADD_FUNCTION_PARAM(x, i32);
    __ADD_FUNCTION_PARAM(y, i32);
    __ADD_FUNCTION_PARAM(z, i32);
    __ADD_CONSTRUCTOR();

#define X(type, ctor, val, args, splat, size, conversion)\
    __IMPLEMENT_CTOR_1(ctor, i32x3, i32x3, val);

    INT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    __IMPLEMENT_CTOR(ctor, i32x3, i32x3)\
    __ADD_FUNCTION_PARAM(arg_0, arg0)\
    __ADD_FUNCTION_PARAM(arg_1, arg1)\
    __ADD_CONSTRUCTOR();

    INT3_CTOR2_LIST
#undef X

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(scaleOperator, operator*, i32x3, i32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, i32x3, i32x3);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, i32x3, i32x3);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8x3, i32x3);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8x3, i32x3);

    __IMPLEMENT_FUNCTION_1(elementAccessOperatorInt, operator[], i32, i32);
    __IMPLEMENT_FUNCTION_1(elementAccessOperatorUInt, operator[], i32, u32);

    char swizzleMask[] = { 'x', 'y', 'z' };
    __IMPLEMENT_SWIZZLE(i32, 3, swizzleMask)
    
    char colorMask[] = { 'r', 'g', 'b' };
    __IMPLEMENT_SWIZZLE(i32, 3, colorMask)
}

} // namespace GPULang
