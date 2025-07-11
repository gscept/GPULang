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
struct RenderState : public State
{
    /// constructor
    RenderState();

    struct __Resolved : State::__Resolved
    {
        virtual ~__Resolved() {};

        Type* typeSymbol;
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
            BlendConstantsType
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
        static const PolygonMode StringToPolygonMode(const TransientString& str);

        PolygonMode polygonMode;

        /// convert from string
        static const CullMode StringToCullMode(const TransientString& str);

        CullMode cullMode;

        /// convert from string
        static const WindingOrderMode StringToWindingOrderMode(const TransientString& str);

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
        bool scissorEnabled;
        bool stencilEnabled;

        /// convert from string
        static const StencilOp StringToStencilOp(const TransientString& str);

        StencilState frontStencilState;
        StencilState backStencilState;


        //------------------------------------------------------------------------------
        /**
            Blend
        */
        //------------------------------------------------------------------------------
        bool logicOpEnabled;

        /// convert from string
        static const LogicOp StringToLogicOp(const TransientString& str);

        LogicOp logicOp;

        /// convert from string
        static const BlendFactor StringToBlendFactor(const TransientString& str);

        /// convert from string
        static const BlendOp StringToBlendOp(const TransientString& str);

        static const uint8_t NUM_BLEND_STATES = 8;
        BlendState blendStates[NUM_BLEND_STATES];
        float blendConstants[4];
    };
};

} // namespace GPULang
