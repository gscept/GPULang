#pragma once
//------------------------------------------------------------------------------
/**
    Termination statement statement

    (C) 2021 Individual contributors, see AUTHORS file
*/  
//------------------------------------------------------------------------------
#include "statement.h"
#include "ast/expressions/expression.h"
namespace GPULang
{

struct TerminateStatement : public Statement
{
    enum class TerminationType 
    {
        Return,
        Discard,
        RayIgnoreIntersection,
        RayTerminate
    };
    /// constructor
    TerminateStatement(Expression* returnValue, TerminationType type);
    /// destructor
    virtual ~TerminateStatement();

    static const ConstantString& TerminationTypeToString(TerminationType type);

    Expression* returnValue;
    TerminationType type;
};

} // namespace GPULang
