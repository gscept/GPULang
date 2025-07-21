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
    Symbol* sym = compiler->GetType(Type::FullType{this->type});
    if (sym == nullptr)
    {
        compiler->UnrecognizedTypeError(TransientString(this->type), this);
        return false;
    }
    return compiler->AddSymbol(this->name, sym);
}

} // namespace GPULang
