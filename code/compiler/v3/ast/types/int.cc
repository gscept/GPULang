//------------------------------------------------------------------------------
//  @file int.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int.h"
namespace GPULang
{

Function Int::ctor;

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

//------------------------------------------------------------------------------
/**
*/
Int::Int()
{
    this->baseType = TypeCode::Int;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_GLOBAL_1(ctor, int, int, uint)
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, int, int);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, int, int);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, int, int);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, int, int);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, int, int);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, int, int);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, int, int);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, int, int);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, int, int);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, int, int);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, int, int);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, int, int);

    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, int, int);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, int, int);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, int, int);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, int, int);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, int, int);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, int, int);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, int, int);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, int, int);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, int, int);
    __IMPLEMENT_FUNCTION_1(moduloAssignOperator, operator%=, int, int);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, bool, int);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, bool, int);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, bool, int);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, bool, int);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, bool, int);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, bool, int);

    __IMPLEMENT_FUNCTION_1(floatAssignOperator, operator=, int, float);
    __IMPLEMENT_FUNCTION_1(uintAssignOperator, operator=, int, uint);
}

} // namespace GPULang
