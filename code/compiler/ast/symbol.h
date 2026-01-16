#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::Symbol
    
    Basic object representation, contains basic information for an AnyFX symbol.

    Each symbol has a set of parser data and compiler data. The other half, compiler data, is data resolved
    from the validation and generation step, and includes the unfolded result of expressions,
    flags and such.
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include <map>
#include "memory.h"
#include "string_utils.h"
#include "containers.h"

#define _IMPLEMENT_ATTRIBUTES() FixedArray<Attribute*> attributes; void CleanupAttributes() { for (auto attr : this->attributes) { attr->~Attribute(); }};
#define _IMPLEMENT_ANNOTATIONS() FixedArray<Annotation*> annotations; void CleanupAnnotations() { for (auto annot : this->annotations) { annot->~Annotation(); }};

namespace GPULang
{


struct Compiler;
struct Symbol
{
    enum SymbolType
    {
        InvalidType,
        AliasType,
        PreprocessorType,
        TypeType,
        ProgramInstanceType,
        RenderStateInstanceType,
        SamplerStateInstanceType,
        VariableType,
        FunctionType,
        StructureType,
        EnumerationType,
        FloatExpressionType,
        FloatVecExpressionType,
        IntExpressionType,
        IntVecExpressionType,
        UIntExpressionType,
        UIntVecExpressionType,
        BoolExpressionType,
        BoolVecExpressionType,
        EnumExpressionType,
        StringExpressionType,
        BinaryExpressionType,
        SymbolExpressionType,
        AccessExpressionType,
        InitializerExpressionType,
        ArrayInitializerExpressionType,
        ArrayIndexExpressionType,
        CallExpressionType,
        CommaExpressionType,
        TernaryExpressionType,
        UnaryExpressionType,
        BreakStatementType,
        ContinueStatementType,
        ExpressionStatementType,
        ForStatementType,
        IfStatementType,
        TerminateStatementType,
        ScopeStatementType,
        SwitchStatementType,
        WhileStatementType,
        GenerateType,
        DeclaredExpressionType,

        NumSymbolTypes
    };

    /// constructor
    Symbol();
    /// destructor
    virtual ~Symbol();

    struct Location
    {
        FixedString file;
        int line;
        uint16_t start, end;

        Location()
            : file()
            , line(-1)
            , start(0)
            , end(0)
        {
        }
        
        bool Valid() const
        {
            return this->line != -1;
        }
    };


    //------------------------------------------------------------------------------
    /**
        Parser state
    */
    //------------------------------------------------------------------------------
    FixedString name;
    FixedString documentation;
    SymbolType symbolType;

    Location location;

    /// construct string representing the line:row and file for this symbol
    std::string GetLocation() const;

    const TransientString TypeToString() const;

    //------------------------------------------------------------------------------
    /**
        Compiler state
    */
    //------------------------------------------------------------------------------
    struct __Resolved
    {
        virtual ~__Resolved() {};
        std::string signature;
        bool unreachable = false;
    };
    __Resolved* resolved;
    
    // Trait to check if T has member `foo`
    template <typename, typename = std::void_t<>>
    struct has_member_thisResolved : std::false_type {};

    template <typename T>
    struct has_member_thisResolved<T, std::void_t<decltype(std::declval<T>().thisResolved)>>
        : std::true_type {};

    template <typename T>
    using Resolve = typename std::remove_pointer<T>::type::__Resolved*;

    template <typename T> 
    static Resolve<T> Resolved(T* t)
    {
        if constexpr (has_member_thisResolved<std::decay_t<T>>::value)
        {
            return t->thisResolved;
        }
        else
        {
            return static_cast<Resolve<T>>(t->resolved);
        }
    }
}; 


struct Scope
{
    ~Scope()
    {
        this->symbols.Invalidate();
        this->symbolLookup.Invalidate();
    }
    enum class ScopeType
    {
        Global,
        Local,
        Type
    };
    ScopeType type = ScopeType::Local;
    PinnedArray<Symbol*> symbols = 0x82000;
    PinnedMap<HashString, Symbol*> symbolLookup = 0x82000;
    Symbol* owningSymbol = nullptr;
    bool unreachable = false;
};

// A domain denotes the context in which a symbol exists, be it unique per workgroup or per invocation
// A domain can be promoted implicitly but demotion requires an explicit operation
enum class Domain
{
    Invalid,
    Device,
    Workgroup,
    Invocation,
};

// Implicit domain promotion can only go upwards in the hierarchy
inline Domain
PromoteDomain(Domain leftDomain, Domain rightDomain)
{
    if ((uint32_t)leftDomain < (uint32_t)rightDomain)
        return rightDomain;
    return leftDomain;
}


} // namespace GPULang
//------------------------------------------------------------------------------
