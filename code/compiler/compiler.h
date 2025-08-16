#pragma once
//------------------------------------------------------------------------------
/**
    Compiler frontend

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <utility>
#include <chrono>
#include "ast/symbol.h"
#include "ast/effect.h"
#include "ast/types/type.h"
#include "ast/program.h"
#include "ast/variable.h"
#include "ast/expressions/boolexpression.h"
#include "validators/validator.h"
#include "generators/generator.h"
#include "binwriter.h"
#include "textwriter.h"
#include "serialize.h"
#include "ast/renderstate.h"

struct GPULangDiagnostic;
namespace GPULang
{

struct Thread;
struct Type;
struct Scope;
struct Generate;
struct Compiler
{
    enum class Backend : uint8_t
    {
        GLSL = 0x1,
        HLSL = 0x2,
        DXIL = 0x4,            // target is DXIL
        SPIRV = 0x8,           // target is generic SPIRV
        VULKAN_SPIRV = 0x10,   // target is SPIRV with the Vulkan subset
        WEBGPU = 0x20,         // target is wgsl
        METAL = 0x40,          // target is msl
        METAL_IR = 0x80        // target is Metal IR (not implemented)
    };
    
    static constexpr StaticMap BackendMap = std::array {
        std::pair{ "vulkan"_h, Backend::VULKAN_SPIRV },
        std::pair{ "directx"_h, Backend::HLSL },
        std::pair{ "metal"_h, Backend::METAL },
        std::pair{ "webgpu"_h, Backend::WEBGPU }
    };
    
    static constexpr StaticMap BackendLookup = std::array {
        std::pair{ Backend::VULKAN_SPIRV, "vulkan"_c },
        std::pair{ Backend::HLSL, "directx"_c },
        std::pair{ Backend::METAL, "metal"_c },
        std::pair{ Backend::WEBGPU, "webgpu"_c }
    };  
    
    enum class Binding : uint8_t
    {
        Cpp         = 0x1,          // C++ header (.h)
        Rust        = 0x2,          // .rs
        Zig         = 0x4,          // .zig
        JavaScript  = 0x8,          // .js
    };
    
    static constexpr StaticMap BindingMap = std::array {
        std::pair{ "cpp"_h, Binding::Cpp },
        std::pair{ "rust"_h, Binding::Rust },
        std::pair{ "zig"_h, Binding::Zig },
        std::pair{ "javascript"_h, Binding::JavaScript }
    };

    enum class ErrorFormat : uint8_t
    {
        MSVC,       // default, output errors according to MSVC
        GCC,
        Clang
    };
    


    struct Options
    {
        uint8_t quiet : 1 = 0;
        uint8_t warningsAsErrors : 1 = 0;
        uint8_t emitTimings : 1 = 0;
        uint8_t disallowImplicitConversion : 1 = 0;
        uint8_t disallowImplicitPromotion : 1 = 0;
        uint8_t disallowUninitializedConst : 1 = 0;
        uint8_t warnOnMissingRenderState : 1 = 0;
        uint8_t warnOnMissingColorExport : 1 = 0;
        uint8_t warnOnImplicitBoolPromotion : 1 = 0;
        uint8_t warnOnImplicitBufferPadding : 1 = 0;
        uint8_t warnOnUnusedParameter : 1 = 0;

        uint8_t validate : 1 = 0;
        uint8_t optimize : 1 = 0;
        uint8_t debugSymbols : 1 = 0;
        
        uint8_t languageServerGeneration : 1 = 0; // if true, compiler will run the backend generation even if it's running the validation only

        uint8_t defaultGroupBinding = 0;

        ErrorFormat errorFormat = ErrorFormat::MSVC;
    };
    
    struct TargetCapabilities
    {
        union
        {
            struct
            {
                uint32_t supportsPhysicalBufferAddresses : 1;
                uint32_t supportsPhysicalAddressing : 1;
                uint32_t supportsInlineSamplers : 1;
                uint32_t supportsGlobalDeviceStorage : 1;
                uint32_t supportsGeometryShaders : 1;
                uint32_t supportsTessellationShaders : 1;
                uint32_t supportsMeshShaders : 1;
                uint32_t supportsRayTracing : 1;
            } flags;
            uint32_t bits;
        };
    };
    
    static constexpr StaticMap TargetCapabilityMap = std::array{
        std::pair{ Backend::HLSL, TargetCapabilities{.flags =
            {
                .supportsPhysicalBufferAddresses = true
                , .supportsPhysicalAddressing = false
                , .supportsInlineSamplers = false
                , .supportsGlobalDeviceStorage = false
                , .supportsGeometryShaders = true
                , .supportsTessellationShaders = true
                , .supportsMeshShaders = true
                , .supportsRayTracing = true
            }
        }},
        std::pair{ Backend::GLSL, TargetCapabilities{.flags =
            {
                .supportsPhysicalBufferAddresses = true
                , .supportsPhysicalAddressing = false
                , .supportsInlineSamplers = false
                , .supportsGlobalDeviceStorage = false
                , .supportsGeometryShaders = true
                , .supportsTessellationShaders = true
                , .supportsMeshShaders = true
                , .supportsRayTracing = true
            }
        }},
        std::pair{ Backend::SPIRV, TargetCapabilities{.flags =
            {
                .supportsPhysicalBufferAddresses = true
                , .supportsPhysicalAddressing = true
                , .supportsInlineSamplers = true
                , .supportsGlobalDeviceStorage = true
                , .supportsGeometryShaders = false
                , .supportsTessellationShaders = false
                , .supportsMeshShaders = false
                , .supportsRayTracing = false
            }
        }},
        std::pair{ Backend::VULKAN_SPIRV, TargetCapabilities{.flags =
            {
                .supportsPhysicalBufferAddresses = true
                , .supportsPhysicalAddressing = false
                , .supportsInlineSamplers = true
                , .supportsGlobalDeviceStorage = false
                , .supportsGeometryShaders = true
                , .supportsTessellationShaders = true
                , .supportsMeshShaders = true
                , .supportsRayTracing = true
            }
        }},
        std::pair{ Backend::WEBGPU, TargetCapabilities{.flags =
            {
                .supportsPhysicalBufferAddresses = false
                , .supportsPhysicalAddressing = false
                , .supportsInlineSamplers = false
                , .supportsGlobalDeviceStorage = false
                , .supportsGeometryShaders = false
                , .supportsTessellationShaders = false
                , .supportsMeshShaders = false
                , .supportsRayTracing = false
            }
        }},
        std::pair{ Backend::METAL, TargetCapabilities{.flags =
            {
                .supportsPhysicalBufferAddresses = false
                , .supportsPhysicalAddressing = false
                , .supportsInlineSamplers = true
                , .supportsGlobalDeviceStorage = false
                , .supportsGeometryShaders = false
                , .supportsTessellationShaders = false
                , .supportsMeshShaders = true
                , .supportsRayTracing = true
            }
        }}
    };
    
    struct Target
    {
        TargetCapabilities capabilities;
        uint32_t backends = 0x0;
    } target;

    /// constructor
    Compiler();
    /// destructor
    ~Compiler();

    /// setup compiler with target language in generation mode
    void Setup(const TransientArray<Compiler::Backend>& langs, Options options);
    /// setup compiler for validation (language server) mode
    void SetupServer(const TransientArray<Compiler::Backend>& langs, Options options);

    /// Start static symbol setup
    void BeginStaticSymbolSetup();
    /// End static symbol setup
    void EndStaticSymbolSetup();

    /// adds symbol to compiler context, allow duplicate if symbol type should support overloading
    bool AddSymbol(const FixedString& name, Symbol* symbol, bool allowDuplicate = false, bool bypass = false);
    /// get symbol by name
    Symbol* GetSymbol(const FixedString& name) const;
    /// adds symbol to compiler context, allow duplicate if symbol type should support overloading
    bool AddSymbol(const TransientString& name, Symbol* symbol, bool allowDuplicate = false, bool bypass = false);
    /// get symbol by name
    Symbol* GetSymbol(const TransientString& name) const;
    /// Get type by FullType
    Type* GetType(const Type::FullType& type) const;
    /// get symbol by name as other type
    template <typename T> T* GetSymbol(const FixedString& name) const;
    /// return iterator to first and last symbol matching name
    TransientArray<Symbol*> GetSymbols(const FixedString& name) const;
    /// get symbol by name as other type
    template <typename T> T* GetSymbol(const TransientString& name) const;
    /// return iterator to first and last symbol matching name
    TransientArray<Symbol*> GetSymbols(const TransientString& name) const;

    /// Push a new scope
    void PushScope(Scope* scope);
    /// pop the scope
    void PopScope();
    /// Is current scope global
    bool IsScopeGlobal();
    /// Is current scope a type
    bool IsScopeType();
    /// get scope owner
    Symbol* GetScopeOwner();
    /// Return true if any parent scope is of a certain type
    Symbol* GetParentScopeOwner(Symbol::SymbolType type);
    /// Mark scope unreachable for subsequent calls
    void MarkScopeUnreachable();
    /// Returns true if scope is unreachable (after return, break or continue statements)
    bool IsUnreachable();
    /// Resets the unreachable state of a scope
    void MarkScopeReachable();

    struct LocalScope
    {

        static LocalScope MakeLocalScope(Compiler* compiler, Scope* scope)
        {
            compiler->PushScope(scope);
            return LocalScope(compiler);
        }

        LocalScope(LocalScope&& rhs)
        {
            this->compiler = rhs.compiler;
            rhs.compiler = nullptr;
        }
        ~LocalScope()
        {
            if (this->compiler != nullptr)
                this->compiler->PopScope();
        }

    private:
        LocalScope(Compiler* compiler)
        {
            this->compiler = compiler;
        }

        Compiler* compiler;
    };

    /// runs the validation and generation steps, returns true if successful, otherwise false and a list of error messages
    bool Compile(Effect* root, BinWriter& binaryWriter, TextWriter& headerWriter);
    /// runs validation without generation
    bool Validate(Effect* root);

    /// produce error in compiler with explicit file, line and column
    void Error(const TransientString& msg, const FixedString& file, int line, int column, int length = 1);
    /// produce error in compiler from symbol
    void Error(const TransientString& msg, const Symbol* sym);
    /// produce error in compiler with explicit file, line and column
    void Warning(const TransientString& msg, const FixedString& file, int line, int column);
    /// produce error in compiler from symbol
    void Warning(const TransientString& msg, const Symbol* sym);
    /// produce an internal generator error
    void GeneratorError(const TransientString& msg);

    /// helper for unrecognized type error
    void UnrecognizedTypeError(const TransientString& type, Symbol* sym);
    /// helper for unrecognized symbol error
    void UnrecognizedSymbolError(const TransientString& symbol, Symbol* sym);
    /// Helper for reserved words
    void ReservedPrefixError(const FixedString& name, const std::string& word, Symbol* sym);

    /// output binary data
    void OutputBinary(const std::vector<Symbol*>& symbols, BinWriter& writer, Serialize::DynamicLengthBlob& dynamicDataBlob);

    FixedString path;
    FixedString filename;
    std::vector<std::string> defines;
    PinnedArray<GPULangDiagnostic> diagnostics = 0xFFFFFF;
    PinnedArray<FixedString> messages = 0xFFF;
    bool hasErrors;

    PinnedArray<Symbol*> symbols = 0xFFFFFF;
    size_t symbolIterator;
    std::vector<Symbol*> dynamicSymbols;        // symbols generated by the compilation process

    GPULang::Thread* staticSetupThread;

    FixedArray<Compiler::Backend> backends;
    Validator* validator = nullptr;
    Generator* headerGenerator = nullptr;

    struct State
    {
        uint8_t allowConstOverride = false;

        ProgramInstance::__Resolved::EntryType shaderType;
        
        union SideEffects
        {
            enum class Masks
            {
                EXPORT_VERTEX_POSITION_BIT = 1 << 0,
                SET_OUTPUT_LAYER_BIT = 1 << 1,
                SET_VIEWPORT_BIT = 1 << 2,
                EXPORT_VERTEX_BIT = 1 << 3,
                EXPORT_PRIMITIVE_BIT = 1 << 4,
                EXPORT_PIXEL_BIT = 1 << 5,
                EXPORT_DEPTH_BIT = 1 << 6,
                KILL_PIXEL_BIT = 1 << 7,
                STOP_RAY_BIT = 1 << 8,
                IGNORE_RAY_BIT = 1 << 9,
            };
            struct
            {
                uint32_t exportsVertexPosition : 1;
                uint32_t setsOutputLayer : 1;
                uint32_t setsViewport : 1;
                uint32_t exportsVertex : 1;
                uint32_t exportsPrimitive : 1;
                uint32_t exportsPixel : 1;
                uint32_t exportsExplicitDepth : 1;
                uint32_t killsPixel : 1;
                uint32_t stopsRay : 1;
                uint32_t ignoresRay : 1;
            } flags;
            uint32_t bits;
        } sideEffects;
        Function* function;
    } currentState;
    uint32_t linkDefineCounter = 0;

    bool branchReturns;

    Scope* intrinsicScope, *mainScope;
    PinnedArray<Scope*> scopes = 0xFFF;
    bool staticSymbolSetup = false;
    
    struct GenerationState
    {
        Generate* owner;
        bool active;
        bool branchActive;
    } generationState;

    std::string debugPath;
    bool debugOutput;

    bool ignoreReservedWords;

    RenderStateInstance defaultRenderState;
    Variable shaderSwitches[ProgramInstance::__Resolved::EntryType::NumProgramEntries];
    BoolExpression shaderValueExpressions[ProgramInstance::__Resolved::EntryType::NumProgramEntries];

    Options options;

    struct Timer
    {
        std::chrono::high_resolution_clock::time_point start, delta;
        std::chrono::duration<double, std::milli> duration;
        
        std::chrono::high_resolution_clock::time_point creation;
        
        Timer()
        {
            this->creation = std::chrono::high_resolution_clock::now();
        }

        void Start()
        {
            this->start = std::chrono::high_resolution_clock::now();
        }

        void Stop()
        {
            this->duration = std::chrono::high_resolution_clock::now() - this->start;
        }

        void Print(const std::string& message)
        {
            fprintf(stdout, "%s took %.2f ms\n", message.c_str(), this->duration.count());
        }
        
        void TotalTime()
        {
            std::chrono::duration<double, std::milli> duration = std::chrono::high_resolution_clock::now() - this->creation;
            fprintf(stdout, "Total time %.2f ms\n\n", duration.count());
            fflush(stdout); // Only flush final output
        }
    } performanceTimer;
};
	
//------------------------------------------------------------------------------
/**
*/
template <typename K, size_t SIZE> inline bool
set_contains(const StaticSet<K, SIZE>& set, const K& key)
{
    return set.Find(key) != set.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename K, size_t SIZE> inline bool
set_contains(const StaticSet<T, SIZE>& set, const K& key)
{
    return set.Find(key) != set.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename K, size_t SIZE1, size_t SIZE2> inline bool
set_contains(const StaticSet<K, SIZE1>& set, const StaticSet<K, SIZE2>& otherSet)
{
    for (const K& key : otherSet)
        if (set.Find(key) != set.end())
            return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
template <typename K> inline bool
set_contains(const PinnedSet<K>& set, const K& key)
{
    return set.Find(key) != set.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename K> inline bool
set_contains(const PinnedSet<T>& set, const K& key)
{
    using T_const = const T;
    return set.Find(T_const(key)) != set.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename K> inline bool
set_contains(const PinnedSet<K>& set, const PinnedSet<K>& otherSet)
{
    for (const K& key : otherSet)
        if (set.Find(key) != set.end())
            return true;
    return false;
}

//------------------------------------------------------------------------------
/**
*/
template <typename K, typename T> inline bool
map_contains(const std::map<K, T>& map, const K& key)
{
    return map.find(key) != map.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename K, typename T> inline bool
map_contains(const std::unordered_map<K, T>& map, const K& key)
{
    return map.find(key) != map.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
inline T*
Compiler::GetSymbol(const FixedString& name) const
{
    return static_cast<T*>(this->GetSymbol(name));
}

//------------------------------------------------------------------------------
/**
*/
template <typename T>
inline T*
Compiler::GetSymbol(const TransientString& name) const
{
    return static_cast<T*>(this->GetSymbol(name));
}

//------------------------------------------------------------------------------
/**
*/
inline Type*
Compiler::GetType(const Type::FullType& type) const
{
    Symbol* sym = nullptr;
    FixedString str;
    if (type.swizzleName.len > 0)
        str = type.swizzleName;
    else
        str = type.name;
    
    auto scopeIter = this->scopes.rbegin();
    do
    {
        auto scope = scopeIter.get();
        PinnedMap<HashString, Symbol*>* map;
        if (scope->type == Scope::ScopeType::Type)
        {
            Type* type = static_cast<Type*>(scope->owningSymbol);
            map = &type->scope.symbolLookup;
        }
        else
        {
            map = &scope->symbolLookup;
        }
        auto range = map->FindRange(HashString(str));
        for (auto it = range.first; it != range.second; it++)
        {
            sym = it->second;
            if (sym->symbolType == Symbol::SymbolType::TypeType || sym->symbolType == Symbol::SymbolType::StructureType || sym->symbolType == Symbol::SymbolType::EnumerationType)
                return static_cast<Type*>(sym);
        }
        scopeIter++;
    }
    while (scopeIter != this->scopes.rend());
    return nullptr;
}

} // namespace GPULang
