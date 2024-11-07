//------------------------------------------------------------------------------
//  @file generator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "generator.h"
#include "ast/symbol.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
void 
Generator::SetupIntrinsics()
{
    /// do nothing
}

//------------------------------------------------------------------------------
/**
*/
bool 
Generator::Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    return false;
}


} // namespace GPULang
