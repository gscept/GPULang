#pragma once
//------------------------------------------------------------------------------
/**
    Compiler backend for SPIRV code generation. This is not to be confused
    with compiling for Vulkan, this generator is meant for generating 
    pure SPIRV output from the AST.

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "generator.h"
#include <map>
#include <set>
#include <stack>
namespace GPULang
{

class SPIRVGenerator : public Generator
{
public:

    /// Bind intrinsics codegen to builtin functions
    void SetupIntrinsics() override;

    /// generate SPIRV output
    bool Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    /// Add or search for a symbol
    uint32_t AddSymbol(std::string name, std::string declare, bool global = false);
    /// Get symbol
    uint32_t GetSymbol(std::string name);
    /// Returns true if symbol exists
    bool HasSymbol(std::string name);
    /// Add an op without a mapping
    void AddOp(std::string name, bool global = false, std::string comment = "");
    /// Add mapped op
    uint32_t AddMappedOp(std::string name, std::string comment = "");
    /// Add capability
    void AddCapability(std::string name, std::string declare);

    /// Find symbol and assert if fails
    uint32_t FindSymbolMapping(std::string value);
    /// Replace symbol mapping, assumes symbol already exists
    void ReplaceSymbolMapping(uint32_t oldMapping, uint32_t newMapping);

    std::set<std::string> requiredCapabilities;
    uint32_t symbolCounter;

    struct Scope
    {
        std::map<std::string, uint32_t> symbols;
    };
    std::vector<Scope> scopeStack;
    std::set<std::string> capabilities;
    /// Push a new scope on the stack
    void PushScope();
    /// Pop scope
    void PopScope();

    std::string decorations;
    std::string declarations;
    std::string header;
    std::string functional;

    using IntrinsicMappingFunction = std::function<uint32_t(Compiler*, SPIRVGenerator*, uint32_t, const std::vector<uint32_t>&, const std::vector<uint32_t>&)>;
    std::map<Function*, IntrinsicMappingFunction> intrinsicMap;
};

} // namespace GPULang
