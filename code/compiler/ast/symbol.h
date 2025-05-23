#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::Symbol
    
    Basic object representation, contains basic information for an AnyFX symbol.

    Each symbol has a set of parser data and compiler data. Parser data comes from
    the ANTLR parse of the source code. The other half, compiler data, is data resolved
    from the validation and generation step, and includes the unfolded result of expressions,
    flags and such.
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include <map>
#include "memory.h"

#define _IMPLEMENT_ATTRIBUTES() std::vector<Attribute*> attributes; void CleanupAttributes() { for (auto attr : this->attributes) { attr->~Attribute(); }};
#define _IMPLEMENT_ANNOTATIONS() std::vector<Annotation*> annotations; void CleanupAnnotations() { for (auto annot : this->annotations) { annot->~Annotation(); }};

namespace GPULang
{


struct Compiler;
struct Symbol
{
    enum SymbolType
    {
        InvalidType,
        AliasType,
        TypeType,
        ProgramType,
        RenderStateType,
        SamplerStateType,
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

        NumSymbolTypes
    };

    /// constructor
    Symbol();
    /// destructor
    virtual ~Symbol();

    struct Location
    {
        std::string file;
        int line;
        int column;

        int start, end;

        Location()
            : line(-1)
            , column(-1)
            , start(0)
            , end(0)
        {
        }
    };


    //------------------------------------------------------------------------------
    /**
        Parser state
    */
    //------------------------------------------------------------------------------
    std::string name;
    std::string documentation;
    SymbolType symbolType;

    Location location;

    /// construct string representing the line:row and file for this symbol
    std::string GetLocation() const;

    const std::string TypeToString() const;

    //------------------------------------------------------------------------------
    /**
        Compiler state
    */
    //------------------------------------------------------------------------------
    struct __Resolved
    {
        virtual ~__Resolved() {};
        std::string signature;
        bool resolved = false;
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
    enum class ScopeType
    {
        Global,
        Local,
        Type
    };
    ScopeType type = ScopeType::Local;
    std::vector<Symbol*> symbols;
    std::multimap<std::string, Symbol*> symbolLookup;
    Symbol* owningSymbol = nullptr;
    bool unreachable = false;
};

} // namespace GPULang
//------------------------------------------------------------------------------