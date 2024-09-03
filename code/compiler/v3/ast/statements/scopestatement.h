#pragma once
//------------------------------------------------------------------------------
/**
    Scope, can hold both statements and variable declarations

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
#include <vector>
namespace GPULang
{

struct ScopeStatement : public Statement
{
    /// constructor
    ScopeStatement(const std::vector<Symbol*>& contents);
    /// destructor
    virtual ~ScopeStatement();

    std::vector<Symbol*> statements;
};

} // namespace GPULang
