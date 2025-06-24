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

struct ProgramType : public Type
{
    /// Constructor
    ProgramType();
};
extern ProgramType ProgramTypeType;

extern Variable Program_renderState;
extern Variable Program_vertexShader;
extern Variable Program_hullShader;
extern Variable Program_domainShader;
extern Variable Program_geometryShader;
extern Variable Program_pixelShader;
extern Variable Program_computeShader;

} // namespace GPULang
