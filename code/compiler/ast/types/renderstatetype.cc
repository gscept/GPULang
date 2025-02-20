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
#define __FINISH_ENUM(val, key) val = Enumeration(); val.labels = labels; val.values = expressions; val.name = #key; val.type = Type::FullType{ "u32" }; val.type.literal = true; this->staticSymbols.push_back(&val);

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
    __ADD_ENUM(InvalidFillMode);
    __ADD_ENUM(FillMode);
    __ADD_ENUM(LineMode);
    __ADD_ENUM(PointMode);
    __FINISH_ENUM(this->polygonModeEnum, PolygonMode);

    __START_ENUM();
    __ADD_ENUM(InvalidCullMode);
    __ADD_ENUM(None);
    __ADD_ENUM(Front);
    __ADD_ENUM(Back);
    __ADD_ENUM(FrontAndBack);
    __FINISH_ENUM(this->cullModeEnum, CullMode);

    __START_ENUM();
    __ADD_ENUM(InvalidWindingOrderMode);
    __ADD_ENUM(ClockwiseMode);
    __ADD_ENUM(CounterClockwiseMode);
    __FINISH_ENUM(this->windingOrderModeEnum, WindingOrderMode);

    __START_ENUM();
    __ADD_ENUM(InvalidStencilOpMode);
    __ADD_ENUM(StencilKeepOp);
    __ADD_ENUM(StencilZeroOp);
    __ADD_ENUM(StencilReplaceOp);
    __ADD_ENUM(StencilIncrementClampOp);
    __ADD_ENUM(StencilDecrementClampOp);
    __ADD_ENUM(StencilInvertOp);
    __ADD_ENUM(StencilIncrementWrapOp);
    __ADD_ENUM(StencilDecrementWrapOp);
    __FINISH_ENUM(this->stencilOpModeEnum, StencilOpMode);

    __START_ENUM();
    __ADD_ENUM(InvalidLogicOpMode);
    __ADD_ENUM(LogicClearOp);
    __ADD_ENUM(LogicAndOp);
    __ADD_ENUM(LogicAndReverseOp);
    __ADD_ENUM(LogicCopyOp);
    __ADD_ENUM(LogicAndInvertedOp);
    __ADD_ENUM(LogicNoOp);
    __ADD_ENUM(LogicXorOp);
    __ADD_ENUM(LogicOrOp);
    __ADD_ENUM(LogicNorOp);
    __ADD_ENUM(LogicEquivalentOp);
    __ADD_ENUM(LogicInvertOp);
    __ADD_ENUM(LogicOrReverseOp);
    __ADD_ENUM(LogicCopyInvertedOp);
    __ADD_ENUM(LogicOrInvertedOp);
    __ADD_ENUM(LogicNandOp);
    __ADD_ENUM(LogicSetOp);
    __FINISH_ENUM(this->logicOpModeEnum, LogicOpMode);

    __START_ENUM();
    __ADD_ENUM(InvalidBlendFactorMode);
    __ADD_ENUM(ZeroFactor);
    __ADD_ENUM(OneFactor);
    __ADD_ENUM(SourceColorFactor);
    __ADD_ENUM(OneMinusSourceColorFactor);
    __ADD_ENUM(DestinationColorFactor);
    __ADD_ENUM(OneMinusDestinationColorFactor);
    __ADD_ENUM(SourceAlphaFactor);
    __ADD_ENUM(OneMinusSourceAlphaFactor);
    __ADD_ENUM(DestinationAlphaFactor);
    __ADD_ENUM(OneMinusDestinationAlphaFactor);
    __ADD_ENUM(ConstantColorFactor);
    __ADD_ENUM(OneMinusConstantColorFactor);
    __ADD_ENUM(ConstantAlphaFactor);
    __ADD_ENUM(OneMinusConstantAlphaFactor);
    __ADD_ENUM(SourceAlphaSaturateFactor);
    __ADD_ENUM(Source1ColorFactor);
    __ADD_ENUM(OneMinusSource1ColorFactor);
    __ADD_ENUM(Source1AlphaFactor);
    __ADD_ENUM(OneMinusSource1AlphaFactor);
    __FINISH_ENUM(this->blendFactorModeEnum, BlendFactorMode);

    __START_ENUM();
    __ADD_ENUM(InvalidBlendOpMode);
    __ADD_ENUM(AddOp);
    __ADD_ENUM(SubtractOp);
    __ADD_ENUM(ReverseSubtractOp);
    __ADD_ENUM(MinOp);
    __ADD_ENUM(MaxOp);
    __FINISH_ENUM(this->blendOpModeEnum, BlendOpMode);

    __START_ENUM();
    __ADD_ENUM(InvalidColorComponentMaskMode);
    __ADD_ENUM(X);
    __ADD_ENUM(XY);
    __ADD_ENUM(XYZ);
    __ADD_ENUM(XYZW);
    __FINISH_ENUM(this->colorComponentMaskEnum, ColorComponentMaskMode);

    std::vector<Symbol*> stencilStateMembers;
    Variable* var = nullptr;
    var = new Variable(); var->type = Type::FullType{ "StencilOpMode" }; var->name = "Fail";
    stencilStateMembers.push_back(var);
    var = new Variable(); var->type = Type::FullType{ "StencilOpMode" }; var->name = "Pass";
    stencilStateMembers.push_back(var);
    var = new Variable(); var->type = Type::FullType{ "StencilOpMode" }; var->name = "DepthFail";
    stencilStateMembers.push_back(var);
    var = new Variable(); var->type = Type::FullType{ "CompareMode" }; var->name = "Compare";
    stencilStateMembers.push_back(var);
    var = new Variable(); var->type = Type::FullType{ "u32" }; var->name = "CompareMask";
    stencilStateMembers.push_back(var);
    var = new Variable(); var->type = Type::FullType{ "u32" }; var->name = "WriteMask";
    stencilStateMembers.push_back(var);
    var = new Variable(); var->type = Type::FullType{ "u32" }; var->name = "ReferenceMask";
    stencilStateMembers.push_back(var);
    this->stencilState.symbols = stencilStateMembers;
    this->stencilState.name = "StencilState";
    this->staticSymbols.push_back(&this->stencilState);

    __SETUP_MEMBER(this->depthClampEnabled, DepthClampEnabled, b8);
    __SETUP_MEMBER(this->noRasterization, NoRasterization, b8);
    __SETUP_MEMBER(this->polygonMode, Polygon, PolygonMode);
    __SETUP_MEMBER(this->cullMode, Cull, CullMode);
    __SETUP_MEMBER(this->windingOrder, WindingOrder, WindingOrderMode);
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
    __SETUP_MEMBER(this->logicOpEnabled, LogicOpEnabled, b8);
    __SETUP_MEMBER(this->logicOp, LogicOp, LogicOpMode);
    __SETUP_MEMBER(this->frontStencilState, FrontStencil, StencilState);
    __SETUP_MEMBER(this->backStencilState, BackStencil, StencilState);
    __SETUP_MEMBER_ARRAY(this->blendEnabled, BlendEnabled, b8, 8u);
    __SETUP_MEMBER_ARRAY(this->sourceBlend, SourceBlend, BlendFactorMode, 8u);
    __SETUP_MEMBER_ARRAY(this->destinationBlend, DestinationBlend, BlendFactorMode, 8u);
    __SETUP_MEMBER_ARRAY(this->sourceAlphaBlend, SourceAlphaBlend, BlendFactorMode, 8u);
    __SETUP_MEMBER_ARRAY(this->destinationAlphaBlend, DestinationAlphaBlend, BlendFactorMode, 8u);
    __SETUP_MEMBER_ARRAY(this->blendOp, BlendOp, BlendOpMode, 8u);
    __SETUP_MEMBER_ARRAY(this->blendOpAlpha, BlendOpAlpha, BlendOpMode, 8u);
    __SETUP_MEMBER_ARRAY(this->colorComponentMask, ColorComponentMask, ColorComponentMaskMode, 8u);
}


} // namespace GPULang


