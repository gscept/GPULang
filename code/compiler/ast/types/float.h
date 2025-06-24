#pragma once
//------------------------------------------------------------------------------
/**
    Float type

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Float : public Type
{
    Float();
};
extern Float  FloatType;

extern Function Float_ctor_UInt;
extern Function Float_ctor_Int;

extern Function Float_additionOperator;
extern Function Float_subtractionOperator;
extern Function Float_multiplicationOperator;
extern Function Float_divisionOperator;
extern Function Float_modOperator;

extern Function Float_additionAssignOperator;
extern Function Float_subtractionAssignOperator;
extern Function Float_multiplicationAssignOperator;
extern Function Float_divisionAssignOperator;

extern Function Float_ltOperator;
extern Function Float_lteOperator;
extern Function Float_gtOperator;
extern Function Float_gteOperator;
extern Function Float_eOperator;
extern Function Float_neOperator;

extern Function Float_intAssignOperator;
extern Function Float_uintAssignOperator;


} // namespace GPULang
