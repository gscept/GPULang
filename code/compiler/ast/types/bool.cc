//------------------------------------------------------------------------------
//  @file bool.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool.h"
namespace GPULang
{


//Function Bool::ctor_UInt;
//Function Bool::ctor_Int;
//
//Function Bool::ororOperator;
//Function Bool::andandOperator;
//Function Bool::eOperator;
//Function Bool::neOperator;
//Function Bool::boolAssignOperator;
//Function Bool::intAssignOperator;
//Function Bool::uintAssignOperator;

//------------------------------------------------------------------------------
/**
*/
Bool::Bool()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_TYPE()
    this->baseType = TypeCode::Bool;
    this->rowSize = 1;
    this->columnSize = 1;
    this->byteSize = 1;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(Bool_ctor_Int, b8, b8, i32, Bool);

    // setup uint constructor
    __IMPLEMENT_CTOR_1(Bool_ctor_UInt, b8, b8, u32, Bool);
    
    __IMPLEMENT_FUNCTION_1(Bool_ororOperator, operator||, b8, b8);
    __IMPLEMENT_FUNCTION_1(Bool_andandOperator, operator&&, b8, b8);

    __IMPLEMENT_FUNCTION_1(Bool_eOperator, operator==, b8, b8);
    __IMPLEMENT_FUNCTION_1(Bool_neOperator, operator!=, b8, b8);

    __IMPLEMENT_FUNCTION_1(Bool_boolAssignOperator, operator=, b8, b8);
    __IMPLEMENT_FUNCTION_1(Bool_intAssignOperator, operator=, b8, i32);
    __IMPLEMENT_FUNCTION_1(Bool_uintAssignOperator, operator=, b8, u32);
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
