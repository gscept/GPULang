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

const StaticMap<ConstantString, RenderState::__Resolved::RenderStateEntryType> stringToRenderStateEntryType =
{
    { "DepthClampEnabled", RenderState::__Resolved::DepthClampEnabledType },
    { "NoPixels", RenderState::__Resolved::NoPixelsType },
    { "Polygon", RenderState::__Resolved::PolygonModeType },
    { "Cull", RenderState::__Resolved::CullModeType },
    { "WindingOrder", RenderState::__Resolved::WindingOrderType },
    { "DepthBiasEnabled", RenderState::__Resolved::DepthBiasEnabledType },
    { "DepthBiasFactor", RenderState::__Resolved::DepthBiasFactorType },
    { "DepthBiasClamp", RenderState::__Resolved::DepthBiasClampType },
    { "DepthBiasSlopeFactor", RenderState::__Resolved::DepthBiasSlopeFactorType },
    { "LineWidth", RenderState::__Resolved::LineWidthType },
    { "DepthTestEnabled", RenderState::__Resolved::DepthTestEnabledType },
    { "DepthWriteEnabled", RenderState::__Resolved::DepthWriteEnabledType },
    { "DepthTestFunction", RenderState::__Resolved::DepthTestFunction },
    { "DepthBoundsTestEnabled", RenderState::__Resolved::DepthBoundsTestEnabledType },
    { "MinDepthBounds", RenderState::__Resolved::MinDepthBoundsType },
    { "MaxDepthBounds", RenderState::__Resolved::MaxDepthBoundsType },
    { "ScissorEnabled", RenderState::__Resolved::ScissorEnabledType },
    { "StencilEnabled", RenderState::__Resolved::StencilEnabledType },
    { "StencilFront.Fail", RenderState::__Resolved::StencilFailOpType },
    { "StencilFront.Pass", RenderState::__Resolved::StencilPassOpType },
    { "StencilFront.DepthFail", RenderState::__Resolved::StencilDepthFailOpType },
    { "StencilFront.Compare", RenderState::__Resolved::StencilCompareModeType },
    { "StencilFront.CompareMask", RenderState::__Resolved::StencilCompareMaskType },
    { "StencilFront.WriteMask", RenderState::__Resolved::StencilWriteMaskType },
    { "StencilFront.ReferenceMask", RenderState::__Resolved::StencilReferenceMaskType },
    { "StencilBack.Fail", RenderState::__Resolved::StencilFailOpType },
    { "StencilBack.Pass", RenderState::__Resolved::StencilPassOpType },
    { "StencilBack.DepthFail", RenderState::__Resolved::StencilDepthFailOpType },
    { "StencilBack.Compare", RenderState::__Resolved::StencilCompareModeType },
    { "StencilBack.CompareMask", RenderState::__Resolved::StencilCompareMaskType },
    { "StencilBack.WriteMask", RenderState::__Resolved::StencilWriteMaskType },
    { "StencilBack.ReferenceMask", RenderState::__Resolved::StencilReferenceMaskType },
    { "LogicOpEnabled", RenderState::__Resolved::LogicOpEnabledType },
    { "LogicOp", RenderState::__Resolved::LogicOpType },
    { "BlendEnabled", RenderState::__Resolved::BlendEnabledType },
    { "SrcBlend", RenderState::__Resolved::SourceBlendColorFactorType },
    { "SourceBlend", RenderState::__Resolved::SourceBlendColorFactorType },
    { "DstBlend", RenderState::__Resolved::DestinationBlendColorFactorType },
    { "DestinationBlend", RenderState::__Resolved::DestinationBlendColorFactorType },
    { "SrcAlphaBlend", RenderState::__Resolved::SourceBlendAlphaFactorType },
    { "SourceAlphaBlend", RenderState::__Resolved::SourceBlendAlphaFactorType },
    { "DstAlphaBlend", RenderState::__Resolved::DestinationBlendAlphaFactorType },
    { "DestinationAlphaBlend", RenderState::__Resolved::DestinationBlendAlphaFactorType },
    { "BlendOp", RenderState::__Resolved::ColorBlendOpType },
    { "BlendOpAlpha", RenderState::__Resolved::AlphaBlendOpType },
    { "ColorComponentMask", RenderState::__Resolved::ColorComponentMaskType },
    { "BlendConstants", RenderState::__Resolved::BlendConstantsType }
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

const StaticMap<std::string, PolygonMode> stringToPolygonMode =
{
    { "Fill" , FillMode },
    { "Line" , LineMode },
    { "Point", PointMode }
};

//------------------------------------------------------------------------------
/**
*/
const PolygonMode
RenderState::__Resolved::StringToPolygonMode(const std::string& str)
{
    auto it = stringToPolygonMode.Find(str);
    if (it != stringToPolygonMode.end())
        return it->second;
    else
        return InvalidPolygonMode;
}

const StaticMap<std::string, CullMode> stringToCullMode =
{
    { "None", NoCullMode },
    { "Front", FrontMode },
    { "Back", BackMode },
    { "FrontAndBack", FrontAndBackMode }
};

//------------------------------------------------------------------------------
/**
*/
const CullMode
RenderState::__Resolved::StringToCullMode(const std::string& str)
{
    auto it = stringToCullMode.Find(str);
    if (it != stringToCullMode.end())
        return it->second;
    else
        return InvalidCullMode;
}

const StaticMap<std::string, WindingOrderMode> stringToWindingOrderMode =
{
    { "Clockwise", ClockwiseMode },
    { "CounterClockwise", CounterClockwiseMode }
};

//------------------------------------------------------------------------------
/**
*/
const WindingOrderMode
RenderState::__Resolved::StringToWindingOrderMode(const std::string& str)
{
    auto it = stringToWindingOrderMode.Find(str);
    if (it != stringToWindingOrderMode.end())
        return it->second;
    else
        return InvalidWindingOrderMode;
}

const StaticMap<std::string, LogicOp> stringToLogicOp =
{
    { "Clear", LogicClearOp },
    { "And", LogicAndOp },
    { "AndReverse", LogicAndReverseOp },
    { "Copy", LogicCopyOp },
    { "AndInverted", LogicAndInvertedOp },
    { "No", LogicNoOp },
    { "Xor", LogicXorOp },
    { "Or", LogicOrOp },
    { "Nor", LogicNorOp },
    { "Equivalent", LogicEquivalentOp },
    { "Invert", LogicInvertOp },
    { "OrReverse", LogicOrReverseOp },
    { "CopyInverted", LogicCopyInvertedOp },
    { "OrInverted", LogicOrInvertedOp },
    { "Nand", LogicNandOp },
    { "Set", LogicSetOp }
};

//------------------------------------------------------------------------------
/**
*/
const LogicOp
RenderState::__Resolved::StringToLogicOp(const std::string& str)
{
    auto it = stringToLogicOp.Find(str);
    if (it != stringToLogicOp.end())
        return it->second;
    else
        return InvalidLogicOp;
}

const StaticMap<std::string, StencilOp> stringToStencilOp =
{
    { "Keep", StencilKeepOp },
    { "Zero", StencilZeroOp },
    { "Replace", StencilReplaceOp },
    { "IncrementClamp", StencilIncrementClampOp },
    { "DecrementClamp", StencilDecrementClampOp },
    { "Invert", StencilInvertOp },
    { "IncrementWrap", StencilIncrementWrapOp },
    { "DecrementWrap", StencilDecrementWrapOp }
};

//------------------------------------------------------------------------------
/**
*/
const StencilOp
RenderState::__Resolved::StringToStencilOp(const std::string& str)
{
    auto it = stringToStencilOp.Find(str);
    if (it != stringToStencilOp.end())
        return it->second;
    else
        return InvalidStencilOp;
}

const StaticMap<std::string, BlendFactor> stringToBlendFactor =
{
    { "Zero", ZeroFactor },
    { "One", OneFactor },
    { "SourceColor", SourceColorFactor },
    { "OneMinusSourceColor", OneMinusSourceColorFactor },
    { "DestinationColor", DestinationColorFactor },
    { "OneMinusDestinationColor", OneMinusDestinationColorFactor },
    { "SourceAlpha", SourceAlphaFactor },
    { "OneMinusSourceAlpha", OneMinusSourceAlphaFactor },
    { "DestinationAlpha", DestinationAlphaFactor },
    { "OneMinusDestinationAlpha", OneMinusDestinationAlphaFactor },
    { "ConstantColor", ConstantColorFactor },
    { "OneMinusConstantColor", OneMinusConstantColorFactor },
    { "ConstantAlpha", ConstantAlphaFactor },
    { "OneMinusConstantAlpha", OneMinusConstantAlphaFactor },
    { "SourceAlphaSaturate", SourceAlphaSaturateFactor },
    { "Source1Color", Source1ColorFactor },
    { "OneMinusSource1Color", OneMinusSource1ColorFactor },
    { "Source1Alpha", Source1AlphaFactor },
    { "OneMinusSource1Alpha", OneMinusSource1AlphaFactor }
};

//------------------------------------------------------------------------------
/**
*/
const BlendFactor
RenderState::__Resolved::StringToBlendFactor(const std::string& str)
{
    auto it = stringToBlendFactor.Find(str);
    if (it != stringToBlendFactor.end())
        return it->second;
    else
        return InvalidBlendFactor;
}

const StaticMap<std::string, BlendOp> stringToBlendOp =
{
    { "Add", AddOp },
    { "Subtract", SubtractOp },
    { "ReverseSubtract", ReverseSubtractOp },
    { "Min", MinOp },
    { "Max", MaxOp }
};

//------------------------------------------------------------------------------
/**
*/
const BlendOp
RenderState::__Resolved::StringToBlendOp(const std::string& str)
{
    auto it = stringToBlendOp.Find(str);
    if (it != stringToBlendOp.end())
        return it->second;
    else
        return InvalidBlendOp;
}

} // namespace GPULang
