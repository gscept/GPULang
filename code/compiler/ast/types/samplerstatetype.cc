//------------------------------------------------------------------------------
//  @file samplerstatetype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "samplerstatetype.h"
#include "ast/expressions/uintexpression.h"
#include "builtins.h"

#define __BEGIN_ENUMS__() TransientArray<ConstantString> labels(256); TransientArray<Expression*> expressions(256); Function* assignOperator; Variable* arg;
#define __START_ENUM() labels.Clear(); expressions.Clear();
#define __ADD_ENUM(val) labels.Append(ConstantString(#val)); expressions.Append(nullptr);
#define __ADD_ENUM_EXPL(name, val) labels.Append(ConstantString(#name)); expressions.Append(StaticAlloc<UIntExpression>(val));
#define __FINISH_ENUM(val, key) new (&val) Enumeration(); Symbol::Resolved(&val)->typeSymbol = &UIntType; val.builtin = true; val.labels = StaticArray<FixedString>(labels); val.values = StaticArray<Expression*>(expressions); val.name = ConstantString(#key); val.baseType = TypeCode::UInt; val.type = Type::FullType{ ConstantString("u32") }; val.type.literal = true; this->staticSymbols.push_back(&val);

#define __SETUP_MEMBER(val, key, ty) val.name = ConstantString(#key); val.type = Type::FullType{ ConstantString(#ty) }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);
#define __SETUP_MEMBER_ARRAY(val, key, ty, size) val.name = ConstantString(#key); val.type = Type::FullType{ ConstantString(#ty), {Type::FullType::Modifier::Array}, {size} }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);

namespace GPULang
{


SamplerStateType::SamplerStateType()
{
    SYMBOL_STATIC_ALLOC = true;
    this->category = Type::Category::SamplerCategory;

    __BEGIN_ENUMS__();

    __START_ENUM();
    __ADD_ENUM_EXPL(InvalidFilterMode, 0xFFFFFFFF);
    __ADD_ENUM_EXPL(Point, 0x0);
    __ADD_ENUM_EXPL(Nearest, 0x0);
    __ADD_ENUM_EXPL(Linear, 0x7);
    __ADD_ENUM_EXPL(MinMagMipLinear, 0x7);
    __ADD_ENUM_EXPL(MinMagLinearMipPoint, 0x3);
    __ADD_ENUM_EXPL(MinLinearMagMipPoint, 0x1);
    __ADD_ENUM_EXPL(MinMagMipPoint, 0x0);
    __ADD_ENUM_EXPL(MinMagPointMipLinear, 0x4);
    __ADD_ENUM_EXPL(MinPointMagMipLinear, 0x6);
    __ADD_ENUM_EXPL(MinLinearMagPointMipLinear, 0x5);
    __ADD_ENUM_EXPL(MinPointMagLinearMipPoint, 0x2);
    __FINISH_ENUM(SamplerState_filterModeEnum, FilterMode);
    
    __START_ENUM();
    __ADD_ENUM(InvalidAddressMode);
    __ADD_ENUM(Repeat);
    __ADD_ENUM(Mirror);
    __ADD_ENUM(Clamp);
    __ADD_ENUM(Border);
    __FINISH_ENUM(SamplerState_addressModeEnum, AddressMode);

    __START_ENUM();
    __ADD_ENUM(InvalidColor);
    __ADD_ENUM(Transparent);
    __ADD_ENUM(Black);
    __ADD_ENUM(White);
    __FINISH_ENUM(SamplerState_colorEnum, Color);

    __SETUP_MEMBER(SamplerState_allAddress, Address, AddressMode);
    __SETUP_MEMBER(SamplerState_addressU, AddressU, AddressMode);
    __SETUP_MEMBER(SamplerState_addressV, AddressV, AddressMode);
    __SETUP_MEMBER(SamplerState_addressW, AddressW, AddressMode);

    __SETUP_MEMBER(SamplerState_allFilter, Filter, FilterMode);
    __SETUP_MEMBER(SamplerState_minFilter, MinFilter, FilterMode);
    __SETUP_MEMBER(SamplerState_magFilter, MagFilter, FilterMode);
    __SETUP_MEMBER(SamplerState_mipFilter, MipFilter, FilterMode);

    __SETUP_MEMBER(SamplerState_mipLodBias, MipLodBias, f32);
    __SETUP_MEMBER(SamplerState_anisotropicEnabled, AnisotropicEnabled, b8);
    __SETUP_MEMBER(SamplerState_maxAnisotropy, MaxAnisotropy, f32);

    __SETUP_MEMBER(SamplerState_compareSamplerEnabled, CompareEnabled, b8);
    __SETUP_MEMBER(SamplerState_compareMode, CompareMode, CompareMode);

    __SETUP_MEMBER(SamplerState_minLod, MinLod, f32);
    __SETUP_MEMBER(SamplerState_maxLod, MaxLod, f32);
    __SETUP_MEMBER(SamplerState_borderColor, Border, Color);

    __SETUP_MEMBER(SamplerState_unnormalizedSamplingEnabled, UnnormalizedSamplingEnabled, b8);

    SYMBOL_STATIC_ALLOC = false;
}
} // namespace GPULang
