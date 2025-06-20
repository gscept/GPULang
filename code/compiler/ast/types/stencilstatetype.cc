//------------------------------------------------------------------------------
//  @file stencilstatetype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "stencilstatetype.h"

#define __SETUP_MEMBER(val, key, ty) val.name = ConstantString(#key); val.type = Type::FullType{ #ty }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);
#define __SETUP_MEMBER_ARRAY(val, key, ty, size) val.name = ConstantString(#key); val.type = Type::FullType{ #ty, {Type::FullType::Modifier::Array}, {StaticAlloc<UIntExpression>(size)} }; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; this->staticSymbols.push_back(&val);

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
StencilStateType::StencilStateType()
{
    this->category = Type::Category::StencilStateCategory;
    
    __SETUP_MEMBER(this->fail, Fail, StencilOp);
    __SETUP_MEMBER(this->pass, Pass, StencilOp);
    __SETUP_MEMBER(this->depthFail, DepthFail, StencilOp);
    __SETUP_MEMBER(this->compare, Compare, CompareMode);
    __SETUP_MEMBER(this->compareMask, CompareMask, u32);
    __SETUP_MEMBER(this->writeMask, WriteMask, u32);
    __SETUP_MEMBER(this->referenceMask, ReferenceMask, u32);
}


} // namespace GPULang
