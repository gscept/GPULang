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
#include "ast/variable.h"
#include "ast/program.h"
#include <unordered_map>
#include <set>
#include <assert.h>
#include <dinput.h>



namespace GPULang
{
struct SPVWriter;
struct Statement;
struct SPIRVResult
{
    uint32_t name = 0xFFFFFFFF;
    uint32_t typeName = 0xFFFFFFFF;
    bool isValue = false;           // If not, then the object needs a load to be read. If it is, doesn't support store
    bool isConst = false;           // If true, value is a constant
    bool isLiteral = false;         // If true, then the value is a literal value and can be constant constructed
    bool isSpecialization = false;  // If true, then the value is the product of specialization
    bool isStructPadded = false;    // If true, the type is padded in a struct and requires an extra access indirection
    uint32_t derefs = 0;
    std::vector<uint32_t> parentTypes;

    Type::SwizzleMask swizzleMask;
    uint32_t swizzleType = 0xFFFFFFFF;

    std::vector<SPIRVResult> accessChain;
    void AddAccessChainLink(std::vector<SPIRVResult> links)
    {
        accessChain.insert(accessChain.end(), links.begin(), links.end());
    }

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

    enum class Storage
    {
        Invalid,
        Private,
        Device,
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
        Output,
        RayPayload,                             // variable is a ray payload
        RayPayloadInput,                        // variable is a ray payload input
        RayHitAttribute,                        // variable ray tracing hit attribute (barycentrics)
        CallableData,                           // variable is ray tracing callable data
        CallableDataInput,                      // variable is ray tracing callable data
    } scope = Storage::Function;

    static ConstantString ScopeToString(Storage s)
    {
        switch (s)
        {
            case Storage::Device:
                return "CrossWorkgroup";
            case Storage::Private:
                return "Private";
            case Storage::WorkGroup:
                return "Workgroup";
            case Storage::Uniform:
                return "Uniform";
            case Storage::Sampler:
            case Storage::Image:
            case Storage::MutableImage:
            case Storage::UniformConstant:
                return "UniformConstant";
            case Storage::StorageBuffer:
                return "StorageBuffer";
            case Storage::PushConstant:
                return "PushConstant";
            case Storage::Function:
                return "Function";
            case Storage::Input:
                return "Input";
            case Storage::Output:
                return "Output";
            case Storage::RayPayload:
                return "RayPayloadKHR";
            case Storage::RayPayloadInput:
                return "IncomingRayPayloadKHR";
            case Storage::RayHitAttribute:
                return "HitAttributeKHR";
            case Storage::CallableData:
                return "CallableDataKHR";
            case Storage::CallableDataInput:
                return "IncomingCallableDataKHR";
            default:
                assert(false);
                return "";
        }
    }

    SPIRVResult()
    {
        *this = SPIRVResult::Invalid();
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

    explicit SPIRVResult(float literal)
    {
        this->literalValue.type = LiteralValue::FloatType;
        this->literalValue.f = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    explicit SPIRVResult(int literal)
    {
        this->literalValue.type = LiteralValue::IntType;
        this->literalValue.i = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    explicit SPIRVResult(bool literal)
    {
        this->literalValue.type = LiteralValue::BoolType;
        this->literalValue.b = literal;
        this->isLiteral = true;
        this->isValue = false;
    }

    explicit SPIRVResult(uint32_t literal)
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
    SPIRVResult type;
};

class SPIRVGenerator : public Generator
{
public:

    /// Constructor
    SPIRVGenerator();

    /// Setup intrinsics
    static void SetupIntrinsics();

    /// generate SPIRV output
    bool Generate(const Compiler* compiler, const Program* program, const std::vector<Symbol*>& symbols, std::function<void(const std::string&, const std::string&)> writerFunc) override;



    /// Add or search for a symbol
    uint32_t AddSymbol(const TransientString& name, const TransientString& declare, bool global = false);
    /// Add or search for a symbol
    uint32_t AddSymbol(const TransientString& name, const TransientString& declare, SPIRVResult type, bool global = false);
    /// Add a symbol for a reserved name
    void AddReservedSymbol(const TransientString& name, uint32_t object, const TransientString& declare, bool global = false);
    /// Get symbol
    const SymbolAssignment GetSymbol(const std::string& name);
    /// Add an op without a mapping
    void AddOp(const TransientString& name, bool global = false, TransientString comment = TransientString(nullptr));
    /// Add mapped op
    uint32_t AddMappedOp(const TransientString& name, TransientString comment = TransientString(nullptr));
    /// Add capability
    void AddCapability(const TransientString& declare);
    /// import extension
    uint32_t ImportExtension(const TransientString& name);
    /// Add extension
    void AddExtension(const TransientString& name);
    /// Add decoration
    void AddDecoration(const TransientString& name, uint32_t object, const TransientString& decorate);
    /// Add member decoration
    void AddMemberDecoration(uint32_t struc, uint32_t index, const TransientString& decorate);
    /// Add extra mapping for preexisting object
    void AddMapping(const TransientString& name, uint32_t object);
    /// Reserve a name
    uint32_t ReserveName();
    /// Add op with reserved name
    void AddReserved(const TransientString& op, uint32_t name, TransientString comment = TransientString(nullptr));
    /// Add function variable declaration
    uint32_t AddVariableDeclaration(Symbol* sym, const TransientString& name, uint32_t typeName, uint32_t init, uint32_t copy, SPIRVResult::Storage scope, SPIRVResult type, bool global = false);

    uint32_t symbolCounter;

    struct Scope
    {
        std::unordered_map<std::string, SymbolAssignment> symbols;
    };
    std::vector<Scope> scopeStack;

    /// Push a type to the stack
    void PushAccessChain(Type* chain, SPIRVResult::Storage scope = SPIRVResult::Storage::Function);
    /// Pop a type from the stack
    void PopAccessChain();
    
    std::vector<std::tuple<Type*, SPIRVResult::Storage>> accessChain;
    
    std::vector<std::string> capabilities;
    /// Push a new scope on the stack
    void PushScope();
    /// Pop scope
    void PopScope();

    GrowingString header;
    GrowingString capability;
    GrowingString extension;
    GrowingString extImport;
    GrowingString decorations;
    GrowingString declarations;
    GrowingString functions;
    
    GrowingString functional;
    std::set<uint32_t> interfaceVariables;

    std::unordered_map<std::string, uint32_t> extensions;
    std::unordered_map<std::string, std::set<std::string>> decorationMap;

    GrowingString variableDeclarations;
    GrowingString parameterInitializations;
    bool blockOpen = false;
    bool literalExtract = false;
    bool linkDefineEvaluation = false;
    uint32_t linkDefineSlot = UINT32_MAX;

    bool skipBreakContinue = false;

    Function* entryPoint = nullptr;
    Program::__Resolved* evaluatingProgram = nullptr;

    SPVWriter* writer;

    struct MergeBlock
    {
        uint32_t continueLabel, breakLabel;
    } mergeBlocks[256];
    uint32_t mergeBlockCounter;

    using FunctionToSPIRVMapping = std::function<SPIRVResult(const Compiler*, SPIRVGenerator*, uint32_t, const std::vector<SPIRVResult>&)>;
    static std::unordered_map<Function*, FunctionToSPIRVMapping> IntrinsicMap;
    std::unordered_map<Function*, FunctionToSPIRVMapping> generatorIntrinsics;

    uint32_t shaderStage;
};

} // namespace GPULang
