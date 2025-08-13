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

constexpr StaticMap stringToAddressMode =
std::array{
    std::pair{ ConstantString("Repeat"), Serialization::AddressMode::RepeatAddressMode }
    , std::pair{ ConstantString("Mirror"), Serialization::AddressMode::MirrorAddressMode }
    , std::pair{ ConstantString("Clamp"), Serialization::AddressMode::ClampAddressMode }
    , std::pair{ ConstantString("Border"), Serialization::AddressMode::BorderAddressMode }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::AddressMode
SamplerStateInstance::__Resolved::StringToAddressMode(const TransientString& str)
{
    auto it = stringToAddressMode.Find(str);
    if (it != stringToAddressMode.end())
        return it->second;
    else
        return Serialization::AddressMode::InvalidAddressMode;
}

constexpr StaticMap stringToFilter =
std::array{
    std::pair{  ConstantString("Linear"), Serialization::Filter::LinearFilter }
    , std::pair{ ConstantString("Point"), Serialization::Filter::PointFilter }
    , std::pair{  ConstantString("Nearest"), Serialization::Filter::PointFilter }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::Filter
SamplerStateInstance::__Resolved::StringToFilter(const TransientString& str)
{
    auto it = stringToFilter.Find(str);
    if (it != stringToFilter.end())
        return it->second;
    else
        return Serialization::Filter::InvalidFilter;
}

constexpr StaticMap stringToBorderColor =
std::array{
    std::pair{ "Transparent"_c, Serialization::BorderColor::TransparentBorder }
    , std::pair{ "Black"_c, Serialization::BorderColor::BlackBorder }
    , std::pair{ "White"_c, Serialization::BorderColor::WhiteBorder }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::BorderColor
SamplerStateInstance::__Resolved::StringToBorderColor(const TransientString& str)
{
    auto it = stringToBorderColor.Find(str);
    if (it != stringToBorderColor.end())
        return it->second;
    else
        return Serialization::BorderColor::InvalidBorderColor;
}

} // namespace GPULang
