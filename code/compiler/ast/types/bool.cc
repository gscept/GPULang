//------------------------------------------------------------------------------
//  @file bool.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool.h"
#include "builtins.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
Bool::Bool()
{
    SYMBOL_STATIC_ALLOC = true;
    
    __BEGIN_TYPE()
    this->name = ConstantString("b8");
    this->baseType = TypeCode::Bool;
    this->rowSize = 1;
    this->columnSize = 1;
    this->byteSize = 1;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(Bool_ctor_Int, b8, Bool, Int);

    // setup uint constructor
    __IMPLEMENT_CTOR_1(Bool_ctor_UInt, b8, Bool, UInt);
    
    __IMPLEMENT_FUNCTION_1(Bool_ororOperator, operator||, Bool, Bool);
    __IMPLEMENT_FUNCTION_1(Bool_andandOperator, operator&&, Bool, Bool);

    __IMPLEMENT_FUNCTION_1(Bool_eOperator, operator==, Bool, Bool);
    __IMPLEMENT_FUNCTION_1(Bool_neOperator, operator!=, Bool, Bool);

    __IMPLEMENT_FUNCTION_1(Bool_boolAssignOperator, operator=, Bool, Bool);
    __IMPLEMENT_FUNCTION_1(Bool_intAssignOperator, operator=, Bool, Int);
    __IMPLEMENT_FUNCTION_1(Bool_uintAssignOperator, operator=, Bool, UInt);
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
