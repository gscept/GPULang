//------------------------------------------------------------------------------
//  @file preprocessor.cc
//  @copyright (C) 2025 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "preprocessor.h"

namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Preprocessor::Preprocessor()
{
    this->resolved = nullptr;
    this->symbolType = Symbol::SymbolType::PreprocessorType;
}

//------------------------------------------------------------------------------
/**
*/
Preprocessor::~Preprocessor()
{
}

}