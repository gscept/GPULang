//------------------------------------------------------------------------------
//  @file programtype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "programtype.h"
#include "builtins.h"

#define __SETUP_MEMBER(val, key, ty, clazz) val.name = ConstantString(#key); Symbol::Resolved(&val)->typeSymbol = &clazz##TypeType; val.type = Type::FullType{ ConstantString(#ty) }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isProgramMember = true; members.Append(&val);

namespace GPULang
{
ProgramType::ProgramType()
{
    SYMBOL_STATIC_ALLOC = true;
    this->name = ConstantString("Program");
    TransientArray<Symbol*> members(64);
    
    __SETUP_MEMBER(Program_renderState, RenderState, renderState, RenderState);
    __SETUP_MEMBER(Program_vertexShader, VertexShader, function, Function);
    __SETUP_MEMBER(Program_hullShader, HullShader, function, Function);
    __SETUP_MEMBER(Program_domainShader, DomainShader, function, Function);
    __SETUP_MEMBER(Program_geometryShader, GeometryShader, function, Function);
    __SETUP_MEMBER(Program_pixelShader, PixelShader, function, Function);
    __SETUP_MEMBER(Program_computeShader, ComputeShader, function, Function);
    this->staticSymbols = StaticArray(members);
    SYMBOL_STATIC_ALLOC = false;
}
} // namespace GPULang
