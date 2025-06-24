#pragma once
//------------------------------------------------------------------------------
/**
    Bool vector of 3

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Bool3 : public Type
{
    /// constructor
    Bool3();
};

extern Bool3 Bool3Type;

extern Function Bool3_ctor_XYZ;
extern Function Bool3_ctorSingleValue;

extern Function Bool3_ororOperator;
extern Function Bool3_andandOperator;
extern Function Bool3_eOperator;
extern Function Bool3_neOperator;

extern Function Bool3_elementAccessOperatorInt;
extern Function Bool3_elementAccessOperatorUInt;

} // namespace GPULang
