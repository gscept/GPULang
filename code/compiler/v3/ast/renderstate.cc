//------------------------------------------------------------------------------
//  @file renderstate.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "renderstate.h"
#include <map>
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
RenderState::RenderState()
{
    this->symbolType = RenderStateType;
    this->resolved = new RenderState::__Resolved();
}

std::map<std::string, RenderState::__Resolved::RenderStateEntryType> stringToRenderStateEntryType =
{
    { "DepthClampEnabled", RenderState::__Resolved::DepthClampEnabledType },
    { "NoPixels", RenderState::__Resolved::NoPixelsType },
    { "PolygonMode", RenderState::__Resolved::PolygonModeType },
    { "CullMode", RenderState::__Resolved::CullModeType },
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
    { "StencilEnabled", RenderState::__Resolved::StencilEnabledType },
    { "FrontStencil.FailOp", RenderState::__Resolved::StencilFailOpType },
    { "FrontStencil.PassOp", RenderState::__Resolved::StencilPassOpType },
    { "FrontStencil.DepthFailOp", RenderState::__Resolved::StencilDepthFailOpType },
    { "FrontStencil.CompareMode", RenderState::__Resolved::StencilCompareModeType },
    { "FrontStencil.CompareMask", RenderState::__Resolved::StencilCompareMaskType },
    { "FrontStencil.WriteMask", RenderState::__Resolved::StencilWriteMaskType },
    { "FrontStencil.ReferenceMask", RenderState::__Resolved::StencilReferenceMaskType },
    { "BackStencil.FailOp", RenderState::__Resolved::StencilFailOpType },
    { "BackStencil.PassOp", RenderState::__Resolved::StencilPassOpType },
    { "BackStencil.DepthFailOp", RenderState::__Resolved::StencilDepthFailOpType },
    { "BackStencil.CompareMode", RenderState::__Resolved::StencilCompareModeType },
    { "BackStencil.CompareMask", RenderState::__Resolved::StencilCompareMaskType },
    { "BackStencil.WriteMask", RenderState::__Resolved::StencilWriteMaskType },
    { "BackStencil.ReferenceMask", RenderState::__Resolved::StencilReferenceMaskType },
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
    { "ColorComponentMask", RenderState::__Resolved::ColorComponentMaskType }
};

//------------------------------------------------------------------------------
/**
*/
const RenderState::__Resolved::RenderStateEntryType
RenderState::__Resolved::StringToEntryType(const std::string& str)
{
    auto it = stringToRenderStateEntryType.find(str);
    if (it != stringToRenderStateEntryType.end())
        return it->second;
    else
        return RenderState::__Resolved::RenderStateEntryType::InvalidRenderStateEntryType;

}

//------------------------------------------------------------------------------
/**
*/
const std::string&
RenderState::__Resolved::EntryTypeToString(const RenderStateEntryType type)
{
    static std::string default = "";
    for (auto& it : stringToRenderStateEntryType)
    {
        if (it.second == type)
            return it.first;
    }
    return default;
}

std::map<std::string, PolygonMode> stringToPolygonMode =
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
    auto it = stringToPolygonMode.find(str);
    if (it != stringToPolygonMode.end())
        return it->second;
    else
        return InvalidPolygonMode;
}

std::map<std::string, CullMode> stringToCullMode =
{
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
    auto it = stringToCullMode.find(str);
    if (it != stringToCullMode.end())
        return it->second;
    else
        return InvalidCullMode;
}

std::map<std::string, WindingOrderMode> stringToWindingOrderMode =
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
    auto it = stringToWindingOrderMode.find(str);
    if (it != stringToWindingOrderMode.end())
        return it->second;
    else
        return InvalidWindingOrderMode;
}

std::map<std::string, LogicOp> stringToLogicOp =
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
    auto it = stringToLogicOp.find(str);
    if (it != stringToLogicOp.end())
        return it->second;
    else
        return InvalidLogicOp;
}

std::map<std::string, StencilOp> stringToStencilOp =
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
    auto it = stringToStencilOp.find(str);
    if (it != stringToStencilOp.end())
        return it->second;
    else
        return InvalidStencilOp;
}

std::map<std::string, BlendFactor> stringToBlendFactor =
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
    auto it = stringToBlendFactor.find(str);
    if (it != stringToBlendFactor.end())
        return it->second;
    else
        return InvalidBlendFactor;
}

std::map<std::string, BlendOp> stringToBlendOp =
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
    auto it = stringToBlendOp.find(str);
    if (it != stringToBlendOp.end())
        return it->second;
    else
        return InvalidBlendOp;
}

} // namespace AnyFX
