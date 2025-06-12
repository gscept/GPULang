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
Generate::Generate()
{
    this->symbolType = GenerateType;
    this->resolved = Alloc<Generate::__Resolved>();
    
    this->thisResolved = (Generate::__Resolved*)this->resolved;
}

//------------------------------------------------------------------------------
/**
*/
Generate::~Generate()
{
    
}

}
