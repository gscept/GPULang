//------------------------------------------------------------------------------
//  @file renderstate.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "renderstate.h"
#include <map>
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
RenderStateInstance::RenderStateInstance()
{
    this->symbolType = RenderStateInstanceType;    
    this->resolved = Alloc<RenderStateInstance::__Resolved>();
    RenderStateInstance::__Resolved* typeResolved = static_cast<RenderStateInstance::__Resolved*>(this->resolved);
    typeResolved->depthClampEnabled = false;
    typeResolved->noPixels = false;
    typeResolved->rasterizationMode = Serialization::RasterizationMode::FillMode;
    typeResolved->cullMode = Serialization::CullMode::BackMode;
    typeResolved->windingOrderMode = Serialization::WindingOrderMode::CounterClockwiseMode;
    typeResolved->depthBiasEnabled = false;
    typeResolved->depthBiasFactor = 0.0f;
    typeResolved->depthBiasClamp = 0.0f;
    typeResolved->depthBiasSlopeFactor = 0.0f;
    typeResolved->lineWidth = 1.0f;

    typeResolved->depthTestEnabled = true;
    typeResolved->depthWriteEnabled = true;
    typeResolved->depthCompare = Serialization::CompareMode::LessEqualCompare;
    typeResolved->depthBoundsTestEnabled = true;
    typeResolved->minDepthBounds = 0.0f;
    typeResolved->maxDepthBounds = 1.0f;
    typeResolved->scissorEnabled = false;
    typeResolved->stencilEnabled = false;
    typeResolved->logicOpEnabled = false;
    typeResolved->logicOp = Serialization::LogicOp::LogicSetOp;
    Serialization::BlendState defaultBlend =
    {
        .blendEnabled = false,
        .sourceColorBlendFactor = Serialization::BlendFactor::OneFactor,
        .destinationColorBlendFactor = Serialization::BlendFactor::OneFactor,
        .sourceAlphaBlendFactor = Serialization::BlendFactor::OneFactor,
        .destinationAlphaBlendFactor = Serialization::BlendFactor::OneFactor,
        .colorBlendOp = Serialization::BlendOp::AddOp,
        .alphaBlendOp = Serialization::BlendOp::AddOp,
        .colorComponentMask = 0xFFFFFFFF
    };
    typeResolved->blendStates[0] = defaultBlend;
    typeResolved->blendStates[1] = defaultBlend;
    typeResolved->blendStates[2] = defaultBlend;
    typeResolved->blendStates[3] = defaultBlend;
    typeResolved->blendStates[4] = defaultBlend;
    typeResolved->blendStates[5] = defaultBlend;
    typeResolved->blendStates[6] = defaultBlend;
    typeResolved->blendStates[7] = defaultBlend;

    Serialization::StencilState defaultStencil =
    {
        .fail = Serialization::StencilOp::StencilKeepOp,
        .pass = Serialization::StencilOp::StencilKeepOp,
        .depthFail = Serialization::StencilOp::StencilKeepOp,
        .compare = Serialization::CompareMode::EqualCompare,
        .compareMask = 0xFFFFFFFF,
        .writeMask = 0xFFFFFFFF,
        .referenceMask = 0xFFFFFFFF
    };
    typeResolved->frontStencilState = defaultStencil;
    typeResolved->backStencilState = defaultStencil;
}

