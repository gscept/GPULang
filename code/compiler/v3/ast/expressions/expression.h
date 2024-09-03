#pragma once
//------------------------------------------------------------------------------
/**
    @class GPULang::Expression
    
    Base class for expressions. 
    In AnyFX, expressions are purely aritmetic, meaning they can only return a value.
    
    (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "ast/symbol.h"
#include "v3/ast/types/type.h"

namespace GPULang
{

struct Compiler;
struct Expression : public Symbol
{
    /// constructor
    Expression();
    /// destructor
    virtual ~Expression();

    /// Evaluate expression and store data in resolved
    virtual bool Resolve(Compiler* compiler);

    /// eval type
    virtual bool EvalType(Type::FullType& out) const;
    /// eval symbol
    virtual bool EvalSymbol(std::string& out) const;
    /// evaluates expression as an integer
    virtual bool EvalInt(int& out) const;
    /// evaluates expression as an integer
    virtual bool EvalUInt(unsigned& out) const;
    /// evaulates expression as a float
    virtual bool EvalFloat(float& out) const;
    /// evaluates expression as a boolean
    virtual bool EvalBool(bool& out) const;
    /// evaluates expression as a string
    virtual std::string EvalString() const;

    bool isLhsValue;
    bool isDeclaration;

    struct __Resolved : public Symbol::__Resolved
    {
        union
        {
            int32_t i;
            uint32_t ui;
            float f;
            bool b;
        } value;
    };
}; 
} // namespace GPULang
//------------------------------------------------------------------------------