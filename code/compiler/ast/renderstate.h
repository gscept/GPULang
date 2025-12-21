#pragma once
//------------------------------------------------------------------------------
/**
    AST for RenderState

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "state.h"
namespace GPULang
{

struct Type;
struct RenderStateInstance : public State
{
    /// constructor
    RenderStateInstance();

    struct __Resolved : State::__Resolved
    {
        virtual ~__Resolved() {};

        Type* typeSymbol = nullptr;
        enum RenderStateEntryType
        {
            InvalidRenderStateEntryType,
            DepthClampEnabledType,
            NoPixelsType,
            RasterizationModeType,
            CullModeType,
            WindingOrderType,
            DepthBiasEnabledType,
            DepthBiasFactorType,
            DepthBiasClampType,
            DepthBiasSlopeFactorType,
            LineWidthType,
            DepthTestEnabledType,
            DepthWriteEnabledType,
            DepthTestFunction,
            DepthBoundsTestEnabledType,
            MinDepthBoundsType,
            MaxDepthBoundsType,
            ScissorEnabledType,
            StencilEnabledType,
            StencilFailOpType,
            StencilPassOpType,
            StencilDepthFailOpType,
            StencilCompareModeType,
            StencilCompareMaskType,
            StencilWriteMaskType,
            StencilReferenceMaskType,
            LogicOpEnabledType,
            LogicOpType,
            BlendEnabledType,
            SourceBlendColorFactorType,
            DestinationBlendColorFactorType,
            SourceBlendAlphaFactorType,
            DestinationBlendAlphaFactorType,
            ColorBlendOpType,
            AlphaBlendOpType,
            ColorComponentMaskType,
            BlendConstantsType,
            SamplesType,
            SampleShadingEnabledType,
            MinSampleShadingType,
            AlphaToCoverageEnabledType,
            AlphaToOneEnabledType,
        };

        /// convert from string to program entry type
        static const RenderStateEntryType StringToEntryType(const TransientString& str);
        /// convert from mapping to string
        static const ConstantString& EntryTypeToString(const RenderStateEntryType type);

        //------------------------------------------------------------------------------
        /**
            Rasterizer
        */
        //------------------------------------------------------------------------------
        bool depthClampEnabled = false;
        bool noPixels = false;

        /// convert from string
        static const Serialization::RasterizationMode StringToPolygonMode(const TransientString& str);

        Serialization::RasterizationMode rasterizationMode = Serialization::RasterizationMode::FillMode;

        /// convert from string
        static const Serialization::CullMode StringToCullMode(const TransientString& str);

        Serialization::CullMode cullMode = Serialization::CullMode::BackMode;

        /// convert from string
        static const Serialization::WindingOrderMode StringToWindingOrderMode(const TransientString& str);
                         
        Serialization::WindingOrderMode windingOrderMode = Serialization::WindingOrderMode::CounterClockwiseMode;
        bool depthBiasEnabled = false;
        float depthBiasFactor = 0.0f;
        float depthBiasClamp = 0.0f;
        float depthBiasSlopeFactor = 0.0f;
        float lineWidth = 1.0f;

        //------------------------------------------------------------------------------
        /**
            Depth-stencil
        */
        //------------------------------------------------------------------------------
        bool depthTestEnabled = false;
        bool depthWriteEnabled = false;
        Serialization::CompareMode depthCompare = Serialization::CompareMode::LessEqualCompare;
        bool depthBoundsTestEnabled = false;
        float minDepthBounds = 0.0f;
        float maxDepthBounds = 1.0f;
        bool scissorEnabled = false;
        bool stencilEnabled = false;

        /// convert from string
        static const Serialization::StencilOp StringToStencilOp(const TransientString& str);

        Serialization::StencilState frontStencilState;
        Serialization::StencilState backStencilState;


        //------------------------------------------------------------------------------
        /**
            Blend
        */
        //------------------------------------------------------------------------------
        bool logicOpEnabled = false;

        /// convert from string
        static const Serialization::LogicOp StringToLogicOp(const TransientString& str);

        Serialization::LogicOp logicOp = Serialization::LogicOp::LogicSetOp;

        /// convert from string
        static const Serialization::BlendFactor StringToBlendFactor(const TransientString& str);

        /// convert from string
        static const Serialization::BlendOp StringToBlendOp(const TransientString& str);

        static const uint8_t NUM_BLEND_STATES = 8;
        Serialization::BlendState blendStates[NUM_BLEND_STATES];
        float blendConstants[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        //------------------------------------------------------------------------------
        /**
            Multisample
        */
        //------------------------------------------------------------------------------
        uint32_t samples = 1;
        bool sampleShadingEnabled = false;
        float minSampleShading = 1.0f;
        bool alphaToCoverageEnabled = false;
        bool alphaToOneEnabled = false;
    };
};

} // namespace GPULang
