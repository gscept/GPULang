#pragma once
//------------------------------------------------------------------------------
/**
    Matrix 4x2

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "type.h"
#include "ast/function.h"
namespace GPULang
{

struct Mat4x2 : public Type
{
    /// constructor
    Mat4x2();
};
extern Mat4x2 Mat4x2Type;

extern Function Mat4x2_floatConstructor;
extern Function Mat4x2_vectorConstructor;

extern Function Mat4x2_additionOperator;
extern Function Mat4x2_subtractionOperator;
extern Function Mat4x2_multiplyOperator;
extern Function Mat4x2_additionAssignOperator;
extern Function Mat4x2_subtractionAssignOperator;
extern Function Mat4x2_multiplyAssignOperator;

extern Function Mat4x2_vectorTransformOperator;
extern Function Mat4x2_scaleOperator;

extern Function Mat4x2_assignOperator;

extern Function Mat4x2_elementAccessOperatorInt;
extern Function Mat4x2_elementAccessOperatorUInt;

} // namespace GPULang
