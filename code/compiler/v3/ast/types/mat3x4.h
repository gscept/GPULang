#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 3x4

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace AnyFX
{

struct Mat3x4 : public Type
{
    /// constructor
    Mat3x4();

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

} // namespace AnyFX
