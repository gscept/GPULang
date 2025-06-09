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
    bool ToString(std::string& out) const;
};

//------------------------------------------------------------------------------
/**
*/
inline bool
Attribute::ToString(std::string& out) const
{
    bool ret = true;
    if (this->expression == nullptr)
        out.append(Format("%s", this->name.c_str()));
    else
    {
        ValueUnion value;
        this->expression->EvalValue(value);
        switch (this->expression->symbolType)
        {
        case Symbol::FloatExpressionType:
        {
            out.append(Format("%s(%f)", this->name.c_str(), value.f[0]));
            break;
        }
        case Symbol::UIntExpressionType:
        {
            out.append(Format("%s(%d)", this->name.c_str(), value.ui[0]));
            break;
        }
        case Symbol::IntExpressionType:
        {
            out.append(Format("%s(%d)", this->name.c_str(), value.i[0]));
            break;
        }
        case Symbol::BoolExpressionType:
        {
            out.append(Format("%s(%d)", this->name.c_str(), value.b[0]));
            break;
        }
        case Symbol::StringExpressionType:
        {
            std::string str = this->expression->EvalString();
            out.append(Format("%s(%s)", this->name.c_str(), str.c_str()));
            break;
        }
        case Symbol::SymbolExpressionType:
        {
            std::string str;
            ret &= this->expression->EvalSymbol(str);
            out.append(Format("%s(%s)", this->name.c_str(), str.c_str()));
            break;
        }
        }
    }
    return ret;
}

} // namespace GPULang
