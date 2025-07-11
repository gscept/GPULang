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
ScopeStatement::ScopeStatement(PinnedArray<Symbol*>&& contents, const std::vector<Expression*>& unfinished)
    : symbols(std::move(contents))
    , unfinished(unfinished)
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
    for (auto* expr : this->unfinished)
        expr->~Expression();
    
    this->symbols.Invalidate();
}

} // namespace GPULang
