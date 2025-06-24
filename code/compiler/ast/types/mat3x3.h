#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 3x3

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat3x3 : public Type
{
    /// constructor
    Mat3x3();
};
extern Mat3x3 Mat3x3Type;

extern Function Mat3x3_identityConstructor;
extern Function Mat3x3_floatConstructor;
extern Function Mat3x3_vectorConstructor;

extern Function Mat3x3_additionOperator;
extern Function Mat3x3_subtractionOperator;
extern Function Mat3x3_multiplyOperator;
extern Function Mat3x3_additionAssignOperator;
extern Function Mat3x3_subtractionAssignOperator;
extern Function Mat3x3_multiplyAssignOperator;

extern Function Mat3x3_vectorTransformOperator;
extern Function Mat3x3_scaleOperator;

extern Function Mat3x3_assignOperator;

extern Function Mat3x3_elementAccessOperatorInt;
extern Function Mat3x3_elementAccessOperatorUInt;

} // namespace GPULang
