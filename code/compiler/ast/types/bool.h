#pragma once
//------------------------------------------------------------------------------
/**
    Bool type

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Bool : public Type
{
    /// constructor
    Bool();
};
extern Bool  BoolType;

extern Function Bool_ctor_UInt;
extern Function Bool_ctor_Int;

extern Function Bool_ororOperator;
extern Function Bool_andandOperator;
extern Function Bool_eOperator;
extern Function Bool_neOperator;

extern Function Bool_boolAssignOperator;
extern Function Bool_intAssignOperator;
extern Function Bool_uintAssignOperator;

} // namespace GPULang
