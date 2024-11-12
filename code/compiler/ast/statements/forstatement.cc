//------------------------------------------------------------------------------
//  @file forstatement.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "forstatement.h"
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
}

//------------------------------------------------------------------------------
/**
*/
ForStatement::~ForStatement()
{
}

} // namespace GPULang
