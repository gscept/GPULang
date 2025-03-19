//------------------------------------------------------------------------------
//  @file uint.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint.h"
namespace GPULang
{
Function UInt::ctor_Int;
Function UInt::ctor_Bool;
Function UInt::ctor_Float;

Function UInt::andOperator;
Function UInt::orOperator;
Function UInt::xorOperator;
Function UInt::leftShiftOperator;
Function UInt::rightShiftOperator;
Function UInt::ororOperator;
Function UInt::andandOperator;
Function UInt::additionOperator;
Function UInt::subtractionOperator;
Function UInt::multiplicationOperator;
Function UInt::divisionOperator;
Function UInt::modOperator;

Function UInt::andAssignOperator;
Function UInt::orAssignOperator;
Function UInt::xorAssignOperator;
Function UInt::leftShiftAssignOperator;
Function UInt::rightShiftAssignOperator;
Function UInt::additionAssignOperator;
Function UInt::subtractionAssignOperator;
Function UInt::multiplicationAssignOperator;
Function UInt::divisionAssignOperator;
Function UInt::moduloAssignOperator;

Function UInt::ltOperator;
Function UInt::lteOperator;
Function UInt::gtOperator;
Function UInt::gteOperator;
Function UInt::eOperator;
Function UInt::neOperator;

Function UInt::floatAssignOperator;
Function UInt::intAssignOperator;
Function UInt::boolAssignOperator;

//------------------------------------------------------------------------------
/**
*/
UInt::UInt()
{
    this->baseType = TypeCode::UInt;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(ctor_Int, u32, u32, i32);
    __IMPLEMENT_CTOR_1(ctor_Bool, u32, u32, b8);
    __IMPLEMENT_CTOR_1(ctor_Float, u32, u32, f32);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, u32, u32);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, u32, u32);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, u32, u32);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, u32, u32);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, u32, u32);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, u32, u32);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, u32, u32);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, u32, u32);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, u32, u32);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, u32, u32);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, u32, u32);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, u32, u32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, u32, u32);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, u32, u32);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, u32, u32);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, u32, u32);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, u32, u32);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, u32, u32);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, u32, u32);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, u32, u32);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, u32, u32);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, u32, u32);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8, u32);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8, u32);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8, u32);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8, u32);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8, u32);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8, i32);

    __IMPLEMENT_FUNCTION_1(floatAssignOperator, operator=, u32, f32);
    __IMPLEMENT_FUNCTION_1(intAssignOperator, operator=, u32, i32);
    __IMPLEMENT_FUNCTION_1(boolAssignOperator, operator=, u32, b8);

}

} // namespace GPULang
