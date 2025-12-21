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

    FixedArray<FixedString> labels;
    FixedArray<Expression*> values;
    FixedArray<Symbol::Location> labelLocations;
    Type::FullType type;
    bool builtin = false;
    Variable fromUnderlyingTypeArg, toUnderlyingTypeArg, eqOpArg, neqOpArg;
    Function fromUnderlyingType, toUnderlyingType, eqOp, neqOp;

    struct __Resolved : public Type::__Resolved
    {
        virtual ~__Resolved() { typeSymbol = nullptr; };
        Type* typeSymbol = nullptr;
    } enumResolved;
    Enumeration::__Resolved* thisResolved = nullptr;
};

} // namespace GPULang
