//------------------------------------------------------------------------------
//  @file uint.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint.h"
namespace GPULang
{
Function UInt::ctor;

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
Function UInt::fractOperator;

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

    __IMPLEMENT_GLOBAL_1(ctor, uint, uint, int);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, uint, uint);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, uint, uint);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, uint, uint);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, uint, uint);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, uint, uint);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, uint, uint);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, uint, uint);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, uint, uint);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, uint, uint);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, uint, uint);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, uint, uint);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, uint, uint);
    __IMPLEMENT_FUNCTION_1(fractOperator, fract, uint, uint);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, uint, uint);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, uint, uint);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, uint, uint);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, uint, uint);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, uint, uint);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, uint, uint);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, uint, uint);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, uint, uint);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, uint, uint);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, uint, uint);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, bool, uint);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, bool, uint);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, bool, uint);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, bool, uint);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, bool, uint);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, bool, int);

    __IMPLEMENT_FUNCTION_1(floatAssignOperator, operator=, uint, float);
    __IMPLEMENT_FUNCTION_1(intAssignOperator, operator=, uint, int);

}

} // namespace GPULang
