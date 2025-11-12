//------------------------------------------------------------------------------
//  @file renderstate.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "renderstate.h"
#include "generated/types.h"
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
    typeResolved->depthBoundsTestEnabled = false;
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
        .colorComponentMask = BlendColorMaskR_value | BlendColorMaskG_value | BlendColorMaskB_value | BlendColorMaskA_value
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

    typeResolved->samples = 1;
    typeResolved->sampleShadingEnabled = false;
    typeResolved->minSampleShading = 1.0f;
    typeResolved->alphaToCoverageEnabled = false;
    typeResolved->alphaToOneEnabled = false;
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
    std::pair{ "BlendConstants"_c, RenderStateInstance::__Resolved::BlendConstantsType },
    std::pair{ "Samples"_c, RenderStateInstance::__Resolved::SamplesType },
    std::pair{ "SampleShadingEnabled"_c, RenderStateInstance::__Resolved::SampleShadingEnabledType },
    std::pair{ "MinSampleShading"_c, RenderStateInstance::__Resolved::MinSampleShadingType },
    std::pair{ "AlphaToCoverageEnabled"_c, RenderStateInstance::__Resolved::AlphaToCoverageEnabledType },
    std::pair{ "AlphaToOneEnabled"_c, RenderStateInstance::__Resolved::AlphaToOneEnabledType },
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

} // namespace GPULang
