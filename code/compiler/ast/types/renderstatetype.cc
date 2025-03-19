//------------------------------------------------------------------------------
//  @file renderstatetype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "renderstatetype.h"
#include "ast/expressions/uintexpression.h"

#define __BEGIN_ENUMS__() std::vector<std::string> labels; std::vector<Expression*> expressions; Function* assignOperator; Variable* arg;
#define __START_ENUM() labels.clear(); expressions.clear();
#define __ADD_ENUM(val) labels.push_back(#val); expressions.push_back(nullptr);
#define __ADD_ENUM_EXPL(name, val) labels.push_back(#name); expressions.push_back(new UIntExpression(val));
#define __IMPL_ENUM_ASSIGN() 
#define __FINISH_ENUM(val, key) val = Enumeration(); val.labels = labels; val.values = expressions; val.name = #key; val.baseType = TypeCode::UInt; val.type = Type::FullType{ "u32" }; val.type.literal = true; this->staticSymbols.push_back(&val);

#define __SETUP_MEMBER(val, key, ty) val.name = #key; val.type = Type::FullType{ #ty }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);
#define __SETUP_MEMBER_ARRAY(val, key, ty, size) val.name = #key; val.type = Type::FullType{ #ty, {Type::FullType::Modifier::Array}, {new UIntExpression(size)} }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
RenderStateType::RenderStateType()
{
    this->category = Type::Category::RenderStateCategory;

    __BEGIN_ENUMS__();

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Fill);
    __ADD_ENUM(Line);
    __ADD_ENUM(Point);
    __FINISH_ENUM(this->polygonModeEnum, PolygonMode);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(None);
    __ADD_ENUM(Front);
    __ADD_ENUM(Back);
    __ADD_ENUM(FrontAndBack);
    __FINISH_ENUM(this->cullModeEnum, CullFace);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Clockwise);
    __ADD_ENUM(CounterClockwise);
    __FINISH_ENUM(this->windingOrderModeEnum, WindingOrder);

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
    __FINISH_ENUM(this->logicOpModeEnum, LogicOp);

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
    __FINISH_ENUM(this->blendFactorModeEnum, BlendFactor);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(Add);
    __ADD_ENUM(Subtract);
    __ADD_ENUM(ReverseSubtract);
    __ADD_ENUM(Min);
    __ADD_ENUM(Max);
    __FINISH_ENUM(this->blendOpModeEnum, BlendOp);

    __START_ENUM();
    __ADD_ENUM(Invalid);
    __ADD_ENUM(X);
    __ADD_ENUM(XY);
    __ADD_ENUM(XYZ);
    __ADD_ENUM(XYZW);
    __FINISH_ENUM(this->colorComponentMaskEnum, ColorComponentMask);

    __SETUP_MEMBER(this->depthClampEnabled, DepthClampEnabled, b8);
    __SETUP_MEMBER(this->noRasterization, NoRasterization, b8);
    __SETUP_MEMBER(this->polygonMode, Fill, PolygonMode);
    __SETUP_MEMBER(this->cullMode, Cull, CullFace);
    __SETUP_MEMBER(this->windingOrder, Winding, WindingOrder);
    __SETUP_MEMBER(this->depthBiasEnabled, DepthBiasEnabled, b8);
    __SETUP_MEMBER(this->depthBiasFactor, DepthBiasFactor, f32);
    __SETUP_MEMBER(this->depthBiasClamp, DepthBiasClamp, f32);
    __SETUP_MEMBER(this->depthBiasSlopeFactor, DepthBiasSlopeFactor, f32);
    __SETUP_MEMBER(this->lineWidth, LineWidth, f32);
    __SETUP_MEMBER(this->depthTestEnabled, DepthTestEnabled, b8);
    __SETUP_MEMBER(this->depthWriteEnabled, DepthWriteEnabled, b8);
    __SETUP_MEMBER(this->depthTestFunction, DepthTestFunction, CompareMode);
    __SETUP_MEMBER(this->depthBoundsTestEnabled, DepthBoundsTestEnabled, b8);
    __SETUP_MEMBER(this->minDepthBounds, MinDepthBounds, f32);
    __SETUP_MEMBER(this->maxDepthBounds, MaxDepthBounds, f32);
    __SETUP_MEMBER(this->stencilEnabled, StencilEnabled, b8);
    __SETUP_MEMBER(this->logicOpEnabled, LogicEnabled, b8);
    __SETUP_MEMBER(this->logicOp, Logic, LogicOp);
    __SETUP_MEMBER(this->frontStencilState, StencilFront, stencilState);
    __SETUP_MEMBER(this->backStencilState, Stencil, stencilState);
    __SETUP_MEMBER_ARRAY(this->blendEnabled, BlendEnabled, b8, 8u);
    __SETUP_MEMBER_ARRAY(this->sourceBlend, SourceBlend, BlendFactor, 8u);
    __SETUP_MEMBER_ARRAY(this->destinationBlend, DestinationBlend, BlendFactor, 8u);
    __SETUP_MEMBER_ARRAY(this->sourceAlphaBlend, SourceAlphaBlend, BlendFactor, 8u);
    __SETUP_MEMBER_ARRAY(this->destinationAlphaBlend, DestinationAlphaBlend, BlendFactor, 8u);
    __SETUP_MEMBER_ARRAY(this->blendOp, ColorBlendOp, BlendOp, 8u);
    __SETUP_MEMBER_ARRAY(this->blendOpAlpha, AlphaBlendOp, BlendOp, 8u);
    __SETUP_MEMBER_ARRAY(this->colorComponentMask, Mask, ColorComponentMask, 8u);
}


} // namespace GPULang


