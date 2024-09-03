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

#define _IMPLEMENT_ATTRIBUTES() std::vector<Attribute> attributes;
#define _IMPLEMENT_ANNOTATIONS() std::vector<Annotation> annotations;

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
        IntExpressionType,
        UIntExpressionType,
        EnumExpressionType,
        BoolExpressionType,
        SwizzleExpressionType,
        StringExpressionType,
        BinaryExpressionType,
        SymbolExpressionType,
        AccessExpressionType,
        InitializerExpressionType,
        ArrayIndexExpressionType,
        AssignExpressionType,
        CallExpressionType,
        CommaExpressionType,
        TernaryExpressionType,
        UnaryExpressionType,
        BreakStatementType,
        ContinueStatementType,
        ExpressionStatementType,
        ForStatementType,
        IfStatementType,
        ReturnStatementType,
        ScopeStatementType,
        SwitchStatementType,
        WhileStatementType,

        NumSymbolTypes
    };

    struct Location
    {
        std::string file;
        int line;
        int column;

        Location()
            : line(-1)
            , column(-1)
        {}
    };

    /// constructor
    Symbol();
    /// destructor
    virtual ~Symbol();

    //------------------------------------------------------------------------------
    /**
        Parser state
    */
    //------------------------------------------------------------------------------
    std::string name;
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
        std::string signature;
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

} // namespace GPULang
//------------------------------------------------------------------------------