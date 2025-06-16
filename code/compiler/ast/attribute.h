#pragma once
//------------------------------------------------------------------------------
/**
    An attribute holds some sort of symbol-related metadata

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <string>
#include "util.h"
#include "symbol.h"
#include "expressions/expression.h"
namespace GPULang
{

struct Compiler;
struct Expression;
struct Attribute
{
    FixedString name;
    Expression* expression;
    Symbol::Location location;

    Attribute() 
        : expression(nullptr)
    {}

    Attribute(const char* name)
        : name(name)
        , expression(nullptr)
    {
    }

    Attribute(Attribute&& rhs) noexcept
    {
        this->name = std::move(rhs.name);
        this->expression = rhs.expression;
        this->location = rhs.location;

        rhs.expression = nullptr;
    }

    ~Attribute() 
    {
        if (this->expression != nullptr)
            this->expression->~Expression();
        this->expression = nullptr;
    }

    /// Copy constructor
    Attribute(const Attribute& rhs) noexcept
    {
        this->name = rhs.name;
        this->expression = rhs.expression;
    }

    /// converts attribute to string
    bool ToString(TransientString& out) const;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Attribute::ToString(TransientString& out) const
{
    bool ret = true;
    if (this->expression == nullptr)
        out.Append(this->name);
    else
    {
        ValueUnion value;
        this->expression->EvalValue(value);
        switch (this->expression->symbolType)
        {
            case Symbol::FloatExpressionType:
            {
                out.Concatenate<false>(this->name, "(", value.f[0], ")");
                break;
            }
            case Symbol::UIntExpressionType:
            {
                out.Concatenate<false>(this->name, "(", value.ui[0], ")");
                break;
            }
            case Symbol::IntExpressionType:
            {
                out.Concatenate<false>(this->name, "(", value.i[0], ")");
                break;
            }
            case Symbol::BoolExpressionType:
            {
                out.Concatenate<false>(this->name, "(", value.b[0], ")");
                break;
            }
            case Symbol::StringExpressionType:
            {
                const TransientString& str = this->expression->EvalString();
                out.Concatenate<false>(this->name, "(", str, ")");
                break;
            }
            case Symbol::SymbolExpressionType:
            {
                FixedString str;
                ret &= this->expression->EvalSymbol(str);
                out.Concatenate<false>(this->name, "(", ret, ")");
                break;
            }
        }
    }
    return ret;
}

} // namespace GPULang
