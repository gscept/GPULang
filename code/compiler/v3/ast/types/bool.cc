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

    __IMPLEMENT_FUNCTION_1(ctorInt, bool, bool, int);

    // setup uint constructor
    __IMPLEMENT_FUNCTION_1(ctorUInt, bool, bool, uint);

    __IMPLEMENT_FUNCTION_1(orOperator, operator|, bool, bool);
    __IMPLEMENT_FUNCTION_1(andOperator, operator&, bool, bool);
    __IMPLEMENT_FUNCTION_1(xorOperator, operator^, bool, bool);
    __IMPLEMENT_FUNCTION_1(orAssignOperator, operator|=, bool, bool);
    __IMPLEMENT_FUNCTION_1(andAssignOperator, operator&=, bool, bool);
    __IMPLEMENT_FUNCTION_1(xorAssignOperator, operator^=, bool, bool);
    __IMPLEMENT_FUNCTION_1(ororOperator, operator&&, bool, bool);
    __IMPLEMENT_FUNCTION_1(andandOperator, operator||, bool, bool);

    __IMPLEMENT_FUNCTION_1(eOperator, operator==, bool, bool);
    __IMPLEMENT_FUNCTION_1(neOperator, operator!=, bool, bool);

    __IMPLEMENT_FUNCTION_1(intAssignOperator, operator=, bool, int);
    __IMPLEMENT_FUNCTION_1(uintAssignOperator, operator=, bool, uint);
}

} // namespace GPULang
