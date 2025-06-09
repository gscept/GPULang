#pragma once
//------------------------------------------------------------------------------
/**
    AST for base class for render/blend state

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "symbol.h"
#include "annotation.h"
#include "serialize.h"
#include <vector>
namespace GPULang
{

struct Expression;
struct State : public Symbol
{
    /// destructor
    virtual ~State();

    FixedArray<Expression*> entries;

    struct __Resolved : Symbol::__Resolved
    {
        virtual ~__Resolved() {};
        static const CompareMode StringToCompareMode(const std::string& str);
    };


};

} // namespace GPULang
