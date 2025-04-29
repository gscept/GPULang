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
