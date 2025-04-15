//------------------------------------------------------------------------------
//  @file samplerstate.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "samplerstate.h"
#include "variable.h"
#include <map>
namespace GPULang
{
SamplerState::SamplerState()
{
    this->symbolType = SamplerStateType;
    this->resolved = new SamplerState::__Resolved();
    this->isInline = false;
    this->isImmutable = false;

    SamplerState::__Resolved* typeResolved = static_cast<SamplerState::__Resolved*>(this->resolved);
    typeResolved->group = Variable::__Resolved::NOT_BOUND;
    typeResolved->binding = Variable::__Resolved::NOT_BOUND;
    typeResolved->addressU = typeResolved->addressV = typeResolved->addressW = RepeatAddressMode;
    typeResolved->minFilter = typeResolved->magFilter = typeResolved->mipFilter = LinearFilter;
    typeResolved->mipLodBias = 0.0f;
    typeResolved->anisotropicEnabled = false;
    typeResolved->maxAnisotropy = 16;   // in case we turn on aniso, make highest quality automatically
    typeResolved->compareSamplerEnabled = false;
    typeResolved->compareMode = LessEqualCompare;
    typeResolved->minLod = 0;
    typeResolved->maxLod = FLT_MAX;
    typeResolved->borderColor = BlackBorder;
    typeResolved->unnormalizedSamplingEnabled = false;
}

const std::map<std::string, SamplerState::__Resolved::SamplerStateEntryType> samplerEntryTypeLookup =
{
    { "Address", SamplerState::__Resolved::AllAddressType }
    , { "AddressU", SamplerState::__Resolved::AddressUType }
    , { "AddressV", SamplerState::__Resolved::AddressVType }
    , { "AddressW", SamplerState::__Resolved::AddressWType }
    , { "Filter", SamplerState::__Resolved::AllFilterType }
    , { "MinFilter", SamplerState::__Resolved::MinFilterType }
    , { "MagFilter", SamplerState::__Resolved::MagFilterType }
    , { "MipFilter", SamplerState::__Resolved::MipFilterType }
    , { "MipLodBias", SamplerState::__Resolved::MipLodBiasType }
    , { "AnisotropicEnabled", SamplerState::__Resolved::AnisotropicFlagType }
    , { "MaxAnisotropy", SamplerState::__Resolved::MaxAnisotropyType }
    , { "CompareEnabled", SamplerState::__Resolved::CompareFlagType }
    , { "CompareMode", SamplerState::__Resolved::CompareModeType }
    , { "MinLod", SamplerState::__Resolved::MinLodType }
    , { "MaxLod", SamplerState::__Resolved::MaxLodType }
    , { "Border", SamplerState::__Resolved::BorderColorType }
    , { "UnnormalizedCoordinates", SamplerState::__Resolved::UnnormalizedSamplingType }
};

//------------------------------------------------------------------------------
/**
*/
const SamplerState::__Resolved::SamplerStateEntryType
SamplerState::__Resolved::StringToEntryType(const std::string& str)
{
    auto it = samplerEntryTypeLookup.find(str);
    if (it != samplerEntryTypeLookup.end())
        return it->second;
    else
        return SamplerState::__Resolved::SamplerStateEntryType::InvalidSamplerStateEntryType;

}

//------------------------------------------------------------------------------
/**
*/
const std::string&
SamplerState::__Resolved::EntryTypeToString(const SamplerStateEntryType type)
{
    static std::string def = "";
    for (auto& it : samplerEntryTypeLookup)
    {
        if (it.second == type)
            return it.first;
    }
    return def;
}

const std::map<std::string, AddressMode> stringToAddressMode =
{
    { "Repeat", RepeatAddressMode }
    , { "Mirror", MirrorAddressMode }
    , { "Clamp", ClampAddressMode }
    , { "Border", BorderAddressMode }
};

//------------------------------------------------------------------------------
/**
*/
const AddressMode
SamplerState::__Resolved::StringToAddressMode(const std::string& str)
{
    auto it = stringToAddressMode.find(str);
    if (it != stringToAddressMode.end())
        return it->second;
    else
        return AddressMode::InvalidAddressMode;
}

const std::map<std::string, Filter> stringToFilter =
{
    { "Linear", LinearFilter }
    , { "Point", PointFilter }
    , { "Nearest", PointFilter }
};

//------------------------------------------------------------------------------
/**
*/
const Filter
SamplerState::__Resolved::StringToFilter(const std::string& str)
{
    auto it = stringToFilter.find(str);
    if (it != stringToFilter.end())
        return it->second;
    else
        return Filter::InvalidFilter;
}

const std::map<std::string, BorderColor> stringToBorderColor =
{
    { "Transparent", TransparentBorder }
    , { "Black", BlackBorder }
    , { "White", WhiteBorder }
};

//------------------------------------------------------------------------------
/**
*/
const BorderColor 
SamplerState::__Resolved::StringToBorderColor(const std::string& str)
{
    auto it = stringToBorderColor.find(str);
    if (it != stringToBorderColor.end())
        return it->second;
    else
        return InvalidBorderColor;
}

} // namespace GPULang
