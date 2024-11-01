//------------------------------------------------------------------------------
//  @file alias.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "alias.h"
#include "compiler.h"
#include "util.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
Alias::Alias()
{
    this->resolved = nullptr;
    this->symbolType = Symbol::SymbolType::AliasType;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Alias::Resolve(Compiler* compiler)
{
    Symbol* sym = compiler->GetSymbol(this->type);
    if (sym == nullptr)
    {
        compiler->Error(Format("Unrecognized type '%s'", this->type.c_str()), this);
        return false;
    }
    return compiler->AddSymbol(this->name, sym);
}

} // namespace GPULang
