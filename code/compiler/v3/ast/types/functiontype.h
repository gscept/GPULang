#pragma once
//------------------------------------------------------------------------------
/**
    Sampler State Type

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/enumeration.h"
namespace GPULang
{

struct FunctionType : public Type
{
    /// Constructor
    FunctionType();
};

} // namespace GPULang