constexpr StaticMap stringToRenderStateEntryType =
std::array{
    std::pair{ "DepthClampEnabled"_c, RenderStateInstance::__Resolved::DepthClampEnabledType },
    std::pair{ "NoPixels"_c, RenderStateInstance::__Resolved::NoPixelsType },
    std::pair{ "Rasterize"_c, RenderStateInstance::__Resolved::RasterizationModeType },
    std::pair{ "Cull"_c, RenderStateInstance::__Resolved::CullModeType },
    std::pair{ "WindingOrder"_c, RenderStateInstance::__Resolved::WindingOrderType },
    std::pair{ "DepthBiasEnabled"_c, RenderStateInstance::__Resolved::DepthBiasEnabledType },
    std::pair{ "DepthBiasFactor"_c, RenderStateInstance::__Resolved::DepthBiasFactorType },
    std::pair{ "DepthBiasClamp"_c, RenderStateInstance::__Resolved::DepthBiasClampType },
    std::pair{ "DepthBiasSlopeFactor"_c, RenderStateInstance::__Resolved::DepthBiasSlopeFactorType },
    std::pair{ "LineWidth"_c, RenderStateInstance::__Resolved::LineWidthType },
    std::pair{ "DepthTestEnabled"_c, RenderStateInstance::__Resolved::DepthTestEnabledType },
    std::pair{ "DepthWriteEnabled"_c, RenderStateInstance::__Resolved::DepthWriteEnabledType },
    std::pair{ "DepthTestFunction"_c, RenderStateInstance::__Resolved::DepthTestFunction },
    std::pair{ "DepthBoundsTestEnabled"_c, RenderStateInstance::__Resolved::DepthBoundsTestEnabledType },
    std::pair{ "MinDepthBounds"_c, RenderStateInstance::__Resolved::MinDepthBoundsType },
    std::pair{ "MaxDepthBounds"_c, RenderStateInstance::__Resolved::MaxDepthBoundsType },
    std::pair{ "ScissorEnabled"_c, RenderStateInstance::__Resolved::ScissorEnabledType },
    std::pair{ "StencilEnabled"_c, RenderStateInstance::__Resolved::StencilEnabledType },
    std::pair{ "StencilFront.Fail"_c, RenderStateInstance::__Resolved::StencilFailOpType },
    std::pair{ "StencilFront.Pass"_c, RenderStateInstance::__Resolved::StencilPassOpType },
    std::pair{ "StencilFront.DepthFail"_c, RenderStateInstance::__Resolved::StencilDepthFailOpType },
    std::pair{ "StencilFront.Compare"_c, RenderStateInstance::__Resolved::StencilCompareModeType },
    std::pair{ "StencilFront.CompareMask"_c, RenderStateInstance::__Resolved::StencilCompareMaskType },
    std::pair{ "StencilFront.WriteMask"_c, RenderStateInstance::__Resolved::StencilWriteMaskType },
    std::pair{ "StencilFront.ReferenceMask"_c, RenderStateInstance::__Resolved::StencilReferenceMaskType },
    std::pair{ "StencilBack.Fail"_c, RenderStateInstance::__Resolved::StencilFailOpType },
    std::pair{ "StencilBack.Pass"_c, RenderStateInstance::__Resolved::StencilPassOpType },
    std::pair{ "StencilBack.DepthFail"_c, RenderStateInstance::__Resolved::StencilDepthFailOpType },
    std::pair{ "StencilBack.Compare"_c, RenderStateInstance::__Resolved::StencilCompareModeType },
    std::pair{ "StencilBack.CompareMask"_c, RenderStateInstance::__Resolved::StencilCompareMaskType },
    std::pair{ "StencilBack.WriteMask"_c, RenderStateInstance::__Resolved::StencilWriteMaskType },
    std::pair{ "StencilBack.ReferenceMask"_c, RenderStateInstance::__Resolved::StencilReferenceMaskType },
    std::pair{ "LogicOpEnabled"_c, RenderStateInstance::__Resolved::LogicOpEnabledType },
    std::pair{ "LogicOp"_c, RenderStateInstance::__Resolved::LogicOpType },
    std::pair{ "BlendEnabled"_c, RenderStateInstance::__Resolved::BlendEnabledType },
    std::pair{ "SrcBlend"_c, RenderStateInstance::__Resolved::SourceBlendColorFactorType },
    std::pair{ "SourceBlend"_c, RenderStateInstance::__Resolved::SourceBlendColorFactorType },
    std::pair{ "DstBlend"_c, RenderStateInstance::__Resolved::DestinationBlendColorFactorType },
    std::pair{ "DestinationBlend"_c, RenderStateInstance::__Resolved::DestinationBlendColorFactorType },
    std::pair{ "SrcAlphaBlend"_c, RenderStateInstance::__Resolved::SourceBlendAlphaFactorType },
    std::pair{ "SourceAlphaBlend"_c, RenderStateInstance::__Resolved::SourceBlendAlphaFactorType },
    std::pair{ "DstAlphaBlend"_c, RenderStateInstance::__Resolved::DestinationBlendAlphaFactorType },
    std::pair{ "DestinationAlphaBlend"_c, RenderStateInstance::__Resolved::DestinationBlendAlphaFactorType },
    std::pair{ "BlendOp"_c, RenderStateInstance::__Resolved::ColorBlendOpType },
    std::pair{ "BlendOpAlpha"_c, RenderStateInstance::__Resolved::AlphaBlendOpType },
    std::pair{ "ColorComponentMask"_c, RenderStateInstance::__Resolved::ColorComponentMaskType },
    std::pair{ "BlendConstants"_c, RenderStateInstance::__Resolved::BlendConstantsType }
};

static ConstantString NoRenderStateEntry = "";

