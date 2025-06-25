//------------------------------------------------------------------------------
//  @file int.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "int.h"
#include "builtins.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Int::Int()
{
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE();
    this->name = ConstantString("i32");
    this->baseType = TypeCode::Int;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(Int_ctor_UInt, i32, Int, UInt);
    __IMPLEMENT_CTOR_1(Int_ctor_Bool, i32, Int, Bool);
    __IMPLEMENT_CTOR_1(Int_ctor_Float, i32, Int, Float);

    __IMPLEMENT_FUNCTION_1(Int_orOperator, operator|, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_andOperator, operator&, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_xorOperator, operator^, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_leftShiftOperator, operator<<, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_rightShiftOperator, operator>>, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_ororOperator, operator&&, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_andandOperator, operator||, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_additionOperator, operator+, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_subtractionOperator, operator-, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_multiplicationOperator, operator*, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_divisionOperator, operator/, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_modOperator, operator%, Int, Int);

    __IMPLEMENT_FUNCTION_1(Int_orAssignOperator, operator|=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_andAssignOperator, operator&=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_xorAssignOperator, operator^=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_leftShiftAssignOperator, operator<<=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_rightShiftAssignOperator, operator>>=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_additionAssignOperator, operator+=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_subtractionAssignOperator, operator-=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_multiplicationAssignOperator, operator*=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_divisionAssignOperator, operator/=, Int, Int);
    __IMPLEMENT_FUNCTION_1(Int_moduloAssignOperator, operator%=, Int, Int);

    __IMPLEMENT_FUNCTION_1(Int_ltOperator, operator<, Bool, Int);
    __IMPLEMENT_FUNCTION_1(Int_lteOperator, operator<=, Bool, Int);
    __IMPLEMENT_FUNCTION_1(Int_gtOperator, operator>, Bool, Int);
    __IMPLEMENT_FUNCTION_1(Int_gteOperator, operator>=, Bool, Int);
    __IMPLEMENT_FUNCTION_1(Int_eOperator, operator==, Bool, Int);
    __IMPLEMENT_FUNCTION_1(Int_neOperator, operator!=, Bool, Int);

    __IMPLEMENT_FUNCTION_1(Int_floatAssignOperator, operator=, Int, Float);
    __IMPLEMENT_FUNCTION_1(Int_uintAssignOperator, operator=, Int, UInt);
    __IMPLEMENT_FUNCTION_1(Int_boolAssignOperator, operator=, Int, Bool);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
