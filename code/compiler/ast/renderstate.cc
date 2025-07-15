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
    if (SYMBOL_STATIC_ALLOC)
        this->resolved = StaticAlloc<RenderStateInstance::__Resolved>();
    else
        this->resolved = Alloc<RenderStateInstance::__Resolved>();
    RenderStateInstance::__Resolved* typeResolved = static_cast<RenderStateInstance::__Resolved*>(this->resolved);
    typeResolved->depthClampEnabled = false;
    typeResolved->noPixels = false;
    typeResolved->polygonMode = Serialization::PolygonMode::FillMode;
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
    std::pair{ ConstantString("DepthClampEnabled"), RenderStateInstance::__Resolved::DepthClampEnabledType },
    std::pair{ ConstantString("NoPixels"), RenderStateInstance::__Resolved::NoPixelsType },
    std::pair{ ConstantString("Polygon"), RenderStateInstance::__Resolved::PolygonModeType },
    std::pair{ ConstantString("Cull"), RenderStateInstance::__Resolved::CullModeType },
    std::pair{ ConstantString("WindingOrder"), RenderStateInstance::__Resolved::WindingOrderType },
    std::pair{ ConstantString("DepthBiasEnabled"), RenderStateInstance::__Resolved::DepthBiasEnabledType },
    std::pair{ ConstantString("DepthBiasFactor"), RenderStateInstance::__Resolved::DepthBiasFactorType },
    std::pair{ ConstantString("DepthBiasClamp"), RenderStateInstance::__Resolved::DepthBiasClampType },
    std::pair{ ConstantString("DepthBiasSlopeFactor"), RenderStateInstance::__Resolved::DepthBiasSlopeFactorType },
    std::pair{ ConstantString("LineWidth"), RenderStateInstance::__Resolved::LineWidthType },
    std::pair{ ConstantString("DepthTestEnabled"), RenderStateInstance::__Resolved::DepthTestEnabledType },
    std::pair{ ConstantString("DepthWriteEnabled"), RenderStateInstance::__Resolved::DepthWriteEnabledType },
    std::pair{ ConstantString("DepthTestFunction"), RenderStateInstance::__Resolved::DepthTestFunction },
    std::pair{ ConstantString("DepthBoundsTestEnabled"), RenderStateInstance::__Resolved::DepthBoundsTestEnabledType },
    std::pair{ ConstantString("MinDepthBounds"), RenderStateInstance::__Resolved::MinDepthBoundsType },
    std::pair{ ConstantString("MaxDepthBounds"), RenderStateInstance::__Resolved::MaxDepthBoundsType },
    std::pair{ ConstantString("ScissorEnabled"), RenderStateInstance::__Resolved::ScissorEnabledType },
    std::pair{ ConstantString("StencilEnabled"), RenderStateInstance::__Resolved::StencilEnabledType },
    std::pair{ ConstantString("StencilFront.Fail"), RenderStateInstance::__Resolved::StencilFailOpType },
    std::pair{ ConstantString("StencilFront.Pass"), RenderStateInstance::__Resolved::StencilPassOpType },
    std::pair{ ConstantString("StencilFront.DepthFail"), RenderStateInstance::__Resolved::StencilDepthFailOpType },
    std::pair{ ConstantString("StencilFront.Compare"), RenderStateInstance::__Resolved::StencilCompareModeType },
    std::pair{ ConstantString("StencilFront.CompareMask"), RenderStateInstance::__Resolved::StencilCompareMaskType },
    std::pair{ ConstantString("StencilFront.WriteMask"), RenderStateInstance::__Resolved::StencilWriteMaskType },
    std::pair{ ConstantString("StencilFront.ReferenceMask"), RenderStateInstance::__Resolved::StencilReferenceMaskType },
    std::pair{ ConstantString("StencilBack.Fail"), RenderStateInstance::__Resolved::StencilFailOpType },
    std::pair{ ConstantString("StencilBack.Pass"), RenderStateInstance::__Resolved::StencilPassOpType },
    std::pair{ ConstantString("StencilBack.DepthFail"), RenderStateInstance::__Resolved::StencilDepthFailOpType },
    std::pair{ ConstantString("StencilBack.Compare"), RenderStateInstance::__Resolved::StencilCompareModeType },
    std::pair{ ConstantString("StencilBack.CompareMask"), RenderStateInstance::__Resolved::StencilCompareMaskType },
    std::pair{ ConstantString("StencilBack.WriteMask"), RenderStateInstance::__Resolved::StencilWriteMaskType },
    std::pair{ ConstantString("StencilBack.ReferenceMask"), RenderStateInstance::__Resolved::StencilReferenceMaskType },
    std::pair{ ConstantString("LogicOpEnabled"), RenderStateInstance::__Resolved::LogicOpEnabledType },
    std::pair{ ConstantString("LogicOp"), RenderStateInstance::__Resolved::LogicOpType },
    std::pair{ ConstantString("BlendEnabled"), RenderStateInstance::__Resolved::BlendEnabledType },
    std::pair{ ConstantString("SrcBlend"), RenderStateInstance::__Resolved::SourceBlendColorFactorType },
    std::pair{ ConstantString("SourceBlend"), RenderStateInstance::__Resolved::SourceBlendColorFactorType },
    std::pair{ ConstantString("DstBlend"), RenderStateInstance::__Resolved::DestinationBlendColorFactorType },
    std::pair{ ConstantString("DestinationBlend"), RenderStateInstance::__Resolved::DestinationBlendColorFactorType },
    std::pair{ ConstantString("SrcAlphaBlend"), RenderStateInstance::__Resolved::SourceBlendAlphaFactorType },
    std::pair{ ConstantString("SourceAlphaBlend"), RenderStateInstance::__Resolved::SourceBlendAlphaFactorType },
    std::pair{ ConstantString("DstAlphaBlend"), RenderStateInstance::__Resolved::DestinationBlendAlphaFactorType },
    std::pair{ ConstantString("DestinationAlphaBlend"), RenderStateInstance::__Resolved::DestinationBlendAlphaFactorType },
    std::pair{ ConstantString("BlendOp"), RenderStateInstance::__Resolved::ColorBlendOpType },
    std::pair{ ConstantString("BlendOpAlpha"), RenderStateInstance::__Resolved::AlphaBlendOpType },
    std::pair{ ConstantString("ColorComponentMask"), RenderStateInstance::__Resolved::ColorComponentMaskType },
    std::pair{ ConstantString("BlendConstants"), RenderStateInstance::__Resolved::BlendConstantsType }
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
    std::pair{ ConstantString("Fill") , Serialization::PolygonMode::FillMode },
    std::pair{ ConstantString("Line") , Serialization::PolygonMode::LineMode },
    std::pair{ ConstantString("Point"), Serialization::PolygonMode::PointMode }
};

