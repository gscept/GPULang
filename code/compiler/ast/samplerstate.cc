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
SamplerState::SamplerState()
{
    this->symbolType = SamplerStateType;
    if (SYMBOL_STATIC_ALLOC)
        this->resolved = StaticAlloc<SamplerState::__Resolved>();
    else
        this->resolved = Alloc<SamplerState::__Resolved>();
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

//------------------------------------------------------------------------------
/**
*/
SamplerState::~SamplerState()
{
    this->CleanupAnnotations();
    this->CleanupAttributes();
}

const StaticMap samplerEntryTypeLookup =
std::array{
    std::pair{ ConstantString("Address"), SamplerState::__Resolved::AllAddressType }
    , std::pair{ ConstantString("AddressU"), SamplerState::__Resolved::AddressUType }
    , std::pair{ ConstantString("AddressV"), SamplerState::__Resolved::AddressVType }
    , std::pair{ ConstantString("AddressW"), SamplerState::__Resolved::AddressWType }
    , std::pair{ ConstantString("Filter"), SamplerState::__Resolved::AllFilterType }
    , std::pair{ ConstantString("MinFilter"), SamplerState::__Resolved::MinFilterType }
    , std::pair{ ConstantString("MagFilter"), SamplerState::__Resolved::MagFilterType }
    , std::pair{ ConstantString("MipFilter"), SamplerState::__Resolved::MipFilterType }
    , std::pair{ ConstantString("MipLodBias"), SamplerState::__Resolved::MipLodBiasType }
    , std::pair{ ConstantString("AnisotropicEnabled"), SamplerState::__Resolved::AnisotropicFlagType }
    , std::pair{ ConstantString("MaxAnisotropy"), SamplerState::__Resolved::MaxAnisotropyType }
    , std::pair{ ConstantString("CompareEnabled"), SamplerState::__Resolved::CompareFlagType }
    , std::pair{ ConstantString("CompareMode"), SamplerState::__Resolved::CompareModeType }
    , std::pair{ ConstantString("MinLod"), SamplerState::__Resolved::MinLodType }
    , std::pair{ ConstantString("MaxLod"), SamplerState::__Resolved::MaxLodType }
    , std::pair{ ConstantString("Border"), SamplerState::__Resolved::BorderColorType }
    , std::pair{ ConstantString("UnnormalizedCoordinates"), SamplerState::__Resolved::UnnormalizedSamplingType }
};

//------------------------------------------------------------------------------
/**
*/
const SamplerState::__Resolved::SamplerStateEntryType
SamplerState::__Resolved::StringToEntryType(const TransientString& str)
{
    auto it = samplerEntryTypeLookup.Find(str);
    if (it != samplerEntryTypeLookup.end())
        return it->second;
    else
        return SamplerState::__Resolved::SamplerStateEntryType::InvalidSamplerStateEntryType;

}

//------------------------------------------------------------------------------
/**
*/
const ConstantString&
SamplerState::__Resolved::EntryTypeToString(const SamplerStateEntryType type)
{
    static ConstantString def = "";
    for (auto& it : samplerEntryTypeLookup)
    {
        if (it.second == type)
            return it.first;
    }
    return def;
}

const StaticMap stringToAddressMode =
std::array{
    std::pair{ ConstantString("Repeat"), RepeatAddressMode }
    , std::pair{ ConstantString("Mirror"), MirrorAddressMode }
    , std::pair{ ConstantString("Clamp"), ClampAddressMode }
    , std::pair{ ConstantString("Border"), BorderAddressMode }
};

//------------------------------------------------------------------------------
/**
*/
const AddressMode
SamplerState::__Resolved::StringToAddressMode(const TransientString& str)
{
    auto it = stringToAddressMode.Find(str);
    if (it != stringToAddressMode.end())
        return it->second;
    else
        return AddressMode::InvalidAddressMode;
}

const StaticMap stringToFilter =
std::array{
    std::pair{  ConstantString("Linear"), LinearFilter }
    , std::pair{ ConstantString("Point"), PointFilter }
    , std::pair{  ConstantString("Nearest"), PointFilter }
};

//------------------------------------------------------------------------------
/**
*/
const Filter
SamplerState::__Resolved::StringToFilter(const TransientString& str)
{
    auto it = stringToFilter.Find(str);
    if (it != stringToFilter.end())
        return it->second;
    else
        return Filter::InvalidFilter;
}

const StaticMap stringToBorderColor =
std::array{
    std::pair{ "Transparent"_c, TransparentBorder }
    , std::pair{ "Black"_c, BlackBorder }
    , std::pair{ "White"_c, WhiteBorder }
};

//------------------------------------------------------------------------------
/**
*/
const BorderColor 
SamplerState::__Resolved::StringToBorderColor(const TransientString& str)
{
    auto it = stringToBorderColor.Find(str);
    if (it != stringToBorderColor.end())
        return it->second;
    else
        return InvalidBorderColor;
}

} // namespace GPULang
