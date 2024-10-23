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
#include "validators/validator.h"
#include "generators/generator.h"
#include "binwriter.h"
#include "textwriter.h"
#include "serialize.h"
#include "ast/renderstate.h"
namespace GPULang
{

struct Type;
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

    struct Options
    {
        uint8_t warningsAsErrors : 1;
        uint8_t emitTimings : 1;
        uint8_t disallowImplicitConversion : 1;
        uint8_t disallowUninitializedConst : 1;
        uint8_t warnOnMissingRenderState : 1;

        uint8_t validate : 1;
        uint8_t optimize : 1;
    };

    struct Target
    {
        Generator* generator;
        std::string name;

        uint8_t supportsPhysicalAddressing : 1;
        uint8_t supportsInlineSamplers : 1;
    } target;

    /// constructor
    Compiler();
    /// destructor
    ~Compiler();

    /// setup compiler with target language
    void Setup(const Compiler::Language& lang, const std::vector<std::string>& defines, Options options, unsigned int version);

    /// adds symbol to compiler context, allow duplicate if symbol type should support overloading
    bool AddSymbol(const std::string& name, Symbol* symbol, bool allowDuplicate = false, bool bypass = false);
    /// get symbol by name
    Symbol* GetSymbol(const std::string& name) const;
    /// get symbol by name as other type
    template <typename T> T* GetSymbol(const std::string& name) const;
    /// return iterator to first and last symbol matching name
    std::vector<Symbol*> GetSymbols(const std::string& name) const;

    struct Scope
    {
        enum class ScopeType
        {
            Global,
            Local,
            Type
        };
        ScopeType type = ScopeType::Local;
        std::vector<Symbol*> symbols;
        std::multimap<std::string, Symbol*> symbolLookup;
        Symbol* owningSymbol;
        bool unreachable;
    };

    /// create new scope and push it to the stack
    void PushScope(Scope::ScopeType type, Symbol* owner = nullptr);
    /// create new scope from type
    void PushScope(Type* type);
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

        static LocalScope MakeLocalScope(Compiler* compiler, Symbol* owner = nullptr)
        {
            compiler->PushScope(Scope::ScopeType::Local, owner);
            return LocalScope(compiler);
        }

        static LocalScope MakeTypeScope(Compiler* compiler, Symbol* type)
        {
            compiler->PushScope(Scope::ScopeType::Type, type);
            return LocalScope(compiler);
        }

        static LocalScope MakeGlobalScope(Compiler* compiler)
        {
            compiler->PushScope(Scope::ScopeType::Global, nullptr);
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

    /// produce error in compiler with explicit file, line and column
    void Error(const std::string& msg, const std::string& file, int line, int column);
    /// produce error in compiler from symbol
    void Error(const std::string& msg, const Symbol* sym);
    /// produce error in compiler with explicit file, line and column
    void Warning(const std::string& msg, const std::string& file, int line, int column);
    /// produce error in compiler from symbol
    void Warning(const std::string& msg, const Symbol* sym);
    /// produce an internal generator error
    void GeneratorError(const std::string& msg);

    /// helper for unrecognized type error
    void UnrecognizedTypeError(const std::string& type, Symbol* sym);
    /// helper for unrecognized symbol error
    void UnrecognizedSymbolError(const std::string& symbol, Symbol* sym);
    /// Helper for reserved words
    void ReservedPrefixError(const std::string& name, const std::string& word, Symbol* sym);

    /// output binary data
    void OutputBinary(const std::vector<Symbol*>& symbols, BinWriter& writer, Serialize::DynamicLengthBlob& dynamicDataBlob);

    std::string filename;
    std::vector<std::string> defines;
    std::vector<std::string> messages;
    bool hasErrors;

    std::vector<Symbol*> symbols;
    size_t symbolIterator;
    std::vector<Symbol*> dynamicSymbols;        // symbols generated by the compilation process

    Compiler::Language lang;
    Validator* validator = nullptr;
    Generator* headerGenerator = nullptr;

    Function* currentFunction = nullptr;

    bool branchReturns;

    std::vector<Scope*> scopes;

    // Gargh, ugly hack to push a type being declared before a symbol needing it
    Type* declareTy;
    Type::FullType declareType;

    std::string debugPath;
    bool debugOutput;

    bool ignoreReservedWords;

    RenderState defaultRenderState;

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
            printf("%s took %.2f ms\n", message.c_str(), this->duration / 1000.0f);
        }
    } performanceTimer;
};

//------------------------------------------------------------------------------
/**
*/
template <typename K> inline bool
set_contains(const std::set<K>& set, const K& key)
{
    return set.find(key) != set.end();
}

//------------------------------------------------------------------------------
/**
*/
template <typename K> inline bool
set_contains(const std::set<K>& set, const std::set<K>& otherSet)
{
    for (const K& key : otherSet)
        if (set.find(key) != set.end())
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
Compiler::GetSymbol(const std::string& name) const
{
    return static_cast<T*>(this->GetSymbol(name));
}

} // namespace GPULang
