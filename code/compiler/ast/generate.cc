//------------------------------------------------------------------------------
//  @file generate.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "generate.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Generate::Generate(const PinnedArray<Symbol*>& symbols)
{
    this->symbolType = GenerateType;
    this->resolved = Alloc<Generate::__Resolved>();
    this->symbols = symbols;
    
    this->thisResolved = (Generate::__Resolved*)this->resolved;
}

//------------------------------------------------------------------------------
/**
*/
Generate::~Generate()
{
    
}

}
