//------------------------------------------------------------------------------
//  @file uint.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "uint.h"
#include "builtins.h"
namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
UInt::UInt()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_TYPE()
    this->name = ConstantString("u32");
    this->baseType = TypeCode::UInt;
    this->columnSize = 1;
    this->rowSize = 1;
    this->byteSize = 4;
    this->category = Type::ScalarCategory;

    __IMPLEMENT_CTOR_1(UInt_ctor_Int, u32, UInt, Int);
    __IMPLEMENT_CTOR_1(UInt_ctor_Bool, u32, UInt, Bool);
    __IMPLEMENT_CTOR_1(UInt_ctor_Float, u32, UInt, Float);

    __IMPLEMENT_FUNCTION_1(UInt_orOperator, operator|, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_andOperator, operator&, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_xorOperator, operator^, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_leftShiftOperator, operator<<, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_rightShiftOperator, operator>>, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_ororOperator, operator&&, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_andandOperator, operator||, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_additionOperator, operator+, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_subtractionOperator, operator-, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_multiplicationOperator, operator*, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_divisionOperator, operator/, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_modOperator, operator%, UInt, UInt);

    __IMPLEMENT_FUNCTION_1(UInt_orAssignOperator, operator|=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_andAssignOperator, operator&=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_xorAssignOperator, operator^=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_leftShiftAssignOperator, operator<<=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_rightShiftAssignOperator, operator>>=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_additionAssignOperator, operator+=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_subtractionAssignOperator, operator-=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_multiplicationAssignOperator, operator*=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_divisionAssignOperator, operator/=, UInt, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_moduloAssignOperator, operator%=, UInt, UInt);

    __IMPLEMENT_FUNCTION_1(UInt_ltOperator, operator<, Bool, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_lteOperator, operator<=, Bool, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_gtOperator, operator>, Bool, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_gteOperator, operator>=, Bool, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_eOperator, operator==, Bool, UInt);
    __IMPLEMENT_FUNCTION_1(UInt_neOperator, operator!=, Bool, UInt);

    __IMPLEMENT_FUNCTION_1(UInt_floatAssignOperator, operator=, UInt, Float);
    __IMPLEMENT_FUNCTION_1(UInt_intAssignOperator, operator=, UInt, Int);
    __IMPLEMENT_FUNCTION_1(UInt_boolAssignOperator, operator=, UInt, Bool);
    __END_TYPE()
    SYMBOL_STATIC_ALLOC = false;
}

} // namespace GPULang
