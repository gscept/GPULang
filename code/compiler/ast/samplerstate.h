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

        Type* typeSymbol;
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

        Serialization::AddressMode addressU;
        Serialization::AddressMode addressV;
        Serialization::AddressMode addressW;

        Serialization::Filter minFilter;
        Serialization::Filter magFilter;
        Serialization::Filter mipFilter;

        float mipLodBias;
        bool anisotropicEnabled;
        float maxAnisotropy;

        bool compareSamplerEnabled;
        Serialization::CompareMode compareMode;

        float minLod;
        float maxLod;

        Serialization::BorderColor borderColor;

        bool unnormalizedSamplingEnabled;        // sample as if texture was in dimensions [0..width, 0..height] instead of [0..1, 0..1]
        bool isInline, isImmutable;

        uint32_t group;
        uint32_t binding;

        ShaderUsage visibilityBits;
    };

    bool isInline, isImmutable;
};

} // namespace GPULang
