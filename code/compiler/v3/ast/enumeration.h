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

    std::vector<std::string> labels;
    std::vector<Expression*> values;
    Type::FullType type;

    struct __Resolved : public Type::__Resolved
    {
        Function uintAssignOperator;
        Variable uintAssignArg;
        Function intAssignOperator;
        Variable intAssignArg;
        Type* typeSymbol;
    };
};

} // namespace GPULang
