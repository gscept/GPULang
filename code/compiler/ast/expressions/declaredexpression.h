#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::DeclaredExpression
    
     A special generation expression which allows compile time evaluation of whether or not a symbol is declared
    
    (C) 2025 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "expression.h"
#include "ast/types/type.h"

namespace GPULang
{

struct Compiler;
struct DeclaredExpression : public Expression
{
    /// constructor
    DeclaredExpression(const FixedString& symbol);
    /// destructor
    virtual ~DeclaredExpression();

    /// Evaluate expression and store data in resolved
    bool Resolve(Compiler* compiler);

    /// eval type
    bool EvalType(Type::FullType& out) const;
    /// eval type symbol
    bool EvalTypeSymbol(Type*& out) const;
    /// eval symbol
    bool EvalSymbol(FixedString& out) const;
    /// Evaluate value
    bool EvalValue(ValueUnion& out) const;
    ///// evaluates expression as an integer
    //virtual bool EvalIntVec(std::vector<int>& out) const;
    ///// evaluates expression as an integer
    //virtual bool EvalUIntVec(std::vector<unsigned>& out) const;
    ///// evaulates expression as a float
    //virtual bool EvalFloatVec(std::vector<float>& out) const;
    ///// evaluates expression as a boolean
    //virtual bool EvalBoolVec(std::vector<bool>& out) const;
    /// evaluates access flags
    bool EvalAccessFlags(unsigned& out) const;
    /// evaluates storage
    bool EvalStorage(Storage& out) const;
    
    /// evaluates expression as a string
    TransientString EvalString() const;

    FixedString symbol;

    struct __Resolved : public Symbol::__Resolved
    {
        virtual ~__Resolved() {};
        bool declared = false;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------
