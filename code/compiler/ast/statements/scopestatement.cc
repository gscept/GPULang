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
    this->resolved = new ScopeStatement::__Resolved;
}

//------------------------------------------------------------------------------
/**
*/
ScopeStatement::~ScopeStatement()
{
    for (Symbol* sym : this->symbols)
        delete sym;
}

} // namespace GPULang
