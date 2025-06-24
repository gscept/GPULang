#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 3x4

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat3x4 : public Type
{
    /// constructor
    Mat3x4();
};
extern Mat3x4 Mat3x4Type;

extern Function Mat3x4_floatConstructor;
extern Function Mat3x4_vectorConstructor;

extern Function Mat3x4_additionOperator;
extern Function Mat3x4_subtractionOperator;
extern Function Mat3x4_multiplyOperator;
extern Function Mat3x4_additionAssignOperator;
extern Function Mat3x4_subtractionAssignOperator;
extern Function Mat3x4_multiplyAssignOperator;

extern Function Mat3x4_vectorTransformOperator;
extern Function Mat3x4_scaleOperator;

extern Function Mat3x4_assignOperator;

extern Function Mat3x4_elementAccessOperatorInt;
extern Function Mat3x4_elementAccessOperatorUInt;

} // namespace GPULang
