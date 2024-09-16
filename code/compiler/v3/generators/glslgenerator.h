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
    bool Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    /// generate function
    void GenerateFunctionSPIRV(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode);
    /// generate structure 
    void GenerateStructureSPIRV(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode);
    /// generate variable
    void GenerateVariableSPIRV(Compiler* compiler, Program* program, Symbol* symbol, std::string& outCode, bool isShaderArgument);

    LanguageFeatureSet featureSet;
};

} // namespace GPULang
