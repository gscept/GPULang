//------------------------------------------------------------------------------
//  @file programtype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "programtype.h"

#define __SETUP_MEMBER(val, key, ty) val.name = ConstantString(#key); val.type = Type::FullType{ ConstantString(#ty) }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isProgramMember = true; this->staticSymbols.push_back(&val);

namespace GPULang
{
ProgramType::ProgramType()
{
    SYMBOL_STATIC_ALLOC = true;

    __SETUP_MEMBER(Program_renderState, RenderState, renderState);
    __SETUP_MEMBER(Program_vertexShader, VertexShader, function);
    __SETUP_MEMBER(Program_hullShader, HullShader, function);
    __SETUP_MEMBER(Program_domainShader, DomainShader, function);
    __SETUP_MEMBER(Program_geometryShader, GeometryShader, function);
    __SETUP_MEMBER(Program_pixelShader, PixelShader, function);
    __SETUP_MEMBER(Program_computeShader, ComputeShader, function);

    SYMBOL_STATIC_ALLOC = false;
}
} // namespace GPULang
