#pragma once
//------------------------------------------------------------------------------
/**
    Generates a .h file

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "generator.h"
namespace GPULang
{

struct HGenerator : public Generator
{
    /// generate code
    bool Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    /// generate structure 
    void GenerateStructureSPIRV(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode);
    /// generate variable
    void GenerateVariableSPIRV(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode, bool isShaderArgument);
};

} // namespace GPULang
