#pragma once
//------------------------------------------------------------------------------
/**
    AST for a preprocessor call

    (C) 2025 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "symbol.h"
namespace GPULang
{

struct Preprocessor : public Symbol
{
    /// constructor
    Preprocessor();
    /// destructor
    virtual ~Preprocessor();

    enum Type
    {
        Include,
        Macro,
        Line,
        IfDef,
        EndIf
    } type;

    std::string method;
    std::vector<std::string> args;
};

}