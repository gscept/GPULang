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
        SubroutineType,
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
            : file("")
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

    template <typename T>
    using Resolve = typename std::remove_pointer<T>::type::__Resolved*;

    template <typename T> 
    static Resolve<T> Resolved(T* t)
    {
        return static_cast<Resolve<T>>(t->resolved);
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
    PinnedMap<FixedString, Symbol*> symbolLookup = 0x82000;
    Symbol* owningSymbol = nullptr;
    bool unreachable = false;
};

} // namespace GPULang
//------------------------------------------------------------------------------
