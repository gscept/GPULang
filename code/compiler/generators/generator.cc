//------------------------------------------------------------------------------
//  @file generator.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "generator.h"
#include "ast/symbol.h"
#include <util.h>
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
Generator::Generate(const Compiler* compiler, const Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc)
{
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void 
Generator::Error(const std::string& msg)
{
    this->messages.push_back(Format("Error:", msg.c_str()));
    this->hasErrors = true;
}

//------------------------------------------------------------------------------
/**
*/
void 
Generator::Warning(const std::string& msg)
{
    this->messages.push_back(Format("Warning:", msg.c_str()));
}


} // namespace GPULang
