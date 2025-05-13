#pragma once
//------------------------------------------------------------------------------
/**
    Enumeration

    @copyright (C) 2021 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "types/type.h"
#include "ast/function.h"
namespace GPULang
{

struct Enumeration : public Type
{
    /// constructor
    Enumeration();
    /// destructor
    virtual ~Enumeration();

    std::vector<std::string> labels;
    std::vector<Expression*> values;
    std::vector<Symbol::Location> labelLocations;
    Type::FullType type;
    bool builtin = false;

    struct __Resolved : public Type::__Resolved
    {
        virtual ~__Resolved() { typeSymbol = nullptr; };
        Type* typeSymbol;
    };
};

} // namespace GPULang