//------------------------------------------------------------------------------
/**
*/
const Serialization::PolygonMode
RenderStateInstance::__Resolved::StringToPolygonMode(const TransientString& str)
{
    auto it = stringToPolygonMode.Find(str);
    if (it != stringToPolygonMode.end())
        return it->second;
    else
        return Serialization::PolygonMode::InvalidPolygonMode;
}

constexpr StaticMap stringToCullMode =
std::array{
    std::pair{ ConstantString("None"), Serialization::CullMode::NoCullMode },
    std::pair{ ConstantString("Front"), Serialization::CullMode::FrontMode },
    std::pair{ ConstantString("Back"), Serialization::CullMode::BackMode },
    std::pair{ ConstantString("FrontAndBack"), Serialization::CullMode::FrontAndBackMode }
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
    std::pair{ ConstantString("Clockwise"), Serialization::WindingOrderMode::ClockwiseMode },
    std::pair{ ConstantString("CounterClockwise"), Serialization::WindingOrderMode::CounterClockwiseMode }
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
    std::pair{ ConstantString("Clear"), Serialization::LogicOp::LogicClearOp },
    std::pair{ ConstantString("And"), Serialization::LogicOp::LogicAndOp },
    std::pair{ ConstantString("AndReverse"), Serialization::LogicOp::LogicAndReverseOp },
    std::pair{ ConstantString("Copy"), Serialization::LogicOp::LogicCopyOp },
    std::pair{ ConstantString("AndInverted"), Serialization::LogicOp::LogicAndInvertedOp },
    std::pair{ ConstantString("No"), Serialization::LogicOp::LogicNoOp },
    std::pair{ ConstantString("Xor"), Serialization::LogicOp::LogicXorOp },
    std::pair{ ConstantString("Or"), Serialization::LogicOp::LogicOrOp },
    std::pair{ ConstantString("Nor"), Serialization::LogicOp::LogicNorOp },
    std::pair{ ConstantString("Equivalent"), Serialization::LogicOp::LogicEquivalentOp },
    std::pair{ ConstantString("Invert"), Serialization::LogicOp::LogicInvertOp },
    std::pair{ ConstantString("OrReverse"), Serialization::LogicOp::LogicOrReverseOp },
    std::pair{ ConstantString("CopyInverted"), Serialization::LogicOp::LogicCopyInvertedOp },
    std::pair{ ConstantString("OrInverted"), Serialization::LogicOp::LogicOrInvertedOp },
    std::pair{ ConstantString("Nand"), Serialization::LogicOp::LogicNandOp },
    std::pair{ ConstantString("Set"), Serialization::LogicOp::LogicSetOp }
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
    std::pair{ ConstantString("Keep"), Serialization::StencilOp::StencilKeepOp },
    std::pair{ ConstantString("Zero"), Serialization::StencilOp::StencilZeroOp },
    std::pair{ ConstantString("Replace"), Serialization::StencilOp::StencilReplaceOp },
    std::pair{ ConstantString("IncrementClamp"), Serialization::StencilOp::StencilIncrementClampOp },
    std::pair{ ConstantString("DecrementClamp"), Serialization::StencilOp::StencilDecrementClampOp },
    std::pair{ ConstantString("Invert"), Serialization::StencilOp::StencilInvertOp },
    std::pair{ ConstantString("IncrementWrap"), Serialization::StencilOp::StencilIncrementWrapOp },
    std::pair{ ConstantString("DecrementWrap"), Serialization::StencilOp::StencilDecrementWrapOp }
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
    std::pair{ ConstantString("Zero"),  Serialization::BlendFactor::ZeroFactor },
    std::pair{ ConstantString("One"),  Serialization::BlendFactor::OneFactor },
    std::pair{ ConstantString("SourceColor"),  Serialization::BlendFactor::SourceColorFactor },
    std::pair{ ConstantString("OneMinusSourceColor"),  Serialization::BlendFactor::OneMinusSourceColorFactor },
    std::pair{ ConstantString("DestinationColor"),  Serialization::BlendFactor::DestinationColorFactor },
    std::pair{ ConstantString("OneMinusDestinationColor"),  Serialization::BlendFactor::OneMinusDestinationColorFactor },
    std::pair{ ConstantString("SourceAlpha"),  Serialization::BlendFactor::SourceAlphaFactor },
    std::pair{ ConstantString("OneMinusSourceAlpha"),  Serialization::BlendFactor::OneMinusSourceAlphaFactor },
    std::pair{ ConstantString("DestinationAlpha"),  Serialization::BlendFactor::DestinationAlphaFactor },
    std::pair{ ConstantString("OneMinusDestinationAlpha"),  Serialization::BlendFactor::OneMinusDestinationAlphaFactor },
    std::pair{ ConstantString("ConstantColor"),  Serialization::BlendFactor::ConstantColorFactor },
    std::pair{ ConstantString("OneMinusConstantColor"),  Serialization::BlendFactor::OneMinusConstantColorFactor },
    std::pair{ ConstantString("ConstantAlpha"),  Serialization::BlendFactor::ConstantAlphaFactor },
    std::pair{ ConstantString("OneMinusConstantAlpha"),  Serialization::BlendFactor::OneMinusConstantAlphaFactor },
    std::pair{ ConstantString("SourceAlphaSaturate"),  Serialization::BlendFactor::SourceAlphaSaturateFactor },
    std::pair{ ConstantString("Source1Color"),  Serialization::BlendFactor::Source1ColorFactor },
    std::pair{ ConstantString("OneMinusSource1Color"),  Serialization::BlendFactor::OneMinusSource1ColorFactor },
    std::pair{ ConstantString("Source1Alpha"),  Serialization::BlendFactor::Source1AlphaFactor },
    std::pair{ ConstantString("OneMinusSource1Alpha"),  Serialization::BlendFactor::OneMinusSource1AlphaFactor }
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
    std::pair{ ConstantString("Add"), Serialization::BlendOp::AddOp },
    std::pair{ ConstantString("Subtract"), Serialization::BlendOp::SubtractOp },
    std::pair{ ConstantString("ReverseSubtract"), Serialization::BlendOp::ReverseSubtractOp },
    std::pair{ ConstantString("Min"), Serialization::BlendOp::MinOp },
    std::pair{ ConstantString("Max"), Serialization::BlendOp::MaxOp }
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
