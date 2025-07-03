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
RenderState::RenderState()
{
    this->symbolType = RenderStateType;
    if (SYMBOL_STATIC_ALLOC)
        this->resolved = StaticAlloc<RenderState::__Resolved>();
    else
        this->resolved = Alloc<RenderState::__Resolved>();
    RenderState::__Resolved* typeResolved = static_cast<RenderState::__Resolved*>(this->resolved);
    typeResolved->depthClampEnabled = false;
    typeResolved->noPixels = false;
    typeResolved->polygonMode = PolygonMode::FillMode;
    typeResolved->cullMode = CullMode::BackMode;
    typeResolved->windingOrderMode = WindingOrderMode::CounterClockwiseMode;
    typeResolved->depthBiasEnabled = false;
    typeResolved->depthBiasFactor = 0.0f;
    typeResolved->depthBiasClamp = 0.0f;
    typeResolved->depthBiasSlopeFactor = 0.0f;
    typeResolved->lineWidth = 1.0f;

    typeResolved->depthTestEnabled = true;
    typeResolved->depthWriteEnabled = true;
    typeResolved->depthCompare = CompareMode::LessEqualCompare;
    typeResolved->depthBoundsTestEnabled = true;
    typeResolved->minDepthBounds = 0.0f;
    typeResolved->maxDepthBounds = 1.0f;
    typeResolved->scissorEnabled = false;
    typeResolved->stencilEnabled = false;
    typeResolved->logicOpEnabled = false;
    typeResolved->logicOp = LogicOp::LogicSetOp;
    BlendState defaultBlend =
    {
        .blendEnabled = false,
        .sourceColorBlendFactor = BlendFactor::OneFactor,
        .destinationColorBlendFactor = BlendFactor::OneFactor,
        .sourceAlphaBlendFactor = BlendFactor::OneFactor,
        .destinationAlphaBlendFactor = BlendFactor::OneFactor,
        .colorBlendOp = BlendOp::AddOp,
        .alphaBlendOp = BlendOp::AddOp,
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

    StencilState defaultStencil =
    {
        .fail = StencilOp::StencilKeepOp,
        .pass = StencilOp::StencilKeepOp,
        .depthFail = StencilOp::StencilKeepOp,
        .compare = CompareMode::EqualCompare,
        .compareMask = 0xFFFFFFFF,
        .writeMask = 0xFFFFFFFF,
        .referenceMask = 0xFFFFFFFF
    };
    typeResolved->frontStencilState = defaultStencil;
    typeResolved->backStencilState = defaultStencil;
}

const StaticMap stringToRenderStateEntryType =
std::array{
    std::pair{ ConstantString("DepthClampEnabled"), RenderState::__Resolved::DepthClampEnabledType },
    std::pair{ ConstantString("NoPixels"), RenderState::__Resolved::NoPixelsType },
    std::pair{ ConstantString("Polygon"), RenderState::__Resolved::PolygonModeType },
    std::pair{ ConstantString("Cull"), RenderState::__Resolved::CullModeType },
    std::pair{ ConstantString("WindingOrder"), RenderState::__Resolved::WindingOrderType },
    std::pair{ ConstantString("DepthBiasEnabled"), RenderState::__Resolved::DepthBiasEnabledType },
    std::pair{ ConstantString("DepthBiasFactor"), RenderState::__Resolved::DepthBiasFactorType },
    std::pair{ ConstantString("DepthBiasClamp"), RenderState::__Resolved::DepthBiasClampType },
    std::pair{ ConstantString("DepthBiasSlopeFactor"), RenderState::__Resolved::DepthBiasSlopeFactorType },
    std::pair{ ConstantString("LineWidth"), RenderState::__Resolved::LineWidthType },
    std::pair{ ConstantString("DepthTestEnabled"), RenderState::__Resolved::DepthTestEnabledType },
    std::pair{ ConstantString("DepthWriteEnabled"), RenderState::__Resolved::DepthWriteEnabledType },
    std::pair{ ConstantString("DepthTestFunction"), RenderState::__Resolved::DepthTestFunction },
    std::pair{ ConstantString("DepthBoundsTestEnabled"), RenderState::__Resolved::DepthBoundsTestEnabledType },
    std::pair{ ConstantString("MinDepthBounds"), RenderState::__Resolved::MinDepthBoundsType },
    std::pair{ ConstantString("MaxDepthBounds"), RenderState::__Resolved::MaxDepthBoundsType },
    std::pair{ ConstantString("ScissorEnabled"), RenderState::__Resolved::ScissorEnabledType },
    std::pair{ ConstantString("StencilEnabled"), RenderState::__Resolved::StencilEnabledType },
    std::pair{ ConstantString("StencilFront.Fail"), RenderState::__Resolved::StencilFailOpType },
    std::pair{ ConstantString("StencilFront.Pass"), RenderState::__Resolved::StencilPassOpType },
    std::pair{ ConstantString("StencilFront.DepthFail"), RenderState::__Resolved::StencilDepthFailOpType },
    std::pair{ ConstantString("StencilFront.Compare"), RenderState::__Resolved::StencilCompareModeType },
    std::pair{ ConstantString("StencilFront.CompareMask"), RenderState::__Resolved::StencilCompareMaskType },
    std::pair{ ConstantString("StencilFront.WriteMask"), RenderState::__Resolved::StencilWriteMaskType },
    std::pair{ ConstantString("StencilFront.ReferenceMask"), RenderState::__Resolved::StencilReferenceMaskType },
    std::pair{ ConstantString("StencilBack.Fail"), RenderState::__Resolved::StencilFailOpType },
    std::pair{ ConstantString("StencilBack.Pass"), RenderState::__Resolved::StencilPassOpType },
    std::pair{ ConstantString("StencilBack.DepthFail"), RenderState::__Resolved::StencilDepthFailOpType },
    std::pair{ ConstantString("StencilBack.Compare"), RenderState::__Resolved::StencilCompareModeType },
    std::pair{ ConstantString("StencilBack.CompareMask"), RenderState::__Resolved::StencilCompareMaskType },
    std::pair{ ConstantString("StencilBack.WriteMask"), RenderState::__Resolved::StencilWriteMaskType },
    std::pair{ ConstantString("StencilBack.ReferenceMask"), RenderState::__Resolved::StencilReferenceMaskType },
    std::pair{ ConstantString("LogicOpEnabled"), RenderState::__Resolved::LogicOpEnabledType },
    std::pair{ ConstantString("LogicOp"), RenderState::__Resolved::LogicOpType },
    std::pair{ ConstantString("BlendEnabled"), RenderState::__Resolved::BlendEnabledType },
    std::pair{ ConstantString("SrcBlend"), RenderState::__Resolved::SourceBlendColorFactorType },
    std::pair{ ConstantString("SourceBlend"), RenderState::__Resolved::SourceBlendColorFactorType },
    std::pair{ ConstantString("DstBlend"), RenderState::__Resolved::DestinationBlendColorFactorType },
    std::pair{ ConstantString("DestinationBlend"), RenderState::__Resolved::DestinationBlendColorFactorType },
    std::pair{ ConstantString("SrcAlphaBlend"), RenderState::__Resolved::SourceBlendAlphaFactorType },
    std::pair{ ConstantString("SourceAlphaBlend"), RenderState::__Resolved::SourceBlendAlphaFactorType },
    std::pair{ ConstantString("DstAlphaBlend"), RenderState::__Resolved::DestinationBlendAlphaFactorType },
    std::pair{ ConstantString("DestinationAlphaBlend"), RenderState::__Resolved::DestinationBlendAlphaFactorType },
    std::pair{ ConstantString("BlendOp"), RenderState::__Resolved::ColorBlendOpType },
    std::pair{ ConstantString("BlendOpAlpha"), RenderState::__Resolved::AlphaBlendOpType },
    std::pair{ ConstantString("ColorComponentMask"), RenderState::__Resolved::ColorComponentMaskType },
    std::pair{ ConstantString("BlendConstants"), RenderState::__Resolved::BlendConstantsType }
};

static ConstantString NoRenderStateEntry = "";

//------------------------------------------------------------------------------
/**
*/
const RenderState::__Resolved::RenderStateEntryType
RenderState::__Resolved::StringToEntryType(const TransientString& str)
{
    auto it = stringToRenderStateEntryType.Find(str);
    if (it != stringToRenderStateEntryType.end())
        return it->second;
    else
        return RenderState::__Resolved::RenderStateEntryType::InvalidRenderStateEntryType;

}

//------------------------------------------------------------------------------
/**
*/
const ConstantString&
RenderState::__Resolved::EntryTypeToString(const RenderStateEntryType type)
{
    for (auto& it : stringToRenderStateEntryType)
    {
        if (it.second == type)
            return it.first;
    }
    return NoRenderStateEntry;
}

const StaticMap stringToPolygonMode =
std::array{
    std::pair{ ConstantString("Fill") , FillMode },
    std::pair{ ConstantString("Line") , LineMode },
    std::pair{ ConstantString("Point"), PointMode }
};

//------------------------------------------------------------------------------
/**
*/
const PolygonMode
RenderState::__Resolved::StringToPolygonMode(const TransientString& str)
{
    auto it = stringToPolygonMode.Find(str);
    if (it != stringToPolygonMode.end())
        return it->second;
    else
        return InvalidPolygonMode;
}

const StaticMap stringToCullMode =
std::array{
    std::pair{ ConstantString("None"), NoCullMode },
    std::pair{ ConstantString("Front"), FrontMode },
    std::pair{ ConstantString("Back"), BackMode },
    std::pair{ ConstantString("FrontAndBack"), FrontAndBackMode }
};

//------------------------------------------------------------------------------
/**
*/
const CullMode
RenderState::__Resolved::StringToCullMode(const TransientString& str)
{
    auto it = stringToCullMode.Find(str);
    if (it != stringToCullMode.end())
        return it->second;
    else
        return InvalidCullMode;
}

const StaticMap stringToWindingOrderMode =
std::array{
    std::pair{ ConstantString("Clockwise"), ClockwiseMode },
    std::pair{ ConstantString("CounterClockwise"), CounterClockwiseMode }
};

//------------------------------------------------------------------------------
/**
*/
const WindingOrderMode
RenderState::__Resolved::StringToWindingOrderMode(const TransientString& str)
{
    auto it = stringToWindingOrderMode.Find(str);
    if (it != stringToWindingOrderMode.end())
        return it->second;
    else
        return InvalidWindingOrderMode;
}

const StaticMap stringToLogicOp =
std::array{
    std::pair{ ConstantString("Clear"), LogicClearOp },
    std::pair{ ConstantString("And"), LogicAndOp },
    std::pair{ ConstantString("AndReverse"), LogicAndReverseOp },
    std::pair{ ConstantString("Copy"), LogicCopyOp },
    std::pair{ ConstantString("AndInverted"), LogicAndInvertedOp },
    std::pair{ ConstantString("No"), LogicNoOp },
    std::pair{ ConstantString("Xor"), LogicXorOp },
    std::pair{ ConstantString("Or"), LogicOrOp },
    std::pair{ ConstantString("Nor"), LogicNorOp },
    std::pair{ ConstantString("Equivalent"), LogicEquivalentOp },
    std::pair{ ConstantString("Invert"), LogicInvertOp },
    std::pair{ ConstantString("OrReverse"), LogicOrReverseOp },
    std::pair{ ConstantString("CopyInverted"), LogicCopyInvertedOp },
    std::pair{ ConstantString("OrInverted"), LogicOrInvertedOp },
    std::pair{ ConstantString("Nand"), LogicNandOp },
    std::pair{ ConstantString("Set"), LogicSetOp }
};

//------------------------------------------------------------------------------
/**
*/
const LogicOp
RenderState::__Resolved::StringToLogicOp(const TransientString& str)
{
    auto it = stringToLogicOp.Find(str);
    if (it != stringToLogicOp.end())
        return it->second;
    else
        return InvalidLogicOp;
}

const StaticMap stringToStencilOp =
std::array{
    std::pair{ ConstantString("Keep"), StencilKeepOp },
    std::pair{ ConstantString("Zero"), StencilZeroOp },
    std::pair{ ConstantString("Replace"), StencilReplaceOp },
    std::pair{ ConstantString("IncrementClamp"), StencilIncrementClampOp },
    std::pair{ ConstantString("DecrementClamp"), StencilDecrementClampOp },
    std::pair{ ConstantString("Invert"), StencilInvertOp },
    std::pair{ ConstantString("IncrementWrap"), StencilIncrementWrapOp },
    std::pair{ ConstantString("DecrementWrap"), StencilDecrementWrapOp }
};

//------------------------------------------------------------------------------
/**
*/
const StencilOp
RenderState::__Resolved::StringToStencilOp(const TransientString& str)
{
    auto it = stringToStencilOp.Find(str);
    if (it != stringToStencilOp.end())
        return it->second;
    else
        return InvalidStencilOp;
}

const StaticMap stringToBlendFactor =
std::array{
    std::pair{ ConstantString("Zero"),  ZeroFactor },
    std::pair{ ConstantString("One"),  OneFactor },
    std::pair{ ConstantString("SourceColor"),  SourceColorFactor },
    std::pair{ ConstantString("OneMinusSourceColor"),  OneMinusSourceColorFactor },
    std::pair{ ConstantString("DestinationColor"),  DestinationColorFactor },
    std::pair{ ConstantString("OneMinusDestinationColor"),  OneMinusDestinationColorFactor },
    std::pair{ ConstantString("SourceAlpha"),  SourceAlphaFactor },
    std::pair{ ConstantString("OneMinusSourceAlpha"),  OneMinusSourceAlphaFactor },
    std::pair{ ConstantString("DestinationAlpha"),  DestinationAlphaFactor },
    std::pair{ ConstantString("OneMinusDestinationAlpha"),  OneMinusDestinationAlphaFactor },
    std::pair{ ConstantString("ConstantColor"),  ConstantColorFactor },
    std::pair{ ConstantString("OneMinusConstantColor"),  OneMinusConstantColorFactor },
    std::pair{ ConstantString("ConstantAlpha"),  ConstantAlphaFactor },
    std::pair{ ConstantString("OneMinusConstantAlpha"),  OneMinusConstantAlphaFactor },
    std::pair{ ConstantString("SourceAlphaSaturate"),  SourceAlphaSaturateFactor },
    std::pair{ ConstantString("Source1Color"),  Source1ColorFactor },
    std::pair{ ConstantString("OneMinusSource1Color"),  OneMinusSource1ColorFactor },
    std::pair{ ConstantString("Source1Alpha"),  Source1AlphaFactor },
    std::pair{ ConstantString("OneMinusSource1Alpha"),  OneMinusSource1AlphaFactor }
};

//------------------------------------------------------------------------------
/**
*/
const BlendFactor
RenderState::__Resolved::StringToBlendFactor(const TransientString& str)
{
    auto it = stringToBlendFactor.Find(str);
    if (it != stringToBlendFactor.end())
        return it->second;
    else
        return InvalidBlendFactor;
}

const StaticMap stringToBlendOp =
std::array{
    std::pair{ ConstantString("Add"), AddOp },
    std::pair{ ConstantString("Subtract"), SubtractOp },
    std::pair{ ConstantString("ReverseSubtract"), ReverseSubtractOp },
    std::pair{ ConstantString("Min"), MinOp },
    std::pair{ ConstantString("Max"), MaxOp }
};

//------------------------------------------------------------------------------
/**
*/
const BlendOp
RenderState::__Resolved::StringToBlendOp(const TransientString& str)
{
    auto it = stringToBlendOp.Find(str);
    if (it != stringToBlendOp.end())
        return it->second;
    else
        return InvalidBlendOp;
}

} // namespace GPULang
