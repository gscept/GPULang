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
#include "ast/symbol.h"
#include <unordered_map>
#include <set>
#include <stack>
namespace GPULang
{

struct Statement;
struct SPIRVResult
{
    uint32_t name = 0xFFFFFFFF;
    uint32_t typeName = 0xFFFFFFFF;
    bool isValue = false;       // If not, then the object needs a load to be read. If it is, doesn't support store
    bool isLiteral = false;     // If true, then the value is a literal value and can be constant constructed
    enum Scope
    {
        Private,
        WorkGroup,
        Uniform,
        UniformConstant,
        StorageBuffer,
        PushConstant,
        Function,
        Image,
        Input,
        Output
    } scope;

    static std::string ScopeToString(Scope s)
    {
        switch (s)
        {
            case Scope::Private:
                return "Private";
                break;
            case Scope::WorkGroup:
                return "WorkGroup";
                break;
            case Scope::Uniform:
                return "Uniform";
                break;
            case Scope::UniformConstant:
                return "UniformConstant";
                break;
            case Scope::StorageBuffer:
                return "StorageBuffer";
                break;
            case Scope::PushConstant:
                return "PushConstant";
                break;
            case Scope::Function:
                return "Function";
                break;
            case Scope::Image:
                return "Image";
                break;
            case Scope::Input:
                return "Input";
                break;
            case Scope::Output:
                return "Output";
                break;
        }
    }

    SPIRVResult(uint32_t name, uint32_t type, bool isValue = false, bool isLiteral = false, Scope scope = Scope::Function)
        : name(name)
        , typeName(type)
        , isValue(isValue)
        , isLiteral(isLiteral)
        , scope(scope)
    {};

    static SPIRVResult Invalid()
    {
        return SPIRVResult(0xFFFFFFFF, 0xFFFFFFFF);
    }

    bool operator!=(const SPIRVResult& rhs) { return this->name != rhs.name || this->typeName != rhs.typeName; }
    bool operator==(const SPIRVResult& rhs) { return this->name == rhs.name && this->typeName == rhs.typeName; }
};



class SPIRVGenerator : public Generator
{
public:

    /// Constructor
    SPIRVGenerator();

    /// Bind intrinsics codegen to builtin functions
    void SetupIntrinsics() override;

    /// generate SPIRV output
    bool Generate(Compiler* compiler, Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;

    /// Add or search for a symbol
    uint32_t AddSymbol(std::string name, std::string declare, bool global = false);
    /// Add a symbol for a reserved name
    void AddReservedSymbol(std::string name, uint32_t object, std::string declare, bool global = false);
    /// Get symbol
    uint32_t GetSymbol(std::string name);
    /// Returns true if symbol exists
    bool HasSymbol(std::string name);
    /// Add an op without a mapping
    void AddOp(std::string name, bool global = false, std::string comment = "");
    /// Add mapped op
    uint32_t AddMappedOp(std::string name, std::string comment = "");
    /// Add capability
    void AddCapability(std::string declare);
    /// Add extension
    uint32_t AddExtension(std::string name);
    /// Add decoration
    void AddDecoration(std::string name, uint32_t object, std::string decorate);
    /// Add member decoration
    void AddMemberDecoration(uint32_t struc, uint32_t index, std::string decorate);
    /// Reserve a name
    uint32_t ReserveName();
    /// Add op with reserved name
    void AddReserved(std::string op, uint32_t name, std::string comment = "");
    /// Add function variable declaration
    uint32_t AddVariableDeclaration(uint32_t type, uint32_t init, std::string comment = "");

    /// Find symbol and assert if fails
    uint32_t FindSymbolMapping(std::string value);
    /// Replace symbol mapping, assumes symbol already exists
    void ReplaceSymbolMapping(uint32_t oldMapping, uint32_t newMapping);

    uint32_t symbolCounter;

    struct Scope
    {
        std::unordered_map<std::string, uint32_t> symbols;
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
    std::string functions;
    
    std::string functional;
    std::vector<uint32_t> interfaceVariables;

    std::unordered_map<std::string, uint32_t> extensions;
    std::unordered_map<std::string, std::set<std::string>> decorationMap;

    std::string variableDeclarations;
    bool blockOpen;

    struct MergeBlock
    {
        uint32_t continueLabel, breakLabel;
    } mergeBlocks[256];
    uint32_t mergeBlockCounter;

    using IntrinsicMappingFunction = std::function<SPIRVResult(Compiler*, SPIRVGenerator*, uint32_t, const std::vector<SPIRVResult>&)>;
    std::unordered_map<Function*, IntrinsicMappingFunction> intrinsicMap;
};

} // namespace GPULang
