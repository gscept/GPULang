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
    ScopeStatement(const PinnedArray<Symbol*>& contents, const std::vector<Expression*>& unfinished);
    /// destructor
    virtual ~ScopeStatement();

    struct __Resolved : public Statement::__Resolved
    {
        virtual ~__Resolved() {};
        Scope scope;
    };

    PinnedArray<Symbol*> symbols;
    std::vector<Expression*> unfinished;
    Symbol::Location ends;

};

} // namespace GPULang
