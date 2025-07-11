//------------------------------------------------------------------------------
//  @file stencilstatetype.cc
//  @copyright (C) 2021 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "stencilstatetype.h"
#include "builtins.h"

#define __SETUP_MEMBER(val, key, ty) val.name = ConstantString(#key); val.type = Type::FullType{ ty.name }; Symbol::Resolved(&val)->typeSymbol = &ty; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; members.Append(&val);
#define __SETUP_MEMBER_ARRAY(val, key, ty, size) val.name = ConstantString(#key); val.type = Type::FullType{ ty.name, {Type::FullType::Modifier::Array}, {StaticAlloc<UIntExpression>(size)} }; Symbol::Resolved(&val)->typeSymbol = &ty; Symbol::Resolved(&val)->usageBits.flags.isVar = true; Symbol::Resolved(&val)->usageBits.flags.isStructMember = true; members.Append(&val);

namespace GPULang
{



//------------------------------------------------------------------------------
/**
*/
StencilStateType::StencilStateType()
{
    SYMBOL_STATIC_ALLOC = true;
    this->name = ConstantString("StencilState");
    this->category = Type::Category::StencilStateCategory;
    
    TransientArray<Symbol*> members(7);
    
    __SETUP_MEMBER(StencilState_fail, Fail, StencilOpTypeType);
    __SETUP_MEMBER(StencilState_pass, Pass, StencilOpTypeType);
    __SETUP_MEMBER(StencilState_depthFail, DepthFail, StencilOpTypeType);
    __SETUP_MEMBER(StencilState_compare, Compare, CompareModeTypeType);
    __SETUP_MEMBER(StencilState_compareMask, CompareMask, UIntType);
    __SETUP_MEMBER(StencilState_writeMask, WriteMask, UIntType);
    __SETUP_MEMBER(StencilState_referenceMask, ReferenceMask, UIntType);
    
    this->staticSymbols = StaticArray(members);
    SYMBOL_STATIC_ALLOC = false;
}


} // namespace GPULang
