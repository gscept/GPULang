#pragma once
//------------------------------------------------------------------------------
/**
    Annotations contains a list of key-value pairs used to pass metadata from 
    source file to reflection, which distinguishes them from attributes which
    are used by the compiler to understand how to transform the code

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <string>
#include "symbol.h"
#include "binwriter.h"
#include "expressions/expression.h"
namespace GPULang 
{

struct Compiler;
struct Annotation
{
    std::string name;
    Expression* value;

    Annotation()
        : value(nullptr)
    {}

    ~Annotation()
    {
        if (this->value)
            delete this->value;
    }

    /// Move constructor
    Annotation(Annotation&& rhs) noexcept
    {
        this->name = rhs.name;
        this->value = rhs.value;
        rhs.name = "";
        rhs.value = nullptr;
    }
};

} // namespace GPULang