#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 2x4

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat2x4 : public Type
{
    /// constructor
    Mat2x4();
};
extern Mat2x4 Mat2x4Type;

extern Function Mat2x4_floatConstructor;
extern Function Mat2x4_vectorConstructor;

extern Function Mat2x4_additionOperator;
extern Function Mat2x4_subtractionOperator;
extern Function Mat2x4_multiplyOperator;
extern Function Mat2x4_additionAssignOperator;
extern Function Mat2x4_subtractionAssignOperator;
extern Function Mat2x4_multiplyAssignOperator;

extern Function Mat2x4_vectorTransformOperator;
extern Function Mat2x4_scaleOperator;

extern Function Mat2x4_assignOperator;

extern Function Mat2x4_elementAccessOperatorInt;
extern Function Mat2x4_elementAccessOperatorUInt;

} // namespace GPULang
