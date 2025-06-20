//------------------------------------------------------------------------------
//  @file int.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int.h"
namespace GPULang
{

Function Int::ctor_UInt;
Function Int::ctor_Bool;
Function Int::ctor_Float;

Function Int::andOperator;
Function Int::orOperator;
Function Int::xorOperator;
Function Int::leftShiftOperator;
Function Int::rightShiftOperator;
Function Int::ororOperator;
Function Int::andandOperator;
Function Int::additionOperator;
Function Int::subtractionOperator;
Function Int::multiplicationOperator;
Function Int::divisionOperator;
Function Int::modOperator;

Function Int::andAssignOperator;
Function Int::orAssignOperator;
Function Int::xorAssignOperator;
Function Int::leftShiftAssignOperator;
Function Int::rightShiftAssignOperator;
Function Int::additionAssignOperator;
Function Int::subtractionAssignOperator;
Function Int::multiplicationAssignOperator;
Function Int::divisionAssignOperator;
Function Int::moduloAssignOperator;

Function Int::ltOperator;
Function Int::lteOperator;
Function Int::gtOperator;
Function Int::gteOperator;
Function Int::eOperator;
Function Int::neOperator;

Function Int::floatAssignOperator;
Function Int::uintAssignOperator;
Function Int::boolAssignOperator;

//------------------------------------------------------------------------------
/**
*/
Int::Int()
{
    __BEGIN_TYPE()
    this->baseType = TypeCode::Int;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(ctor_UInt, i32, i32, u32);
    __IMPLEMENT_CTOR_1(ctor_Bool, i32, i32, b8);
    __IMPLEMENT_CTOR_1(ctor_Float, i32, i32, f32);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, i32, i32);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, i32, i32);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, i32, i32);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, i32, i32);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, i32, i32);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, i32, i32);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, i32, i32);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, i32, i32);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, i32, i32);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, i32, i32);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, i32, i32);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, i32, i32);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, i32, i32);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, i32, i32);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, i32, i32);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, i32, i32);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, i32, i32);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, i32, i32);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, i32, i32);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, i32, i32);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, i32, i32);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, i32, i32);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8, i32);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8, i32);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8, i32);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8, i32);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8, i32);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8, i32);

    __IMPLEMENT_FUNCTION_1(floatAssignOperator, operator=, i32, f32);
    __IMPLEMENT_FUNCTION_1(uintAssignOperator, operator=, i32, u32);
    __IMPLEMENT_FUNCTION_1(boolAssignOperator, operator=, i32, b8);
}

} // namespace GPULang
