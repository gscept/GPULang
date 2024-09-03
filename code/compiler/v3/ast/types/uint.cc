//------------------------------------------------------------------------------
//  @file uint.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint.h"
namespace GPULang
{

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

    __IMPLEMENT_FUNCTION_1(ctorInt, uint, uint, int);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, uint, uint);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, uint, uint);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, uint, uint);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, uint, uint);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, uint, uint);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, uint, uint);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, uint, uint);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, uint, uint);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, uint, uint);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, uint, uint);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, uint, uint);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, uint, uint);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, uint, uint);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, uint, uint);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, uint, uint);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, uint, uint);
    __IMPLEMENT_FUNCTION_1(moduloOperator, operator%, uint, uint);
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

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, uint, int);
}

} // namespace GPULang
