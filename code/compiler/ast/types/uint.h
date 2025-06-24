#pragma once
//------------------------------------------------------------------------------
/**
    Unsigned integer

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct UInt : public Type
{
    /// constructor
    UInt();
};

extern UInt UIntType;
extern Function UInt_ctor_Int;
extern Function UInt_ctor_Bool;
extern Function UInt_ctor_Float;

extern Function UInt_andOperator;
extern Function UInt_orOperator;
extern Function UInt_xorOperator;
extern Function UInt_leftShiftOperator;
extern Function UInt_rightShiftOperator;
extern Function UInt_ororOperator;
extern Function UInt_andandOperator;
extern Function UInt_additionOperator;
extern Function UInt_subtractionOperator;
extern Function UInt_multiplicationOperator;
extern Function UInt_divisionOperator;
extern Function UInt_modOperator;

extern Function UInt_andAssignOperator;
extern Function UInt_orAssignOperator;
extern Function UInt_xorAssignOperator;
extern Function UInt_leftShiftAssignOperator;
extern Function UInt_rightShiftAssignOperator;
extern Function UInt_additionAssignOperator;
extern Function UInt_subtractionAssignOperator;
extern Function UInt_multiplicationAssignOperator;
extern Function UInt_divisionAssignOperator;
extern Function UInt_moduloAssignOperator;

extern Function UInt_ltOperator;
extern Function UInt_lteOperator;
extern Function UInt_gtOperator;
extern Function UInt_gteOperator;
extern Function UInt_eOperator;
extern Function UInt_neOperator;

extern Function UInt_floatAssignOperator;
extern Function UInt_intAssignOperator;
extern Function UInt_boolAssignOperator;

} // namespace GPULang
