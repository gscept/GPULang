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

    static Function identityConstructor;
    static Function floatConstructor;
    static Function vectorConstructor;

    static Function additionOperator;
    static Function subtractionOperator;
    static Function multiplyOperator;
    static Function additionAssignOperator;
    static Function subtractionAssignOperator;
    static Function multiplyAssignOperator;

    static Function vectorTransformOperator;
    static Function scaleOperator;
    
    static Function assignOperator;

    static Function elementAccessOperatorInt;
    static Function elementAccessOperatorUInt;
};

} // namespace GPULang
