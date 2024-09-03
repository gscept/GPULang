#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 4x4

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat4x4 : public Type
{
    /// constructor
    Mat4x4();

    Function floatConstructor;
    Function vectorConstructor;

    Function additionOperator;
    Function subtractionOperator;
    Function multiplyOperator;
    Function additionAssignOperator;
    Function subtractionAssignOperator;
    Function multiplyAssignOperator;

    Function vectorTransformOperator;

    Function assignOperator;

    Function bracketOperatorInt;
    Function bracketOperatorUInt;
};

} // namespace GPULang
