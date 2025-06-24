#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 4x3

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat4x3 : public Type
{
    /// constructor
    Mat4x3();
};
extern Mat4x3 Mat4x3Type;

extern Function Mat4x3_floatConstructor;
extern Function Mat4x3_vectorConstructor;

extern Function Mat4x3_additionOperator;
extern Function Mat4x3_subtractionOperator;
extern Function Mat4x3_multiplyOperator;
extern Function Mat4x3_additionAssignOperator;
extern Function Mat4x3_subtractionAssignOperator;
extern Function Mat4x3_multiplyAssignOperator;

extern Function Mat4x3_vectorTransformOperator;
extern Function Mat4x3_scaleOperator;

extern Function Mat4x3_assignOperator;

extern Function Mat4x3_elementAccessOperatorInt;
extern Function Mat4x3_elementAccessOperatorUInt;

} // namespace GPULang