//------------------------------------------------------------------------------
/**
*/
const RenderStateInstance::__Resolved::RenderStateEntryType
RenderStateInstance::__Resolved::StringToEntryType(const TransientString& str)
{
    auto it = stringToRenderStateEntryType.Find(str);
    if (it != stringToRenderStateEntryType.end())
        return it->second;
    else
        return RenderStateInstance::__Resolved::RenderStateEntryType::InvalidRenderStateEntryType;

}

//------------------------------------------------------------------------------
/**
*/
const ConstantString&
RenderStateInstance::__Resolved::EntryTypeToString(const RenderStateEntryType type)
{
    for (auto& it : stringToRenderStateEntryType)
    {
        if (it.second == type)
            return it.first;
    }
    return NoRenderStateEntry;
}

constexpr StaticMap stringToPolygonMode =
std::array{
    std::pair{ "Fill"_c , Serialization::RasterizationMode::FillMode },
    std::pair{ "Line"_c , Serialization::RasterizationMode::LineMode },
    std::pair{ "Point"_c, Serialization::RasterizationMode::PointMode }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::RasterizationMode
RenderStateInstance::__Resolved::StringToPolygonMode(const TransientString& str)
{
    auto it = stringToPolygonMode.Find(str);
    if (it != stringToPolygonMode.end())
        return it->second;
    else
        return Serialization::RasterizationMode::InvalidRasterizationMode;
}

constexpr StaticMap stringToCullMode =
std::array{
    std::pair{ "None"_c, Serialization::CullMode::NoCullMode },
    std::pair{ "Front"_c, Serialization::CullMode::FrontMode },
    std::pair{ "Back"_c, Serialization::CullMode::BackMode },
    std::pair{ "FrontAndBack"_c, Serialization::CullMode::FrontAndBackMode }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::CullMode
RenderStateInstance::__Resolved::StringToCullMode(const TransientString& str)
{
    auto it = stringToCullMode.Find(str);
    if (it != stringToCullMode.end())
        return it->second;
    else
        return Serialization::CullMode::InvalidCullMode;
}

constexpr StaticMap stringToWindingOrderMode =
std::array{
    std::pair{ "Clockwise"_c, Serialization::WindingOrderMode::ClockwiseMode },
    std::pair{ "CounterClockwise"_c, Serialization::WindingOrderMode::CounterClockwiseMode }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::WindingOrderMode
RenderStateInstance::__Resolved::StringToWindingOrderMode(const TransientString& str)
{
    auto it = stringToWindingOrderMode.Find(str);
    if (it != stringToWindingOrderMode.end())
        return it->second;
    else
        return Serialization::WindingOrderMode::InvalidWindingOrderMode;
}

constexpr StaticMap stringToLogicOp =
std::array{
    std::pair{ "Clear"_c, Serialization::LogicOp::LogicClearOp },
    std::pair{ "And"_c, Serialization::LogicOp::LogicAndOp },
    std::pair{ "AndReverse"_c, Serialization::LogicOp::LogicAndReverseOp },
    std::pair{ "Copy"_c, Serialization::LogicOp::LogicCopyOp },
    std::pair{ "AndInverted"_c, Serialization::LogicOp::LogicAndInvertedOp },
    std::pair{ "No"_c, Serialization::LogicOp::LogicNoOp },
    std::pair{ "Xor"_c, Serialization::LogicOp::LogicXorOp },
    std::pair{ "Or"_c, Serialization::LogicOp::LogicOrOp },
    std::pair{ "Nor"_c, Serialization::LogicOp::LogicNorOp },
    std::pair{ "Equivalent"_c, Serialization::LogicOp::LogicEquivalentOp },
    std::pair{ "Invert"_c, Serialization::LogicOp::LogicInvertOp },
    std::pair{ "OrReverse"_c, Serialization::LogicOp::LogicOrReverseOp },
    std::pair{ "CopyInverted"_c, Serialization::LogicOp::LogicCopyInvertedOp },
    std::pair{ "OrInverted"_c, Serialization::LogicOp::LogicOrInvertedOp },
    std::pair{ "Nand"_c, Serialization::LogicOp::LogicNandOp },
    std::pair{ "Set"_c, Serialization::LogicOp::LogicSetOp }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::LogicOp
RenderStateInstance::__Resolved::StringToLogicOp(const TransientString& str)
{
    auto it = stringToLogicOp.Find(str);
    if (it != stringToLogicOp.end())
        return it->second;
    else
        return Serialization::LogicOp::InvalidLogicOp;
}

constexpr StaticMap stringToStencilOp =
std::array{
    std::pair{ "Keep"_c, Serialization::StencilOp::StencilKeepOp },
    std::pair{ "Zero"_c, Serialization::StencilOp::StencilZeroOp },
    std::pair{ "Replace"_c, Serialization::StencilOp::StencilReplaceOp },
    std::pair{ "IncrementClamp"_c, Serialization::StencilOp::StencilIncrementClampOp },
    std::pair{ "DecrementClamp"_c, Serialization::StencilOp::StencilDecrementClampOp },
    std::pair{ "Invert"_c, Serialization::StencilOp::StencilInvertOp },
    std::pair{ "IncrementWrap"_c, Serialization::StencilOp::StencilIncrementWrapOp },
    std::pair{ "DecrementWrap"_c, Serialization::StencilOp::StencilDecrementWrapOp }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::StencilOp
RenderStateInstance::__Resolved::StringToStencilOp(const TransientString& str)
{
    auto it = stringToStencilOp.Find(str);
    if (it != stringToStencilOp.end())
        return it->second;
    else
        return Serialization::StencilOp::InvalidStencilOp;
}

constexpr StaticMap stringToBlendFactor =
std::array{
    std::pair{ "Zero"_c,  Serialization::BlendFactor::ZeroFactor },
    std::pair{ "One"_c,  Serialization::BlendFactor::OneFactor },
    std::pair{ "SourceColor"_c,  Serialization::BlendFactor::SourceColorFactor },
    std::pair{ "OneMinusSourceColor"_c,  Serialization::BlendFactor::OneMinusSourceColorFactor },
    std::pair{ "DestinationColor"_c,  Serialization::BlendFactor::DestinationColorFactor },
    std::pair{ "OneMinusDestinationColor"_c,  Serialization::BlendFactor::OneMinusDestinationColorFactor },
    std::pair{ "SourceAlpha"_c,  Serialization::BlendFactor::SourceAlphaFactor },
    std::pair{ "OneMinusSourceAlpha"_c,  Serialization::BlendFactor::OneMinusSourceAlphaFactor },
    std::pair{ "DestinationAlpha"_c,  Serialization::BlendFactor::DestinationAlphaFactor },
    std::pair{ "OneMinusDestinationAlpha"_c,  Serialization::BlendFactor::OneMinusDestinationAlphaFactor },
    std::pair{ "ConstantColor"_c,  Serialization::BlendFactor::ConstantColorFactor },
    std::pair{ "OneMinusConstantColor"_c,  Serialization::BlendFactor::OneMinusConstantColorFactor },
    std::pair{ "ConstantAlpha"_c,  Serialization::BlendFactor::ConstantAlphaFactor },
    std::pair{ "OneMinusConstantAlpha"_c,  Serialization::BlendFactor::OneMinusConstantAlphaFactor },
    std::pair{ "SourceAlphaSaturate"_c,  Serialization::BlendFactor::SourceAlphaSaturateFactor },
    std::pair{ "Source1Color"_c,  Serialization::BlendFactor::Source1ColorFactor },
    std::pair{ "OneMinusSource1Color"_c,  Serialization::BlendFactor::OneMinusSource1ColorFactor },
    std::pair{ "Source1Alpha"_c,  Serialization::BlendFactor::Source1AlphaFactor },
    std::pair{ "OneMinusSource1Alpha"_c,  Serialization::BlendFactor::OneMinusSource1AlphaFactor }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::BlendFactor
RenderStateInstance::__Resolved::StringToBlendFactor(const TransientString& str)
{
    auto it = stringToBlendFactor.Find(str);
    if (it != stringToBlendFactor.end())
        return it->second;
    else
        return Serialization::BlendFactor::InvalidBlendFactor;
}

constexpr StaticMap stringToBlendOp =
std::array{
    std::pair{ "Add"_c, Serialization::BlendOp::AddOp },
    std::pair{ "Subtract"_c, Serialization::BlendOp::SubtractOp },
    std::pair{ "ReverseSubtract"_c, Serialization::BlendOp::ReverseSubtractOp },
    std::pair{ "Min"_c, Serialization::BlendOp::MinOp },
    std::pair{ "Max"_c, Serialization::BlendOp::MaxOp }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::BlendOp
RenderStateInstance::__Resolved::StringToBlendOp(const TransientString& str)
{
    auto it = stringToBlendOp.Find(str);
    if (it != stringToBlendOp.end())
        return it->second;
    else
        return Serialization::BlendOp::InvalidBlendOp;
}

} // namespace GPULang
