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
        Comment,
        Macro,
        Undefine,
        Call,
        Line,
        If,
        Else,
        EndIf,
        Pragma
    } type;

    FixedArray<FixedString> args;
    FixedArray<Symbol::Location> argLocations;
    FixedString contents;
};

}
