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


struct HeaderWriter
{
    uint32_t indentationLevel = 0;
    std::string output;

    void WriteLine(const std::string& in)
    {
        for (uint32_t i = 0; i < indentationLevel; i++)
            output.append("    ");
        output.append(Format("%s\n", in.c_str()));
    }
    void Write(const char* buf, size_t len)
    {
        for (uint32_t i = 0; i < indentationLevel; i++)
            output.append("    ");
        output.append(buf, len);
    }
    void Write(const char* buf)
    {
        for (uint32_t i = 0; i < indentationLevel; i++)
            output.append("    ");
        output.append(buf);
    }
    void Write(const std::string& in)
    {
        for (uint32_t i = 0; i < indentationLevel; i++)
            output.append("    ");
        output.append(in);
    }
    void Write(const TransientString& in)
    {
        for (uint32_t i = 0; i < indentationLevel; i++)
            output.append("    ");
        output.append(in.c_str());
    }

    void Indent()
    {
        indentationLevel++;
    }
    void Unindent()
    {
        if (indentationLevel > 0)
            indentationLevel--;
    }
};

struct HGenerator : public Generator
{
    /// generate code
    bool Generate(const Compiler* compiler, const Program* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    /// generate function
    void GenerateFunctionH(const Compiler* compiler, const Program* program, Symbol* symbol, Program::__Resolved::ProgramEntryType shaderType, HeaderWriter& writer);
    /// generate structure 
    void GenerateStructureH(const Compiler* compiler, const Program* program, Symbol* symbol, HeaderWriter& writer);
    /// generate variable
    void GenerateVariableH(const Compiler* compiler, const Program* program, Symbol* symbol, HeaderWriter& writer, bool isShaderArgument, bool evaluateLinkDefinedVariables);
    /// generate enum
    void GenerateEnumH(const Compiler* compiler, const Program* program, Symbol* symbol, HeaderWriter& writer);
    /// generate program
    void GenerateProgramH(const Compiler* compiler, const Program* program, const PinnedArray<Symbol*>& symbols, HeaderWriter& writer);
};

} // namespace GPULang
