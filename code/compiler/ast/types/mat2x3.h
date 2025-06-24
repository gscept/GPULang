#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 2x3

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat2x3 : public Type
{
    /// constructor
    Mat2x3();
};
extern Mat2x3 Mat2x3Type;

extern Function Mat2x3_floatConstructor;
extern Function Mat2x3_vectorConstructor;

extern Function Mat2x3_additionOperator;
extern Function Mat2x3_subtractionOperator;
extern Function Mat2x3_multiplyOperator;
extern Function Mat2x3_additionAssignOperator;
extern Function Mat2x3_subtractionAssignOperator;
extern Function Mat2x3_multiplyAssignOperator;

extern Function Mat2x3_vectorTransformOperator;
extern Function Mat2x3_scaleOperator;

extern Function Mat2x3_assignOperator;

extern Function Mat2x3_elementAccessOperatorInt;
extern Function Mat2x3_elementAccessOperatorUInt;

} // namespace GPULang
