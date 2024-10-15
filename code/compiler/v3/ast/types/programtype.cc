//------------------------------------------------------------------------------
//  @file programtype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "programtype.h"

#define __SETUP_MEMBER(val, key, ty) val.name = #key; val.type = Type::FullType{ #ty }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; this->staticSymbols.push_back(&val);

namespace GPULang
{
ProgramType::ProgramType()
{
    __SETUP_MEMBER(this->renderState, RenderState, renderState);
    __SETUP_MEMBER(this->vertexShader, VertexShader, function);
    __SETUP_MEMBER(this->hullShader, HullShader, function);
    __SETUP_MEMBER(this->domainShader, DomainShader, function);
    __SETUP_MEMBER(this->geometryShader, GeometryShader, function);
    __SETUP_MEMBER(this->pixelShader, PixelShader, function);
    __SETUP_MEMBER(this->computeShader, ComputeShader, function);
}
} // namespace GPULang
