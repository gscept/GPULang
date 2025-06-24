#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 3x2

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat3x2 : public Type
{
    /// constructor
    Mat3x2();
};
extern Mat3x2 Mat3x2Type;

extern Function Mat3x2_floatConstructor;
extern Function Mat3x2_vectorConstructor;

extern Function Mat3x2_additionOperator;
extern Function Mat3x2_subtractionOperator;
extern Function Mat3x2_multiplyOperator;
extern Function Mat3x2_additionAssignOperator;
extern Function Mat3x2_subtractionAssignOperator;
extern Function Mat3x2_multiplyAssignOperator;

extern Function Mat3x2_vectorTransformOperator;
extern Function Mat3x2_scaleOperator;

extern Function Mat3x2_assignOperator;

extern Function Mat3x2_elementAccessOperatorInt;
extern Function Mat3x2_elementAccessOperatorUInt;

} // namespace GPULang
