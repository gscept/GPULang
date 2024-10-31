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
#include "ast/types/type.h"
#include "ast/program.h"
#include <unordered_map>
#include <set>
#include <stack>
#include <assert.h>
#include <dinput.h>

namespace GPULang
{

struct Statement;
struct SPIRVResult
{
    uint32_t name = 0xFFFFFFFF;
    uint32_t typeName = 0xFFFFFFFF;
    bool isValue = false;           // If not, then the object needs a load to be read. If it is, doesn't support store
    bool isConst = false;           // If true, value is a constant
    bool isLiteral = false;         // If true, then the value is a literal value and can be constant constructed
    bool isSpecialization = false;  // If true, then the value is the product of specialization
    std::vector<uint32_t> parentTypes;

    Type::SwizzleMask swizzleMask;
    uint32_t swizzleType = 0xFFFFFFFF;

    struct LiteralValue
    {
        union
        {
            float f;
            int i;
            uint32_t ui;
            bool b;
        };

        enum Type
        {
            FloatType,
            IntType,
            UIntType,
            BoolType
        } type;
    } literalValue;

    enum Storage
    {
        Private,
        WorkGroup,
        Uniform,
        UniformConstant,
        StorageBuffer,
        Sampler,
        PushConstant,
        Function,
        Image,
        MutableImage,
        Input,
        Output
    } scope;

    static std::string ScopeToString(Storage s)
    {
        switch (s)
        {
            case Storage::Private:
                return "Private";
                break;
            case Storage::WorkGroup:
                return "WorkGroup";
                break;
            case Storage::Uniform:
                return "Uniform";
                break;
            case Storage::Sampler:
            case Storage::Image:
            case Storage::MutableImage:
            case Storage::UniformConstant:
                return "UniformConstant";
                break;
            case Storage::StorageBuffer:
                return "StorageBuffer";
                break;
            case Storage::PushConstant:
                return "PushConstant";
                break;
            case Storage::Function:
                return "Function";
                break;
            case Storage::Input:
                return "Input";
                break;
            case Storage::Output:
                return "Output";
                break;
            default:
                assert(false);
                return "";
        }
    }

    SPIRVResult(uint32_t name, uint32_t type, bool isValue = false, bool isConstant = false, Storage scope = Storage::Function, const std::vector<uint32_t>& parentTypes = {})
        : name(name)
        , typeName(type)
        , isValue(isValue)
        , isConst(isConstant)
        , scope(scope)
        , parentTypes(parentTypes)
    {
        this->swizzleMask.bits.x = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.y = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.z = Type::SwizzleMask::Invalid;
        this->swizzleMask.bits.w = Type::SwizzleMask::Invalid;
    };

    SPIRVResult(float literal)
    {
        this->literalValue.type = LiteralValue::FloatType;
        this->literalValue.f = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    SPIRVResult(int literal)
    {
        this->literalValue.type = LiteralValue::IntType;
        this->literalValue.i = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    SPIRVResult(uint32_t literal)
    {
        this->literalValue.type = LiteralValue::UIntType;
        this->literalValue.ui = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    static SPIRVResult Invalid()
    {
        return SPIRVResult(0xFFFFFFFF, 0xFFFFFFFF);
    }

    bool operator!=(const SPIRVResult& rhs) { return this->name != rhs.name || this->typeName != rhs.typeName; }
    bool operator==(const SPIRVResult& rhs) { return this->name == rhs.name && this->typeName == rhs.typeName; }
};

struct SymbolAssignment
{
    Symbol* sym;
    uint32_t value;
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
    uint32_t AddSymbol(const std::string& name, const std::string& declare, bool global = false);
    /// Add a symbol for a reserved name
    void AddReservedSymbol(const std::string& name, uint32_t object, const std::string& declare, bool global = false);
    /// Get symbol
    const SymbolAssignment GetSymbol(const std::string& name);
    /// Returns true if symbol exists
    bool HasSymbol(const std::string& name);
    /// Add an op without a mapping
    void AddOp(const std::string& name, bool global = false, std::string comment = "");
    /// Add mapped op
    uint32_t AddMappedOp(const std::string& name, std::string comment = "");
    /// Add capability
    void AddCapability(const std::string& declare);
    /// Add extension
    uint32_t AddExtension(const std::string& name);
    /// Add decoration
    void AddDecoration(const std::string& name, uint32_t object, const std::string& decorate);
    /// Add member decoration
    void AddMemberDecoration(uint32_t struc, uint32_t index, const std::string& decorate);
    /// Add extra mapping for preexisting object
    void AddMapping(const std::string& name, uint32_t object);
    /// Reserve a name
    uint32_t ReserveName();
    /// Add op with reserved name
    void AddReserved(const std::string& op, uint32_t name, std::string comment = "");
    /// Add function variable declaration
    uint32_t AddVariableDeclaration(Symbol* sym, const std::string& name, uint32_t type, uint32_t init, SPIRVResult::Storage scope, bool global = false);

    uint32_t symbolCounter;

    struct Scope
    {
        std::unordered_map<std::string, SymbolAssignment> symbols;
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
    std::set<uint32_t> interfaceVariables;

    std::unordered_map<std::string, uint32_t> extensions;
    std::unordered_map<std::string, std::set<std::string>> decorationMap;

    std::string variableDeclarations;
    bool blockOpen = false;
    bool literalExtract = false;
    bool linkDefineEvaluation = false;
    uint32_t linkDefineSlot = UINT32_MAX;

    bool skipBreakContinue = false;

    Function* entryPoint = nullptr;
    Program::__Resolved* evaluatingProgram = nullptr;

    struct MergeBlock
    {
        uint32_t continueLabel, breakLabel;
    } mergeBlocks[256];
    uint32_t mergeBlockCounter;

    using IntrinsicMappingFunction = std::function<SPIRVResult(Compiler*, SPIRVGenerator*, uint32_t, const std::vector<SPIRVResult>&)>;
    std::unordered_map<Function*, IntrinsicMappingFunction> intrinsicMap;
};

} // namespace GPULang
