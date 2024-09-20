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

struct SPIRVResult
{
    uint32_t name = 0xFFFFFFFF;
    uint32_t typeName = 0xFFFFFFFF;
    bool isValue = false;       // If not, then the object needs a load to be read. If it is, doesn't support store
    bool isLiteral = false;     // If true, then the value is a literal value and can be constant constructed

    SPIRVResult(uint32_t name, uint32_t type, bool isValue = false, bool isLiteral = false)
        : name(name)
        , typeName(type)
        , isValue(isValue)
        , isLiteral(isLiteral)
    {};

    static SPIRVResult Invalid()
    {
        return SPIRVResult(0xFFFFFFFF, 0xFFFFFFFF);
    }
};

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

    using IntrinsicMappingFunction = std::function<SPIRVResult(Compiler*, SPIRVGenerator*, uint32_t, const std::vector<SPIRVResult>&)>;
    std::map<Function*, IntrinsicMappingFunction> intrinsicMap;
};

} // namespace GPULang
