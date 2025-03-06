#pragma once
//------------------------------------------------------------------------------
/**
    RenderState type

    @copyright (C) 2021 Gustav Sterbrant
*/  
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
#include "ast/variable.h"
#include "ast/enumeration.h"
#include "ast/structure.h"
namespace GPULang
{

struct RenderStateType : public Type
{
    /// Constructor
    RenderStateType();

    Enumeration polygonModeEnum;
    Enumeration cullModeEnum;
    Enumeration windingOrderModeEnum;
    Enumeration logicOpModeEnum;
    Enumeration blendFactorModeEnum;
    Enumeration blendOpModeEnum;
    Enumeration stencilOpModeEnum;
    Enumeration colorComponentMaskEnum;

    Variable depthClampEnabled;
    Variable noRasterization;
    Variable polygonMode;
    Variable cullMode;
    Variable windingOrder;
    Variable depthBiasEnabled;
    Variable depthBiasFactor;
    Variable depthBiasClamp;
    Variable depthBiasSlopeFactor;
    Variable lineWidth;
    Variable depthTestEnabled;
    Variable depthWriteEnabled;
    Variable depthTestFunction;
    Variable depthBoundsTestEnabled;
    Variable minDepthBounds;
    Variable maxDepthBounds;
    Variable stencilEnabled;
    Variable logicOpEnabled;
    Variable logicOp;
    Variable blendEnabled;
    Variable sourceBlend;
    Variable destinationBlend;
    Variable sourceAlphaBlend;
    Variable destinationAlphaBlend;
    Variable blendOp;
    Variable blendOpAlpha;
    Variable colorComponentMask;
    Variable frontStencilFailOp;
    Variable frontStencilPassOp;
    Variable frontStencilDepthFailOp;
    Variable frontStencilCompareMode;
    Variable frontStencilCompareMask;
    Variable frontStencilWriteMask;
    Variable frontStencilReferenceMask;
    Variable backStencilFailOp;
    Variable backStencilPassOp;
    Variable backStencilDepthFailOp;
    Variable backStencilCompareMode;
    Variable backStencilCompareMask;
    Variable backStencilWriteMask;
    Variable backStencilReferenceMask;

};

} // namespace GPULang
