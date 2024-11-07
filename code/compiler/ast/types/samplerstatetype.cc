//------------------------------------------------------------------------------
//  @file samplerstatetype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "samplerstatetype.h"
#include "ast/expressions/uintexpression.h"

#define __BEGIN_ENUMS__() std::vector<std::string> labels; std::vector<Expression*> expressions; Function* assignOperator; Variable* arg;
#define __START_ENUM() labels.clear(); expressions.clear();
#define __ADD_ENUM(val) labels.push_back(#val); expressions.push_back(nullptr);
#define __ADD_ENUM_EXPL(name, val) labels.push_back(#name); expressions.push_back(new UIntExpression(val));
#define __FINISH_ENUM(val, key) val = Enumeration(); val.labels = labels; val.values = expressions; val.name = #key; val.type = { "u32" }; val.type.literal = true; this->staticSymbols.push_back(&val);

#define __SETUP_MEMBER(val, key, ty) val.name = #key; val.type = Type::FullType{ #ty }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; this->staticSymbols.push_back(&val);
#define __SETUP_MEMBER_ARRAY(val, key, ty, size) val.name = #key; val.type = Type::FullType{ #ty, {Type::FullType::Modifier::Array}, {size} }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; this->staticSymbols.push_back(&val);

namespace GPULang
{
SamplerStateType::SamplerStateType()
{
    __BEGIN_ENUMS__();

    __START_ENUM();
    __ADD_ENUM(InvalidFilterMode);
    __ADD_ENUM(Point);
    __ADD_ENUM(Linear);
    __ADD_ENUM_EXPL(Nearest, 1);
    __FINISH_ENUM(this->filterModeEnum, FilterMode);
    
    __START_ENUM();
    __ADD_ENUM(InvalidAddressMode);
    __ADD_ENUM(Wrap);
    __ADD_ENUM(Repeat);
    __ADD_ENUM(Mirror);
    __ADD_ENUM(Clamp);
    __ADD_ENUM(Border);
    __FINISH_ENUM(this->addressModeEnum, AddressMode);

    __START_ENUM();
    __ADD_ENUM(InvalidBorderColor);
    __ADD_ENUM(Transparent);
    __ADD_ENUM(Black);
    __ADD_ENUM(White);
    __FINISH_ENUM(this->borderColorEnum, BorderColorMode);

    __SETUP_MEMBER(this->allAddress, Address, AddressMode);
    __SETUP_MEMBER(this->addressU, AddressU, AddressMode);
    __SETUP_MEMBER(this->addressV, AddressV, AddressMode);
    __SETUP_MEMBER(this->addressW, AddressW, AddressMode);


    __SETUP_MEMBER(this->allFilter, Filter, FilterMode);
    __SETUP_MEMBER(this->minFilter, MinFilter, FilterMode);
    __SETUP_MEMBER(this->magFilter, MagFilter, FilterMode);
    __SETUP_MEMBER(this->mipFilter, MipFilter, FilterMode);

    __SETUP_MEMBER(this->mipLodBias, MipLodBias, f32);
    __SETUP_MEMBER(this->anisotropicEnabled, AnisotropicEnabled, b8);
    __SETUP_MEMBER(this->maxAnisotropy, MaxAnisotropy, f32);

    __SETUP_MEMBER(this->compareSamplerEnabled, CompareSamplerEnabled, b8);
    __SETUP_MEMBER(this->compareMode, CompareMode, CompareMode);

    __SETUP_MEMBER(this->minLod, MinLod, f32);
    __SETUP_MEMBER(this->maxLod, MaxLod, f32);
    __SETUP_MEMBER(this->borderColor, BorderColor, BorderColorMode);

    __SETUP_MEMBER(this->unnormalizedSamplingEnabled, UnnormalizedSamplingEnabled, b8);
}
} // namespace GPULang
