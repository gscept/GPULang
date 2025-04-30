//------------------------------------------------------------------------------
//  @file scopestatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "scopestatement.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
ScopeStatement::ScopeStatement(const std::vector<Symbol*>& contents)
    : symbols(contents)
{
    this->symbolType = ScopeStatementType;
    this->resolved = Alloc<ScopeStatement::__Resolved>();
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->scope.type = Scope::ScopeType::Local;
}

//------------------------------------------------------------------------------
/**
*/
ScopeStatement::~ScopeStatement()
{
    for (auto* sym : this->symbols)
        sym->~Symbol();
}

} // namespace GPULang
