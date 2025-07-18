#pragma once
//------------------------------------------------------------------------------
/**
    Compiler backend for GLSL code generation

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "generator.h"
namespace GPULang
{

class GLSLGenerator : public Generator
{
public:

    enum LanguageFeatureSet
    {
        OpenGLFeatureSet,
        VulkanFeatureSet
    };

    /// constructor
    GLSLGenerator(LanguageFeatureSet featureSet);
    /// Destructor
    ~GLSLGenerator();

    /// generate GLSL output
    bool Generate(const Compiler* compiler, const ProgramInstance* program, const PinnedArray<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    /// generate function
    void GenerateFunctionSPIRV(const Compiler* compiler, const ProgramInstance* program, Symbol* symbol, std::string& outCode);
    /// generate structure
    void GenerateStructureSPIRV(const Compiler* compiler, const ProgramInstance* program, Symbol* symbol, std::string& outCode);
    /// generate variable
    void GenerateVariableSPIRV(const Compiler* compiler, const ProgramInstance* program, Symbol* symbol, std::string& outCode, bool isShaderArgument);

    LanguageFeatureSet featureSet;
};

} // namespace GPULang
