#pragma once
//------------------------------------------------------------------------------
/**
    Bool vector of 4

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Bool4 : public Type
{
    ///constructor
    Bool4();
};
extern Bool4 Bool4Type;

extern Function Bool4_ctor_XYZW;
extern Function Bool4_ctorSingleValue;

extern Function Bool4_ororOperator;
extern Function Bool4_andandOperator;
extern Function Bool4_eOperator;
extern Function Bool4_neOperator;

extern Function Bool4_elementAccessOperatorInt;
extern Function Bool4_elementAccessOperatorUInt;

} // namespace GPULang
