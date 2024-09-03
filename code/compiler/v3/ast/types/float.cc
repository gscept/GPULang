//------------------------------------------------------------------------------
//  @file float.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float.h"
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
Float::Float()
{
    this->baseType = TypeCode::Float;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_FUNCTION_1(ctorUInt, float, float, uint);
    __IMPLEMENT_FUNCTION_1(ctorInt, float, float, int);
    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float, float);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float, float);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, float, float);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, float, float);
    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, float, float);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, float, float);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, float, float);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, float, float);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, bool, float);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, bool, float);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, bool, float);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, bool, float);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, bool, float);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, bool, float);

    __IMPLEMENT_FUNCTION_1(intAssignOperator, operator=, float, int);
    __IMPLEMENT_FUNCTION_1(uintAssignOperator, operator=, float, uint);
}

} // namespace AnyFX
