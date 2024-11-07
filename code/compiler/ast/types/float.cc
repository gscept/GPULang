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

    __IMPLEMENT_CTOR_1(ctor_UInt, f32, f32, u32);
    __IMPLEMENT_CTOR_1(ctor_Int, f32, f32, i32);

    __IMPLEMENT_FUNCTION_1(additionOperator, operator+, f32, f32);
    __IMPLEMENT_FUNCTION_1(subtractionOperator, operator-, f32, f32);
    __IMPLEMENT_FUNCTION_1(multiplicationOperator, operator*, f32, f32);
    __IMPLEMENT_FUNCTION_1(divisionOperator, operator/, f32, f32);
    __IMPLEMENT_FUNCTION_1(modOperator, operator%, f32, f32);

    __IMPLEMENT_FUNCTION_1(additionAssignOperator, operator+=, f32, f32);
    __IMPLEMENT_FUNCTION_1(subtractionAssignOperator, operator-=, f32, f32);
    __IMPLEMENT_FUNCTION_1(multiplicationAssignOperator, operator*=, f32, f32);
    __IMPLEMENT_FUNCTION_1(divisionAssignOperator, operator/=, f32, f32);

    __IMPLEMENT_FUNCTION_1(ltOperator, operator<, b8, f32);
    __IMPLEMENT_FUNCTION_1(lteOperator, operator<=, b8, f32);
    __IMPLEMENT_FUNCTION_1(gtOperator, operator>, b8, f32);
    __IMPLEMENT_FUNCTION_1(gteOperator, operator>=, b8, f32);
    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8, f32);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8, f32);

    __IMPLEMENT_FUNCTION_1(intAssignOperator, operator=, f32, i32);
    __IMPLEMENT_FUNCTION_1(uintAssignOperator, operator=, f32, u32);
}

} // namespace GPULang
