//------------------------------------------------------------------------------
//  @file int.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int.h"
namespace GPULang
{

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

    __IMPLEMENT_FUNCTION_1(ctorUInt, int, int, uint)

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, int, int);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, int, int);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, int, int);
    __IMPLEMENT_FUNCTION_1(leftShiftOperator, operator<<, int, int);
    __IMPLEMENT_FUNCTION_1(rightShiftOperator, operator>>, int, int);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, int, int);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, int, int);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, int, int);
    __IMPLEMENT_FUNCTION_1(leftShiftAssignOperator, operator<<=, int, int);
    __IMPLEMENT_FUNCTION_1(rightShiftAssignOperator, operator>>=, int, int);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, int, int);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, int, int);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, int, int);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, int, int);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, int, int);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, int, int);
    __IMPLEMENT_FUNCTION_1(moduloOperator, operator%, int, int);
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

    __IMPLEMENT_FUNCTION_1(assignOperator, operator=, int, uint);
}

} // namespace GPULang
