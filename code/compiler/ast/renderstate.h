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

struct RenderState : public State
{
    /// constructor
    RenderState();

    struct __Resolved : State::__Resolved
    {

        enum RenderStateEntryType
        {
            InvalidRenderStateEntryType,
            DepthClampEnabledType,
            NoPixelsType,
            PolygonModeType,
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
            BlendConstantsType
        };

        /// convert from string to program entry type
        static const RenderStateEntryType StringToEntryType(const std::string& str);
        /// convert from mapping to string
        static const std::string& EntryTypeToString(const RenderStateEntryType type);

        //------------------------------------------------------------------------------
        /**
            Rasterizer
        */
        //------------------------------------------------------------------------------
        bool depthClampEnabled;
        bool noPixels;

        /// convert from string
        static const PolygonMode StringToPolygonMode(const std::string& str);

        PolygonMode polygonMode;

        /// convert from string
        static const CullMode StringToCullMode(const std::string& str);

        CullMode cullMode;

        /// convert from string
        static const WindingOrderMode StringToWindingOrderMode(const std::string& str);

        WindingOrderMode windingOrderMode;
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
        CompareMode depthCompare;
        bool depthBoundsTestEnabled;
        float minDepthBounds;
        float maxDepthBounds;
        bool stencilEnabled;

        /// convert from string
        static const StencilOp StringToStencilOp(const std::string& str);

        StencilState frontStencilState;
        StencilState backStencilState;


        //------------------------------------------------------------------------------
        /**
            Blend
        */
        //------------------------------------------------------------------------------
        bool logicOpEnabled;

        /// convert from string
        static const LogicOp StringToLogicOp(const std::string& str);

        LogicOp logicOp;

        /// convert from string
        static const BlendFactor StringToBlendFactor(const std::string& str);

        /// convert from string
        static const BlendOp StringToBlendOp(const std::string& str);

        static const uint8_t NUM_BLEND_STATES = 8;
        BlendState blendStates[NUM_BLEND_STATES];
        float blendConstants[4];
    };
};

} // namespace GPULang
