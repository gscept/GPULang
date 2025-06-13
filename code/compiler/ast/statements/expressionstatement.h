#pragma once
//------------------------------------------------------------------------------
/**
    Variable declaration

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
#include <vector>
namespace GPULang
{

struct ExpressionStatement : public Statement
{
    /// constructor
    ExpressionStatement(FixedArray<Expression*> expressions);
    /// destructor
    virtual ~ExpressionStatement();

    FixedArray<Expression*> expressions;
};

} // namespace GPULang
