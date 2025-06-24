#pragma once
//------------------------------------------------------------------------------
/**
    Integer type

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Int : public Type
{
    /// constructor
    Int();
};
extern Int  IntType;

extern Function Int_ctor_UInt;
extern Function Int_ctor_Bool;
extern Function Int_ctor_Float;

extern Function Int_andOperator;
extern Function Int_orOperator;
extern Function Int_xorOperator;
extern Function Int_leftShiftOperator;
extern Function Int_rightShiftOperator;
extern Function Int_ororOperator;
extern Function Int_andandOperator;
extern Function Int_additionOperator;
extern Function Int_subtractionOperator;
extern Function Int_multiplicationOperator;
extern Function Int_divisionOperator;
extern Function Int_modOperator;

extern Function Int_andAssignOperator;
extern Function Int_orAssignOperator;
extern Function Int_xorAssignOperator;
extern Function Int_leftShiftAssignOperator;
extern Function Int_rightShiftAssignOperator;
extern Function Int_additionAssignOperator;
extern Function Int_subtractionAssignOperator;
extern Function Int_multiplicationAssignOperator;
extern Function Int_divisionAssignOperator;
extern Function Int_moduloAssignOperator;

extern Function Int_ltOperator;
extern Function Int_lteOperator;
extern Function Int_gtOperator;
extern Function Int_gteOperator;
extern Function Int_eOperator;
extern Function Int_neOperator;

extern Function Int_floatAssignOperator;
extern Function Int_uintAssignOperator;
extern Function Int_boolAssignOperator;

} // namespace GPULang
