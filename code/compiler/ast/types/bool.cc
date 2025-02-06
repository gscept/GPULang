//------------------------------------------------------------------------------
//  @file bool.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool.h"
namespace GPULang
{

Function Bool::ctor_UInt;
Function Bool::ctor_Int;

Function Bool::ororOperator;
Function Bool::andandOperator;
Function Bool::eOperator;
Function Bool::neOperator;
Function Bool::boolAssignOperator;
Function Bool::intAssignOperator;
Function Bool::uintAssignOperator;

//------------------------------------------------------------------------------
/**
*/
Bool::Bool()
{
    this->baseType = TypeCode::Bool;
    this->rowSize = 1;
    this->columnSize = 1;
    this->byteSize = 1;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(ctor_Int, b8, b8, i32);
    __ADD_CONSTRUCTOR()

    // setup uint constructor
    __IMPLEMENT_CTOR_1(ctor_UInt, b8, b8, u32);
    __ADD_CONSTRUCTOR()
    
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, b8, b8);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, b8, b8);

    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8, b8);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8, b8);

    __IMPLEMENT_FUNCTION_1(boolAssignOperator, operator=, b8, b8);
    __IMPLEMENT_FUNCTION_1(intAssignOperator, operator=, b8, i32);
    __IMPLEMENT_FUNCTION_1(uintAssignOperator, operator=, b8, u32);
}

} // namespace GPULang
