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
    FixedString name;
    Expression* value;
    Symbol::Location location;

    Annotation()
        : value(nullptr)
    {}

    ~Annotation()
    {
        if (this->value != nullptr)
            this->value->~Expression();
    }

    /// Copy constructor
    Annotation(const Annotation& rhs) noexcept
    {
        this->name = rhs.name;
        this->value = rhs.value;
    }

    Annotation(Annotation&& rhs) noexcept
    {
        this->name = std::move(rhs.name);
        this->value = rhs.value;

        rhs.value = nullptr;
    }
};

} // namespace GPULang
