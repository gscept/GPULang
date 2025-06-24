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
};
extern Mat4x4 Mat4x4Type;

extern Function Mat4x4_identityConstructor;
extern Function Mat4x4_floatConstructor;
extern Function Mat4x4_vectorConstructor;

extern Function Mat4x4_additionOperator;
extern Function Mat4x4_subtractionOperator;
extern Function Mat4x4_multiplyOperator;
extern Function Mat4x4_additionAssignOperator;
extern Function Mat4x4_subtractionAssignOperator;
extern Function Mat4x4_multiplyAssignOperator;

extern Function Mat4x4_vectorTransformOperator;
extern Function Mat4x4_scaleOperator;

extern Function Mat4x4_assignOperator;

extern Function Mat4x4_elementAccessOperatorInt;
extern Function Mat4x4_elementAccessOperatorUInt;

} // namespace GPULang
