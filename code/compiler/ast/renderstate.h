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

        Type* typeSymbol;
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
        bool depthClampEnabled;
        bool noPixels;

        /// convert from string
        static const Serialization::RasterizationMode StringToPolygonMode(const TransientString& str);

        Serialization::RasterizationMode rasterizationMode;

        /// convert from string
        static const Serialization::CullMode StringToCullMode(const TransientString& str);

        Serialization::CullMode cullMode;

        /// convert from string
        static const Serialization::WindingOrderMode StringToWindingOrderMode(const TransientString& str);

        Serialization::WindingOrderMode windingOrderMode;
        bool depthBiasEnabled;
        float depthBiasFactor;
        float depthBiasClamp;
        float depthBiasSlopeFactor;
        float lineWidth;

        //------------------------------------------------------------------------------
        /**
            Depth-stencil
        */
        //------------------------------------------------------------------------------
        bool depthTestEnabled;
        bool depthWriteEnabled;
        Serialization::CompareMode depthCompare;
        bool depthBoundsTestEnabled;
        float minDepthBounds;
        float maxDepthBounds;
        bool scissorEnabled;
        bool stencilEnabled;

        /// convert from string
        static const Serialization::StencilOp StringToStencilOp(const TransientString& str);

        Serialization::StencilState frontStencilState;
        Serialization::StencilState backStencilState;


        //------------------------------------------------------------------------------
        /**
            Blend
        */
        //------------------------------------------------------------------------------
        bool logicOpEnabled;

        /// convert from string
        static const Serialization::LogicOp StringToLogicOp(const TransientString& str);

        Serialization::LogicOp logicOp;

        /// convert from string
        static const Serialization::BlendFactor StringToBlendFactor(const TransientString& str);

        /// convert from string
        static const Serialization::BlendOp StringToBlendOp(const TransientString& str);

        static const uint8_t NUM_BLEND_STATES = 8;
        Serialization::BlendState blendStates[NUM_BLEND_STATES];
        float blendConstants[4];

        //------------------------------------------------------------------------------
        /**
            Multisample
        */
        //------------------------------------------------------------------------------
        uint32_t samples;
        bool sampleShadingEnabled;
        float minSampleShading;
        bool alphaToCoverageEnabled;
        bool alphaToOneEnabled;
    };
};

} // namespace GPULang
