#pragma once
//------------------------------------------------------------------------------
/**
    Bool vector of 2

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Bool2 : public Type
{
    /// constructor
    Bool2();
};
extern Bool2 Bool2Type;

extern Function Bool2_ctor_XY;
extern Function Bool2_ctorSingleValue;

extern Function Bool2_ororOperator;
extern Function Bool2_andandOperator;
extern Function Bool2_eOperator;
extern Function Bool2_neOperator;

extern Function Bool2_elementAccessOperatorInt;
extern Function Bool2_elementAccessOperatorUInt;

} // namespace GPULang
