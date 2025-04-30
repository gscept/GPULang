//------------------------------------------------------------------------------
//  @file forstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "forstatement.h"
#include "../variable.h"
namespace GPULang
{

//------------------------------------------------------------------------------
/**
*/
ForStatement::ForStatement(const std::vector<Variable*> declarations, Expression* condition, Expression* loop, Statement* contents)
    : declarations(declarations)
    , condition(condition)
    , loop(loop)
    , contents(contents)
{
    this->symbolType = ForStatementType;
    this->resolved = Alloc<ForStatement::__Resolved>();
    auto thisResolved = Symbol::Resolved(this);
    thisResolved->scope.type = Scope::ScopeType::Local;
}

//------------------------------------------------------------------------------
/**
*/
ForStatement::~ForStatement()
{
    for (auto var : this->declarations)
        var->~Variable();
    this->condition->~Expression();
    this->loop->~Expression();
    this->contents->~Statement();
}

} // namespace GPULang
