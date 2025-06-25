//------------------------------------------------------------------------------
//  @file renderstatetype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "renderstatetype.h"
#include "ast/expressions/uintexpression.h"
#include "builtins.h"

#define __BEGIN_ENUMS__() TransientArray<ConstantString> labels(32); TransientArray<Expression*> expressions(32); Function* assignOperator; Variable* arg;
#define __START_ENUM() labels.Clear(); expressions.Clear();
#define __ADD_ENUM(val) labels.Append(ConstantString(#val)); expressions.Append(nullptr);
#define __ADD_ENUM_EXPL(name, val) labels.Append(ConstantString(#name)); expressions.Append(StaticAlloc<UIntExpression>(val));
#define __IMPL_ENUM_ASSIGN()
#define __FINISH_ENUM(val, key) new (&val) Enumeration(); Symbol::Resolved(&val)->typeSymbol = &UIntType; val.builtin = true; val.labels = StaticArray<FixedString>(labels); val.values = StaticArray<Expression*>(expressions); val.name = ConstantString(#key); val.baseType = TypeCode::UInt; val.type = Type::FullType{ ConstantString("u32") }; val.type.literal = true; this->staticSymbols.push_back(&val);

#define __SETUP_MEMBER(val, key, ty) val.name = ConstantString(#key); val.type = Type::FullType{ ConstantString(#ty) }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);
#define __SETUP_MEMBER_ARRAY(val, key, ty, size) val.name = ConstantString(#key); val.type = Type::FullType{ ConstantString(#ty), {Type::FullType::Modifier::Array}, {StaticAlloc<UIntExpression>(size)} }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
RenderStateType::RenderStateType()
{
    SYMBOL_STATIC_ALLOC = true;

    this->category = Type::Category::RenderStateCategory;

    __BEGIN_ENUMS__();

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Fill);
    __ADD_ENUM(Line);
    __ADD_ENUM(Point);
    __FINISH_ENUM(RenderState_polygonModeEnum, PolygonMode);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(None);
    __ADD_ENUM(Front);
    __ADD_ENUM(Back);
    __ADD_ENUM(FrontAndBack);
    __FINISH_ENUM(RenderState_cullModeEnum, CullFace);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Clockwise);
    __ADD_ENUM(CounterClockwise);
    __FINISH_ENUM(RenderState_windingOrderModeEnum, WindingOrder);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Clear);
    __ADD_ENUM(And);
    __ADD_ENUM(AndReverse);
    __ADD_ENUM(Copy);
    __ADD_ENUM(AndInverted);
    __ADD_ENUM(No);
    __ADD_ENUM(Xor);
    __ADD_ENUM(Or);
    __ADD_ENUM(Nor);
    __ADD_ENUM(Equivalent);
    __ADD_ENUM(Invert);
    __ADD_ENUM(OrReverse);
    __ADD_ENUM(CopyInverted);
    __ADD_ENUM(OrInverted);
    __ADD_ENUM(Nand);
    __ADD_ENUM(Set);
    __FINISH_ENUM(RenderState_logicOpModeEnum, LogicOp);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Zero);
    __ADD_ENUM(One);
    __ADD_ENUM(SourceColor);
    __ADD_ENUM(OneMinusSourceColor);
    __ADD_ENUM(DestinationColor);
    __ADD_ENUM(OneMinusDestinationColor);
    __ADD_ENUM(SourceAlpha);
    __ADD_ENUM(OneMinusSourceAlpha);
    __ADD_ENUM(DestinationAlpha);
    __ADD_ENUM(OneMinusDestinationAlpha);
    __ADD_ENUM(ConstantColor);
    __ADD_ENUM(OneMinusConstantColor);
    __ADD_ENUM(ConstantAlpha);
    __ADD_ENUM(OneMinusConstantAlpha);
    __ADD_ENUM(SourceAlphaSaturate);
    __ADD_ENUM(Source1Color);
    __ADD_ENUM(OneMinusSource1Color);
    __ADD_ENUM(Source1Alpha);
    __ADD_ENUM(OneMinusSource1Alpha);
    __FINISH_ENUM(RenderState_blendFactorModeEnum, BlendFactor);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Add);
    __ADD_ENUM(Subtract);
    __ADD_ENUM(ReverseSubtract);
    __ADD_ENUM(Min);
    __ADD_ENUM(Max);
    __FINISH_ENUM(RenderState_blendOpModeEnum, BlendOp);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(X);
    __ADD_ENUM(XY);
    __ADD_ENUM(XYZ);
    __ADD_ENUM(XYZW);
    __FINISH_ENUM(RenderState_colorComponentMaskEnum, ColorComponentMask);

    __SETUP_MEMBER(RenderState_depthClampEnabled, DepthClampEnabled, b8);
    __SETUP_MEMBER(RenderState_noRasterization, NoRasterization, b8);
    __SETUP_MEMBER(RenderState_polygonMode, Fill, PolygonMode);
    __SETUP_MEMBER(RenderState_cullMode, Cull, CullFace);
    __SETUP_MEMBER(RenderState_windingOrder, Winding, WindingOrder);
    __SETUP_MEMBER(RenderState_depthBiasEnabled, DepthBiasEnabled, b8);
    __SETUP_MEMBER(RenderState_depthBiasFactor, DepthBiasFactor, f32);
    __SETUP_MEMBER(RenderState_depthBiasClamp, DepthBiasClamp, f32);
    __SETUP_MEMBER(RenderState_depthBiasSlopeFactor, DepthBiasSlopeFactor, f32);
    __SETUP_MEMBER(RenderState_lineWidth, LineWidth, f32);
    __SETUP_MEMBER(RenderState_depthTestEnabled, DepthTestEnabled, b8);
    __SETUP_MEMBER(RenderState_depthWriteEnabled, DepthWriteEnabled, b8);
    __SETUP_MEMBER(RenderState_depthTestFunction, DepthTestFunction, CompareMode);
    __SETUP_MEMBER(RenderState_depthBoundsTestEnabled, DepthBoundsTestEnabled, b8);
    __SETUP_MEMBER(RenderState_minDepthBounds, MinDepthBounds, f32);
    __SETUP_MEMBER(RenderState_maxDepthBounds, MaxDepthBounds, f32);
    __SETUP_MEMBER(RenderState_scissorEnabled, ScissorEnabled, b8);
    __SETUP_MEMBER(RenderState_stencilEnabled, StencilEnabled, b8);
    __SETUP_MEMBER(RenderState_logicOpEnabled, LogicEnabled, b8);
    __SETUP_MEMBER(RenderState_logicOp, Logic, LogicOp);
    __SETUP_MEMBER(RenderState_frontStencilState, StencilFront, stencilState);
    __SETUP_MEMBER(RenderState_backStencilState, Stencil, stencilState);
    __SETUP_MEMBER_ARRAY(RenderState_blendEnabled, BlendEnabled, b8, 8u);
    __SETUP_MEMBER_ARRAY(RenderState_sourceBlend, SourceBlend, BlendFactor, 8u);
    __SETUP_MEMBER_ARRAY(RenderState_destinationBlend, DestinationBlend, BlendFactor, 8u);
    __SETUP_MEMBER_ARRAY(RenderState_sourceAlphaBlend, SourceAlphaBlend, BlendFactor, 8u);
    __SETUP_MEMBER_ARRAY(RenderState_destinationAlphaBlend, DestinationAlphaBlend, BlendFactor, 8u);
    __SETUP_MEMBER_ARRAY(RenderState_blendOp, ColorBlendOp, BlendOp, 8u);
    __SETUP_MEMBER_ARRAY(RenderState_blendOpAlpha, AlphaBlendOp, BlendOp, 8u);
    __SETUP_MEMBER_ARRAY(RenderState_colorComponentMask, Mask, ColorComponentMask, 8u);

    SYMBOL_STATIC_ALLOC = false;
}
} // namespace GPULang


