#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 2x2

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat2x2 : public Type
{
    /// constructor
    Mat2x2();
};
extern Mat2x2 Mat2x2Type;

extern Function Mat2x2_identityConstructor;
extern Function Mat2x2_floatConstructor;
extern Function Mat2x2_vectorConstructor;

extern Function Mat2x2_additionOperator;
extern Function Mat2x2_subtractionOperator;
extern Function Mat2x2_multiplyOperator;
extern Function Mat2x2_additionAssignOperator;
extern Function Mat2x2_subtractionAssignOperator;
extern Function Mat2x2_multiplyAssignOperator;

extern Function Mat2x2_vectorTransformOperator;
extern Function Mat2x2_scaleOperator;

extern Function Mat2x2_assignOperator;

extern Function Mat2x2_elementAccessOperatorInt;
extern Function Mat2x2_elementAccessOperatorUInt;

} // namespace GPULang
