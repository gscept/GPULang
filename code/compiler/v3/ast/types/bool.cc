//------------------------------------------------------------------------------
//  @file bool.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "bool.h"
namespace GPULang
{

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

    __IMPLEMENT_GLOBAL_1(ctor_Int, b8, b8, i32);
    __ADD_CONSTRUCTOR()

    // setup uint constructor
    __IMPLEMENT_GLOBAL_1(ctor_UInt, b8, b8, u32);
    __ADD_CONSTRUCTOR()

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, b8, b8);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, b8, b8);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, b8, b8);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, b8, b8);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, b8, b8);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, b8, b8);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, b8, b8);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, b8, b8);

    __IMPLEMENT_FUNCTION_1(eOperator, operator==, b8, b8);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, b8, b8);

    __IMPLEMENT_FUNCTION_1(boolAssignOperator, operator=, b8, b8);
    __IMPLEMENT_FUNCTION_1(intAssignOperator, operator=, b8, i32);
    __IMPLEMENT_FUNCTION_1(uintAssignOperator, operator=, b8, u32);
}

} // namespace GPULang
