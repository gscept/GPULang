#pragma once
//------------------------------------------------------------------------------
/**
    Generates a .h file

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "generator.h"
#include "ast/program.h"

namespace GPULang
{

struct HGenerator : public Generator
{
    /// generate code
    bool Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    /// generate function
    void GenerateFunctionH(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode);
    /// generate structure 
    void GenerateStructureH(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode);
    /// generate variable
    void GenerateVariableH(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode, bool isShaderArgument, bool evaluateConstants);
    /// generate enum
    void GenerateEnumH(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode);
    /// generate program
    void GenerateProgramH(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::string& outCode);

private:
};

} // namespace GPULang
