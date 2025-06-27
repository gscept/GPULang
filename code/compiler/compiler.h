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

namespace GPULang
{

struct Thread;
struct Diagnostic
{
    enum Severity
    {
        Error,
        Warning,
        Info
    };
    std::string error;
    std::string file;
    Severity severity = Severity::Error;
    int line, column, length;
};

struct Type;
struct Scope;
struct Compiler
{
    enum class Language : uint8_t
    {
        GLSL,
        GLSL_SPIRV,     // uses GLSL as intermediate to generate SPIRV
        HLSL,
        HLSL_SPIRV,     // uses HLSL as intermediate to generate SPIRV
        SPIRV,          // target is pure generic SPIRV
        VULKAN_SPIRV    // target is SPIRV with the Vulkan subset
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
        uint8_t symbols : 1 = 0;

        uint8_t defaultGroupBinding = 0;

        ErrorFormat errorFormat = ErrorFormat::MSVC;
    };

    struct Target
    {
        Generator* generator;
        std::string name;

        uint8_t supportsPhysicalBufferAddresses : 1;
        uint8_t supportsPhysicalAddressing : 1;
        uint8_t supportsInlineSamplers : 1;
        uint8_t supportsGlobalDeviceStorage : 1;
    } target;

    /// constructor
    Compiler();
    /// destructor
    ~Compiler();

    /// setup compiler with target language in generation mode
    void Setup(const Compiler::Language& lang, const std::vector<std::string>& defines, Options options);
    /// setup compiler for validation (language server) mode
    void Setup(Options options);
    /// Create generator
    Generator* CreateGenerator(const Compiler::Language& lang, Options options);

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
    std::vector<Symbol*> GetSymbols(const FixedString& name) const;
    /// get symbol by name as other type
    template <typename T> T* GetSymbol(const TransientString& name) const;
    /// return iterator to first and last symbol matching name
    std::vector<Symbol*> GetSymbols(const TransientString& name) const;

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

    std::string path;
    std::string filename;
    std::vector<std::string> defines;
    PinnedArray<Diagnostic> diagnostics = 0xFFFFFF;
    std::vector<std::string> messages;
    bool hasErrors;

    PinnedArray<Symbol*> symbols = 0xFFFFFF;
    size_t symbolIterator;
    std::vector<Symbol*> dynamicSymbols;        // symbols generated by the compilation process

    GPULang::Thread* staticSetupThread;

    Compiler::Language lang;
    Validator* validator = nullptr;
    Generator* headerGenerator = nullptr;

    struct State
    {
        uint8_t allowConstOverride = false;

        Program::__Resolved::ProgramEntryType shaderType;
        
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
        bool active;
        bool branchActive;
    } generationState;

    std::string debugPath;
    bool debugOutput;

    bool ignoreReservedWords;

    RenderState defaultRenderState;
    Variable shaderSwitches[Program::__Resolved::ProgramEntryType::NumProgramEntries];
    BoolExpression shaderValueExpressions[Program::__Resolved::ProgramEntryType::NumProgramEntries];

    Options options;

    struct Timer
    {
        clock_t start, delta;
        float duration;

        void Start()
        {
            this->start = clock();
        }

        void Stop()
        {
            this->duration = (clock() - this->start) * 1000 / float(CLOCKS_PER_SEC);
        }

        void Print(const std::string& message)
        {
            printf("%s took %.2f ms\n", message.c_str(), this->duration);
        }
    } performanceTimer;
};

//------------------------------------------------------------------------------
/**
*/
template <typename K> inline bool
set_contains(const StaticSet<K>& set, const K& key)
{
    return set.Find(key) != set.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename K> inline bool
set_contains(const StaticSet<T>& set, const K& key)
{
    using T_const = const T;
    return set.Find(key) != set.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename K> inline bool
set_contains(const StaticSet<K>& set, const StaticSet<K>& otherSet)
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
inline constexpr T min(T a, T b)
{
    return a < b ? a : b;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename T2>
inline constexpr T min(T a, T2 b)
{
    return a < b ? a : b;
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
    
    return static_cast<Type*>(this->GetSymbol(str));
}

} // namespace GPULang
