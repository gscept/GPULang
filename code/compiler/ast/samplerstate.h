#pragma once
//------------------------------------------------------------------------------
/**
    A sampler state defines a set of sampler settings

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "state.h"
#include "serialize.h"
#include "annotation.h"
#include "attribute.h"

#include <set>
namespace GPULang
{

struct SamplerStateInstance : public State
{
    /// constructor
    SamplerStateInstance();
    /// Destructor
    ~SamplerStateInstance();

    _IMPLEMENT_ANNOTATIONS();
    _IMPLEMENT_ATTRIBUTES();

    struct __Resolved : public State::__Resolved
    {
        virtual ~__Resolved() {};

        Type* typeSymbol = nullptr;
        enum SamplerStateEntryType
        {
            InvalidSamplerStateEntryType,
            AllAddressType,
            AddressUType,
            AddressVType,
            AddressWType,
            AllFilterType,
            MinFilterType,
            MagFilterType,
            MipFilterType,
            MipLodBiasType,
            AnisotropicFlagType,
            MaxAnisotropyType,
            CompareFlagType,
            CompareModeType,
            MinLodType,
            MaxLodType,
            BorderColorType,
            UnnormalizedSamplingType
        };

        /// convert from string to program entry type
        static const SamplerStateEntryType StringToEntryType(const TransientString& str);
        /// convert from mapping to string
        static const ConstantString& EntryTypeToString(const SamplerStateEntryType type);


        /// convert from string to enum
        static const Serialization::BorderColor StringToBorderColor(const TransientString& str);

        /// convert from string to enum
        static const Serialization::Filter StringToFilter(const TransientString& str);

        /// convert from string to enum
        static const Serialization::AddressMode StringToAddressMode(const TransientString& str);

        Serialization::AddressMode addressU = Serialization::AddressMode::RepeatAddressMode;
        Serialization::AddressMode addressV = Serialization::AddressMode::RepeatAddressMode;
        Serialization::AddressMode addressW = Serialization::AddressMode::RepeatAddressMode;

        Serialization::Filter minFilter = Serialization::Filter::LinearFilter;
        Serialization::Filter magFilter = Serialization::Filter::LinearFilter;
        Serialization::Filter mipFilter = Serialization::Filter::LinearFilter;

        float mipLodBias = false;
        bool anisotropicEnabled = false;
        float maxAnisotropy = 16.0f;

        bool compareSamplerEnabled = false;
        Serialization::CompareMode compareMode = Serialization::CompareMode::LessEqualCompare;

        float minLod = 0.0f;
        float maxLod = FLT_MAX;

        Serialization::BorderColor borderColor = Serialization::BorderColor::TransparentBorder;

        bool unnormalizedSamplingEnabled = false;        // sample as if texture was in dimensions [0..width, 0..height] instead of [0..1, 0..1]
        bool isInline = false, isImmutable = false;

        uint8_t group = 0xFF;
        uint8_t binding = 0xFF;

        ShaderUsage visibilityBits;
    };

    bool isInline = false, isImmutable = false;
};

} // namespace GPULang
