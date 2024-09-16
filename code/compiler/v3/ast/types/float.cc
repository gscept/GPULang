//------------------------------------------------------------------------------
//  @file float.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "float.h"
namespace GPULang
{

Function Float::ctor_UInt;
Function Float::ctor_Int;
Function Float::additionOperator;
Function Float::subtractionOperator;
Function Float::multiplicationOperator;
Function Float::divisionOperator;
Function Float::modOperator;
Function Float::additionAssignOperator;
Function Float::subtractionAssignOperator;
Function Float::multiplicationAssignOperator;
Function Float::divisionAssignOperator;

Function Float::ltOperator;
Function Float::lteOperator;
Function Float::gtOperator;
Function Float::gteOperator;
Function Float::eOperator;
Function Float::neOperator;

Function Float::intAssignOperator;
Function Float::uintAssignOperator;

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

    __IMPLEMENT_GLOBAL_1(ctor_UInt, float, float, uint);
    __IMPLEMENT_GLOBAL_1(ctor_Int, float, float, int);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, float, float);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, float, float);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, float, float);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, float, float);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, float, float);

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

} // namespace GPULang
