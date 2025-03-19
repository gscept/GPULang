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

struct SamplerStateType : public Type
{
    /// Constructor
    SamplerStateType();

    Enumeration filterModeEnum;
    Enumeration addressModeEnum;
    Enumeration colorEnum;

    Variable allAddress;
    Variable addressU;
    Variable addressV;
    Variable addressW;

    Variable allFilter;
    Variable minFilter;
    Variable magFilter;
    Variable mipFilter;

    Variable mipLodBias;
    Variable anisotropicEnabled;
    Variable maxAnisotropy;

    Variable compareSamplerEnabled;
    Variable compareMode;

    Variable minLod;
    Variable maxLod;

    Variable borderColor;

    Variable unnormalizedSamplingEnabled;

};

} // namespace GPULang
