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
};
extern SamplerStateType SamplerStateTypeType;

extern Enumeration SamplerState_filterModeEnum;
extern Enumeration SamplerState_addressModeEnum;
extern Enumeration SamplerState_colorEnum;

extern Variable SamplerState_allAddress;
extern Variable SamplerState_addressU;
extern Variable SamplerState_addressV;
extern Variable SamplerState_addressW;

extern Variable SamplerState_allFilter;
extern Variable SamplerState_minFilter;
extern Variable SamplerState_magFilter;
extern Variable SamplerState_mipFilter;

extern Variable SamplerState_mipLodBias;
extern Variable SamplerState_anisotropicEnabled;
extern Variable SamplerState_maxAnisotropy;

extern Variable SamplerState_compareSamplerEnabled;
extern Variable SamplerState_compareMode;

extern Variable SamplerState_minLod;
extern Variable SamplerState_maxLod;

extern Variable SamplerState_borderColor;

extern Variable SamplerState_unnormalizedSamplingEnabled;

} // namespace GPULang
