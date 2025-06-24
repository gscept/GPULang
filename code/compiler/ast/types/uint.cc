//------------------------------------------------------------------------------
//  @file uint.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
UInt::UInt()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_TYPE()
    this->baseType = TypeCode::UInt;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(UInt_ctor_Int, u32, u32, i32, UInt);
    __IMPLEMENT_CTOR_1(UInt_ctor_Bool, u32, u32, b8, UInt);
    __IMPLEMENT_CTOR_1(UInt_ctor_Float, u32, u32, f32, UInt);

    __IMPLEMENT_FUNCTION_1(UInt_orOperator, operator|, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_andOperator, operator&, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_xorOperator, operator^, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_leftShiftOperator, operator<<, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_rightShiftOperator, operator>>, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_ororOperator, operator&&, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_andandOperator, operator||, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_additionOperator, operator+, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_subtractionOperator, operator-, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_multiplicationOperator, operator*, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_divisionOperator, operator/, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_modOperator, operator%, u32, u32);

    __IMPLEMENT_FUNCTION_1(UInt_orAssignOperator, operator|=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_andAssignOperator, operator&=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_xorAssignOperator, operator^=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_leftShiftAssignOperator, operator<<=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_rightShiftAssignOperator, operator>>=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_additionAssignOperator, operator+=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_subtractionAssignOperator, operator-=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_multiplicationAssignOperator, operator*=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_divisionAssignOperator, operator/=, u32, u32);
    __IMPLEMENT_FUNCTION_1(UInt_moduloAssignOperator, operator%=, u32, u32);

    __IMPLEMENT_FUNCTION_1(UInt_ltOperator, operator<, b8, u32);
    __IMPLEMENT_FUNCTION_1(UInt_lteOperator, operator<=, b8, u32);
    __IMPLEMENT_FUNCTION_1(UInt_gtOperator, operator>, b8, u32);
    __IMPLEMENT_FUNCTION_1(UInt_gteOperator, operator>=, b8, u32);
    __IMPLEMENT_FUNCTION_1(UInt_eOperator, operator==, b8, u32);
    __IMPLEMENT_FUNCTION_1(UInt_neOperator, operator!=, b8, i32);

    __IMPLEMENT_FUNCTION_1(UInt_floatAssignOperator, operator=, u32, f32);
    __IMPLEMENT_FUNCTION_1(UInt_intAssignOperator, operator=, u32, i32);
    __IMPLEMENT_FUNCTION_1(UInt_boolAssignOperator, operator=, u32, b8);
    SYMBOL_STATIC_ALLOC = false;

}

} // namespace GPULang
