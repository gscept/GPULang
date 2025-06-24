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
    SYMBOL_STATIC_ALLOC = true;

    __BEGIN_TYPE()
    this->baseType = TypeCode::Int;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(Int_ctor_UInt, i32, i32, u32, Int);
    __IMPLEMENT_CTOR_1(Int_ctor_Bool, i32, i32, b8, Int);
    __IMPLEMENT_CTOR_1(Int_ctor_Float, i32, i32, f32, Int);

    __IMPLEMENT_FUNCTION_1(Int_orOperator, operator|, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_andOperator, operator&, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_xorOperator, operator^, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_leftShiftOperator, operator<<, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_rightShiftOperator, operator>>, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_ororOperator, operator&&, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_andandOperator, operator||, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_additionOperator, operator+, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_subtractionOperator, operator-, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_multiplicationOperator, operator*, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_divisionOperator, operator/, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_modOperator, operator%, i32, i32);

    __IMPLEMENT_FUNCTION_1(Int_orAssignOperator, operator|=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_andAssignOperator, operator&=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_xorAssignOperator, operator^=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_leftShiftAssignOperator, operator<<=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_rightShiftAssignOperator, operator>>=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_additionAssignOperator, operator+=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_subtractionAssignOperator, operator-=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_multiplicationAssignOperator, operator*=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_divisionAssignOperator, operator/=, i32, i32);
    __IMPLEMENT_FUNCTION_1(Int_moduloAssignOperator, operator%=, i32, i32);

    __IMPLEMENT_FUNCTION_1(Int_ltOperator, operator<, b8, i32);
    __IMPLEMENT_FUNCTION_1(Int_lteOperator, operator<=, b8, i32);
    __IMPLEMENT_FUNCTION_1(Int_gtOperator, operator>, b8, i32);
    __IMPLEMENT_FUNCTION_1(Int_gteOperator, operator>=, b8, i32);
    __IMPLEMENT_FUNCTION_1(Int_eOperator, operator==, b8, i32);
    __IMPLEMENT_FUNCTION_1(Int_neOperator, operator!=, b8, i32);

    __IMPLEMENT_FUNCTION_1(Int_floatAssignOperator, operator=, i32, f32);
    __IMPLEMENT_FUNCTION_1(Int_uintAssignOperator, operator=, i32, u32);
    __IMPLEMENT_FUNCTION_1(Int_boolAssignOperator, operator=, i32, b8);

    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
