#pragma once
//------------------------------------------------------------------------------
/**
    RenderState type

    @copyright (C) 2021 Gustav Sterbrant
*/  
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/variable.h"
#include "ast/enumeration.h"
namespace GPULang
{

struct RenderStateType : public Type
{
    /// Constructor
    RenderStateType();
};
extern RenderStateType RenderStateTypeType;

extern Enumeration RenderState_polygonModeEnum;
extern Enumeration RenderState_cullModeEnum;
extern Enumeration RenderState_windingOrderModeEnum;
extern Enumeration RenderState_logicOpModeEnum;
extern Enumeration RenderState_blendFactorModeEnum;
extern Enumeration RenderState_blendOpModeEnum;
extern Enumeration RenderState_colorComponentMaskEnum;

extern Variable RenderState_depthClampEnabled;
extern Variable RenderState_noRasterization;
extern Variable RenderState_polygonMode;
extern Variable RenderState_cullMode;
extern Variable RenderState_windingOrder;
extern Variable RenderState_depthBiasEnabled;
extern Variable RenderState_depthBiasFactor;
extern Variable RenderState_depthBiasClamp;
extern Variable RenderState_depthBiasSlopeFactor;
extern Variable RenderState_lineWidth;
extern Variable RenderState_depthTestEnabled;
extern Variable RenderState_depthWriteEnabled;
extern Variable RenderState_depthTestFunction;
extern Variable RenderState_depthBoundsTestEnabled;
extern Variable RenderState_minDepthBounds;
extern Variable RenderState_maxDepthBounds;
extern Variable RenderState_scissorEnabled;
extern Variable RenderState_stencilEnabled;
extern Variable RenderState_logicOpEnabled;
extern Variable RenderState_logicOp;
extern Variable RenderState_blendEnabled;
extern Variable RenderState_sourceBlend;
extern Variable RenderState_destinationBlend;
extern Variable RenderState_sourceAlphaBlend;
extern Variable RenderState_destinationAlphaBlend;
extern Variable RenderState_blendOp;
extern Variable RenderState_blendOpAlpha;
extern Variable RenderState_colorComponentMask;
extern Variable RenderState_frontStencilState;
extern Variable RenderState_backStencilState;

} // namespace GPULang
