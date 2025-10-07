//------------------------------------------------------------------------------
//  @file samplerstate.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "samplerstate.h"
#include "variable.h"
#include <map>
#include <float.h>
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
SamplerStateInstance::SamplerStateInstance()
{
    this->symbolType = SamplerStateInstanceType;    
    this->resolved = Alloc<SamplerStateInstance::__Resolved>();
    this->isInline = false;
    this->isImmutable = false;

    SamplerStateInstance::__Resolved* typeResolved = static_cast<SamplerStateInstance::__Resolved*>(this->resolved);
    typeResolved->group = Variable::__Resolved::NOT_BOUND;
    typeResolved->binding = Variable::__Resolved::NOT_BOUND;
    typeResolved->addressU = typeResolved->addressV = typeResolved->addressW = Serialization::AddressMode::RepeatAddressMode;
    typeResolved->minFilter = typeResolved->magFilter = typeResolved->mipFilter = Serialization::Filter::LinearFilter;
    typeResolved->mipLodBias = 0.0f;
    typeResolved->anisotropicEnabled = false;
    typeResolved->maxAnisotropy = 16;   // in case we turn on aniso, make highest quality automatically
    typeResolved->compareSamplerEnabled = false;
    typeResolved->compareMode = Serialization::CompareMode::LessEqualCompare;
    typeResolved->minLod = 0;
    typeResolved->maxLod = FLT_MAX;
    typeResolved->borderColor = Serialization::BorderColor::BlackBorder;
    typeResolved->unnormalizedSamplingEnabled = false;
}

//------------------------------------------------------------------------------
/**
*/
SamplerStateInstance::~SamplerStateInstance()
{
    this->CleanupAnnotations();
    this->CleanupAttributes();
}

constexpr StaticMap samplerEntryTypeLookup =
std::array{
    std::pair{ ConstantString("Address"), SamplerStateInstance::__Resolved::AllAddressType }
    , std::pair{ ConstantString("AddressU"), SamplerStateInstance::__Resolved::AddressUType }
    , std::pair{ ConstantString("AddressV"), SamplerStateInstance::__Resolved::AddressVType }
    , std::pair{ ConstantString("AddressW"), SamplerStateInstance::__Resolved::AddressWType }
    , std::pair{ ConstantString("Filter"), SamplerStateInstance::__Resolved::AllFilterType }
    , std::pair{ ConstantString("MinFilter"), SamplerStateInstance::__Resolved::MinFilterType }
    , std::pair{ ConstantString("MagFilter"), SamplerStateInstance::__Resolved::MagFilterType }
    , std::pair{ ConstantString("MipFilter"), SamplerStateInstance::__Resolved::MipFilterType }
    , std::pair{ ConstantString("MipLodBias"), SamplerStateInstance::__Resolved::MipLodBiasType }
    , std::pair{ ConstantString("AnisotropicEnabled"), SamplerStateInstance::__Resolved::AnisotropicFlagType }
    , std::pair{ ConstantString("MaxAnisotropy"), SamplerStateInstance::__Resolved::MaxAnisotropyType }
    , std::pair{ ConstantString("CompareEnabled"), SamplerStateInstance::__Resolved::CompareFlagType }
    , std::pair{ ConstantString("CompareMode"), SamplerStateInstance::__Resolved::CompareModeType }
    , std::pair{ ConstantString("MinLod"), SamplerStateInstance::__Resolved::MinLodType }
    , std::pair{ ConstantString("MaxLod"), SamplerStateInstance::__Resolved::MaxLodType }
    , std::pair{ ConstantString("Border"), SamplerStateInstance::__Resolved::BorderColorType }
    , std::pair{ ConstantString("UnnormalizedCoordinates"), SamplerStateInstance::__Resolved::UnnormalizedSamplingType }
};

//------------------------------------------------------------------------------
/**
*/
const SamplerStateInstance::__Resolved::SamplerStateEntryType
SamplerStateInstance::__Resolved::StringToEntryType(const TransientString& str)
{
    auto it = samplerEntryTypeLookup.Find(str);
    if (it != samplerEntryTypeLookup.end())
        return it->second;
    else
        return SamplerStateInstance::__Resolved::SamplerStateEntryType::InvalidSamplerStateEntryType;

}

//------------------------------------------------------------------------------
/**
*/
const ConstantString&
SamplerStateInstance::__Resolved::EntryTypeToString(const SamplerStateEntryType type)
{
    static ConstantString def = "";
    for (auto& it : samplerEntryTypeLookup)
    {
        if (it.second == type)
            return it.first;
    }
    return def;
}

} // namespace GPULang
