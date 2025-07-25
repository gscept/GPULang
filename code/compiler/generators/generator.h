#pragma once
//------------------------------------------------------------------------------
/**
    Compiler backend for code generation. This class is responsible for outputting
    code to generate the machine binary code.

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <vector>
#include <string>
#include <functional>
#include "ast/expressions/boolexpression.h"
#include "ast/program.h"

namespace GPULang
{

struct Timer;
struct Symbol;
struct Compiler;
struct Program;
struct Function;
struct Generator
{
    /// generate code
    virtual bool Generate(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc);

    /// produce error in compiler with explicit file, line and column
    void Error(const std::string& msg);
    /// produce error in compiler with explicit file, line and column
    void Warning(const std::string& msg);

    PinnedArray<FixedString> messages = 0xFFF;
    bool hasErrors = false;

    BoolExpression shaderValueExpressions[ProgramInstance::__Resolved::EntryType::NumProgramEntries];
    std::vector<std::string> targetLanguageFiles;
    Function* mainFunction;
};

} // namespace GPULang
