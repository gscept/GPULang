//------------------------------------------------------------------------------
//  @file stencilstatetype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "stencilstatetype.h"

#define __SETUP_MEMBER(val, key, ty) val.name = ConstantString(#key); val.type = Type::FullType{ ConstantString(#ty) }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);
#define __SETUP_MEMBER_ARRAY(val, key, ty, size) val.name = ConstantString(#key); val.type = Type::FullType{ ConstantString(#ty), {Type::FullType::Modifier::Array}, {StaticAlloc<UIntExpression>(size)} }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);

namespace GPULang
{



//------------------------------------------------------------------------------
/**
*/
StencilStateType::StencilStateType()
{
    SYMBOL_STATIC_ALLOC = true;
    this->category = Type::Category::StencilStateCategory;
    
    __SETUP_MEMBER(StencilState_fail, Fail, StencilOp);
    __SETUP_MEMBER(StencilState_pass, Pass, StencilOp);
    __SETUP_MEMBER(StencilState_depthFail, DepthFail, StencilOp);
    __SETUP_MEMBER(StencilState_compare, Compare, CompareMode);
    __SETUP_MEMBER(StencilState_compareMask, CompareMask, u32);
    __SETUP_MEMBER(StencilState_writeMask, WriteMask, u32);
    __SETUP_MEMBER(StencilState_referenceMask, ReferenceMask, u32);
    SYMBOL_STATIC_ALLOC = false;
}


} // namespace GPULang
