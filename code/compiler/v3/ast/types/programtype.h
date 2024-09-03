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
namespace AnyFX
{

struct ProgramType : public Type
{
    /// Constructor
    ProgramType();

    Variable renderState;
    Variable vertexShader;
    Variable hullShader;
    Variable domainShader;
    Variable geometryShader;
    Variable pixelShader;
    Variable computeShader;
};

} // namespace AnyFX
