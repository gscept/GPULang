//------------------------------------------------------------------------------
//  @file type.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "type.h"
#include "builtins.h"
#include "ast/expressions/expression.h"
#include "ast/expressions/uintexpression.h"
#include <set>

#if __WIN32__
#define forceinline __forceinline
#else
#define forceinline __attribute__((always_inline))
#endif
namespace GPULang
{

struct Namer
{
    Namer()
    {
        BoolType.name = ConstantString("b8");
        Bool2Type.name = ConstantString("b8x2");
        Bool3Type.name = ConstantString("b8x3");
        Bool4Type.name = ConstantString("b8x4");
        FloatType.name = ConstantString("f32");
        Float2Type.name = ConstantString("f32x2");
        Float3Type.name = ConstantString("f32x3");
        Float4Type.name = ConstantString("f32x4");
        IntType.name = ConstantString("i32");
        Int2Type.name = ConstantString("i32x2");
        Int3Type.name = ConstantString("i32x3");
        Int4Type.name = ConstantString("i32x4");
        UIntType.name = ConstantString("u32");
        UInt2Type.name = ConstantString("u32x2");
        UInt3Type.name = ConstantString("u32x3");
        UInt4Type.name = ConstantString("u32x4");
        Mat2x2Type.name = ConstantString("f32x2x2");
        Mat2x3Type.name = ConstantString("f32x2x3");
        Mat2x4Type.name = ConstantString("f32x2x4");
        Mat3x2Type.name = ConstantString("f32x3x2");
        Mat3x3Type.name = ConstantString("f32x3x3");
        Mat3x4Type.name = ConstantString("f32x3x4");
        Mat4x2Type.name = ConstantString("f32x4x2");
        Mat4x3Type.name = ConstantString("f32x4x3");
        Mat4x4Type.name = ConstantString("f32x4x4");
    }
};
Namer GlobalNamer;
Function Bool_ctor_UInt;
Function Bool_ctor_Int;
Function Bool_ororOperator;
Function Bool_andandOperator;
Function Bool_eOperator;
Function Bool_neOperator;
Function Bool_boolAssignOperator;
Function Bool_intAssignOperator;
Function Bool_uintAssignOperator;
Bool  BoolType;

Function Bool2_ctor_XY;
Function Bool2_ctorSingleValue;
Function Bool2_ororOperator;
Function Bool2_andandOperator;
Function Bool2_eOperator;
Function Bool2_neOperator;
Function Bool2_elementAccessOperatorInt;
Function Bool2_elementAccessOperatorUInt;
Bool2 Bool2Type;

Function Bool3_ctor_XYZ;
Function Bool3_ctorSingleValue;
Function Bool3_ororOperator;
Function Bool3_andandOperator;
Function Bool3_eOperator;
Function Bool3_neOperator;
Function Bool3_elementAccessOperatorInt;
Function Bool3_elementAccessOperatorUInt;
Bool3 Bool3Type;

Function Bool4_ctor_XYZW;
Function Bool4_ctorSingleValue;
Function Bool4_ororOperator;
Function Bool4_andandOperator;
Function Bool4_eOperator;
Function Bool4_neOperator;
Function Bool4_elementAccessOperatorInt;
Function Bool4_elementAccessOperatorUInt;
Bool4 Bool4Type;

Function Int_ctor_UInt;
Function Int_ctor_Bool;
Function Int_ctor_Float;
Function Int_andOperator;
Function Int_orOperator;
Function Int_xorOperator;
Function Int_leftShiftOperator;
Function Int_rightShiftOperator;
Function Int_ororOperator;
Function Int_andandOperator;
Function Int_additionOperator;
Function Int_subtractionOperator;
Function Int_multiplicationOperator;
Function Int_divisionOperator;
Function Int_modOperator;
Function Int_andAssignOperator;
Function Int_orAssignOperator;
Function Int_xorAssignOperator;
Function Int_leftShiftAssignOperator;
Function Int_rightShiftAssignOperator;
Function Int_additionAssignOperator;
Function Int_subtractionAssignOperator;
Function Int_multiplicationAssignOperator;
Function Int_divisionAssignOperator;
Function Int_moduloAssignOperator;
Function Int_ltOperator;
Function Int_lteOperator;
Function Int_gtOperator;
Function Int_gteOperator;
Function Int_eOperator;
Function Int_neOperator;
Function Int_floatAssignOperator;
Function Int_uintAssignOperator;
Function Int_boolAssignOperator;
Int  IntType;

Function Int2_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
     Function type##_##ctor;

    INT2_CTOR_LIST
#undef X

Function Int2_andOperator;
Function Int2_orOperator;
Function Int2_xorOperator;
Function Int2_leftShiftOperator;
Function Int2_rightShiftOperator;
Function Int2_additionOperator;
Function Int2_subtractionOperator;
Function Int2_multiplicationOperator;
Function Int2_divisionOperator;
Function Int2_modOperator;
Function Int2_scaleOperator;
Function Int2_andAssignOperator;
Function Int2_orAssignOperator;
Function Int2_xorAssignOperator;
Function Int2_leftShiftAssignOperator;
Function Int2_rightShiftAssignOperator;
Function Int2_additionAssignOperator;
Function Int2_subtractionAssignOperator;
Function Int2_multiplicationAssignOperator;
Function Int2_divisionAssignOperator;
Function Int2_moduloAssignOperator;
Function Int2_ltOperator;
Function Int2_lteOperator;
Function Int2_gtOperator;
Function Int2_gteOperator;
Function Int2_eOperator;
Function Int2_neOperator;
Function Int2_elementAccessOperatorInt;
Function Int2_elementAccessOperatorUInt;
Int2 Int2Type;

Function Int3_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
     Function type##_##ctor;

INT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
     Function type##_##ctor;

INT3_CTOR2_LIST
#undef X

Function Int3_andOperator;
Function Int3_orOperator;
Function Int3_xorOperator;
Function Int3_leftShiftOperator;
Function Int3_rightShiftOperator;
Function Int3_additionOperator;
Function Int3_subtractionOperator;
Function Int3_multiplicationOperator;
Function Int3_divisionOperator;
Function Int3_modOperator;
Function Int3_scaleOperator;
Function Int3_andAssignOperator;
Function Int3_orAssignOperator;
Function Int3_xorAssignOperator;
Function Int3_leftShiftAssignOperator;
Function Int3_rightShiftAssignOperator;
Function Int3_additionAssignOperator;
Function Int3_subtractionAssignOperator;
Function Int3_multiplicationAssignOperator;
Function Int3_divisionAssignOperator;
Function Int3_moduloAssignOperator;
Function Int3_ltOperator;
Function Int3_lteOperator;
Function Int3_gtOperator;
Function Int3_gteOperator;
Function Int3_eOperator;
Function Int3_neOperator;
Function Int3_elementAccessOperatorInt;
Function Int3_elementAccessOperatorUInt;
Int3 Int3Type;

Function Int4_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    Function type##_##ctor;

INT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
     Function type##_##ctor;

INT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
     Function type##_##ctor;

INT4_CTOR3_LIST
#undef X

Function Int4_andOperator;
Function Int4_orOperator;
Function Int4_xorOperator;
Function Int4_leftShiftOperator;
Function Int4_rightShiftOperator;
Function Int4_additionOperator;
Function Int4_subtractionOperator;
Function Int4_multiplicationOperator;
Function Int4_divisionOperator;
Function Int4_modOperator;
Function Int4_scaleOperator;

Function Int4_andAssignOperator;
Function Int4_orAssignOperator;
Function Int4_xorAssignOperator;
Function Int4_leftShiftAssignOperator;
Function Int4_rightShiftAssignOperator;
Function Int4_additionAssignOperator;
Function Int4_subtractionAssignOperator;
Function Int4_multiplicationAssignOperator;
Function Int4_divisionAssignOperator;
Function Int4_moduloAssignOperator;

Function Int4_ltOperator;
Function Int4_lteOperator;
Function Int4_gtOperator;
Function Int4_gteOperator;
Function Int4_eOperator;
Function Int4_neOperator;

Function Int4_elementAccessOperatorInt;
Function Int4_elementAccessOperatorUInt;
Int4 Int4Type;

Function UInt_ctor_Int;
Function UInt_ctor_Bool;
Function UInt_ctor_Float;
Function UInt_andOperator;
Function UInt_orOperator;
Function UInt_xorOperator;
Function UInt_leftShiftOperator;
Function UInt_rightShiftOperator;
Function UInt_ororOperator;
Function UInt_andandOperator;
Function UInt_additionOperator;
Function UInt_subtractionOperator;
Function UInt_multiplicationOperator;
Function UInt_divisionOperator;
Function UInt_modOperator;
Function UInt_andAssignOperator;
Function UInt_orAssignOperator;
Function UInt_xorAssignOperator;
Function UInt_leftShiftAssignOperator;
Function UInt_rightShiftAssignOperator;
Function UInt_additionAssignOperator;
Function UInt_subtractionAssignOperator;
Function UInt_multiplicationAssignOperator;
Function UInt_divisionAssignOperator;
Function UInt_moduloAssignOperator;
Function UInt_ltOperator;
Function UInt_lteOperator;
Function UInt_gtOperator;
Function UInt_gteOperator;
Function UInt_eOperator;
Function UInt_neOperator;
Function UInt_floatAssignOperator;
Function UInt_intAssignOperator;
Function UInt_boolAssignOperator;
UInt  UIntType;

#define X(type, ctor, val, args, splat, size, conversion)\
    Function type##_##ctor;

UINT2_CTOR_LIST
#undef X

Function UInt2_ctor;
Function UInt2_andOperator;
Function UInt2_orOperator;
Function UInt2_xorOperator;
Function UInt2_leftShiftOperator;
Function UInt2_rightShiftOperator;
Function UInt2_additionOperator;
Function UInt2_subtractionOperator;
Function UInt2_multiplicationOperator;
Function UInt2_divisionOperator;
Function UInt2_modOperator;
Function UInt2_scaleOperator;
Function UInt2_andAssignOperator;
Function UInt2_orAssignOperator;
Function UInt2_xorAssignOperator;
Function UInt2_leftShiftAssignOperator;
Function UInt2_rightShiftAssignOperator;
Function UInt2_additionAssignOperator;
Function UInt2_subtractionAssignOperator;
Function UInt2_multiplicationAssignOperator;
Function UInt2_divisionAssignOperator;
Function UInt2_moduloAssignOperator;
Function UInt2_ltOperator;
Function UInt2_lteOperator;
Function UInt2_gtOperator;
Function UInt2_gteOperator;
Function UInt2_eOperator;
Function UInt2_neOperator;
Function UInt2_elementAccessOperatorInt;
Function UInt2_elementAccessOperatorUInt;
UInt2 UInt2Type;

#define X(type, ctor, val, args, splat, size, conversion)\
    Function type##_##ctor;

UINT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type##_##ctor;

UINT3_CTOR2_LIST
#undef X

Function UInt3_ctor;
Function UInt3_andOperator;
Function UInt3_orOperator;
Function UInt3_xorOperator;
Function UInt3_leftShiftOperator;
Function UInt3_rightShiftOperator;
Function UInt3_additionOperator;
Function UInt3_subtractionOperator;
Function UInt3_multiplicationOperator;
Function UInt3_divisionOperator;
Function UInt3_modOperator;
Function UInt3_scaleOperator;
Function UInt3_andAssignOperator;
Function UInt3_orAssignOperator;
Function UInt3_xorAssignOperator;
Function UInt3_leftShiftAssignOperator;
Function UInt3_rightShiftAssignOperator;
Function UInt3_additionAssignOperator;
Function UInt3_subtractionAssignOperator;
Function UInt3_multiplicationAssignOperator;
Function UInt3_divisionAssignOperator;
Function UInt3_moduloAssignOperator;
Function UInt3_ltOperator;
Function UInt3_lteOperator;
Function UInt3_gtOperator;
Function UInt3_gteOperator;
Function UInt3_eOperator;
Function UInt3_neOperator;
Function UInt3_elementAccessOperatorInt;
Function UInt3_elementAccessOperatorUInt;
UInt3 UInt3Type;

#define X(type, ctor, val, args, splat, size, conversion)\
    Function type##_##ctor;

UINT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type##_##ctor;

UINT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg2)\
    Function type##_##ctor;

UINT4_CTOR3_LIST
#undef X

Function UInt4_ctor;
Function UInt4_andOperator;
Function UInt4_orOperator;
Function UInt4_xorOperator;
Function UInt4_leftShiftOperator;
Function UInt4_rightShiftOperator;
Function UInt4_additionOperator;
Function UInt4_subtractionOperator;
Function UInt4_multiplicationOperator;
Function UInt4_divisionOperator;
Function UInt4_modOperator;
Function UInt4_scaleOperator;
Function UInt4_andAssignOperator;
Function UInt4_orAssignOperator;
Function UInt4_xorAssignOperator;
Function UInt4_leftShiftAssignOperator;
Function UInt4_rightShiftAssignOperator;
Function UInt4_additionAssignOperator;
Function UInt4_subtractionAssignOperator;
Function UInt4_multiplicationAssignOperator;
Function UInt4_divisionAssignOperator;
Function UInt4_moduloAssignOperator;
Function UInt4_ltOperator;
Function UInt4_lteOperator;
Function UInt4_gtOperator;
Function UInt4_gteOperator;
Function UInt4_eOperator;
Function UInt4_neOperator;
Function UInt4_elementAccessOperatorInt;
Function UInt4_elementAccessOperatorUInt;
UInt4 UInt4Type;

Function Float_ctor_UInt;
Function Float_ctor_Int;
Function Float_additionOperator;
Function Float_subtractionOperator;
Function Float_multiplicationOperator;
Function Float_divisionOperator;
Function Float_modOperator;
Function Float_additionAssignOperator;
Function Float_subtractionAssignOperator;
Function Float_multiplicationAssignOperator;
Function Float_divisionAssignOperator;
Function Float_ltOperator;
Function Float_lteOperator;
Function Float_gtOperator;
Function Float_gteOperator;
Function Float_eOperator;
Function Float_neOperator;
Function Float_intAssignOperator;
Function Float_uintAssignOperator;
Float  FloatType;

Function Float2_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    Function type##_##ctor;

FLOAT2_CTOR_LIST
#undef X

Function Float2_additionOperator;
Function Float2_subtractionOperator;
Function Float2_multiplicationOperator;
Function Float2_divisionOperator;
Function Float2_modOperator;
Function Float2_scaleOperator;
Function Float2_matrix24Mul;
Function Float2_matrix23Mul;
Function Float2_matrix22Mul;
Function Float2_additionAssignOperator;
Function Float2_subtractionAssignOperator;
Function Float2_multiplicationAssignOperator;
Function Float2_divisionAssignOperator;
Function Float2_ltOperator;
Function Float2_lteOperator;
Function Float2_gtOperator;
Function Float2_gteOperator;
Function Float2_eOperator;
Function Float2_neOperator;
Function Float2_elementAccessOperatorInt;
Function Float2_elementAccessOperatorUInt;
Float2 Float2Type;

Function Float3_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
    Function type##_##ctor;

FLOAT3_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
    Function type##_##ctor;

FLOAT3_CTOR2_LIST
#undef X

Function Float3_additionOperator;
Function Float3_subtractionOperator;
Function Float3_multiplicationOperator;
Function Float3_divisionOperator;
Function Float3_modOperator;
Function Float3_scaleOperator;
Function Float3_matrix34Mul;
Function Float3_matrix33Mul;
Function Float3_matrix32Mul;
Function Float3_additionAssignOperator;
Function Float3_subtractionAssignOperator;
Function Float3_multiplicationAssignOperator;
Function Float3_divisionAssignOperator;
Function Float3_ltOperator;
Function Float3_lteOperator;
Function Float3_gtOperator;
Function Float3_gteOperator;
Function Float3_eOperator;
Function Float3_neOperator;
Function Float3_elementAccessOperatorInt;
Function Float3_elementAccessOperatorUInt;
Float3 Float3Type;

Function Float4_ctor;
#define X(type, ctor, val, args, splat, size, conversion)\
     Function type##_##ctor;

FLOAT4_CTOR_LIST
#undef X

#define X(type, ctor, arg0, arg1)\
     Function type##_##ctor;

FLOAT4_CTOR2_LIST
#undef X

#define X(type, ctor, arg0, arg1, arg3)\
     Function type##_##ctor;

FLOAT4_CTOR3_LIST
#undef X

Function Float4_additionOperator;
Function Float4_subtractionOperator;
Function Float4_multiplicationOperator;
Function Float4_divisionOperator;
Function Float4_modOperator;
Function Float4_scaleOperator;
Function Float4_matrix44Mul;
Function Float4_matrix43Mul;
Function Float4_matrix42Mul;
Function Float4_additionAssignOperator;
Function Float4_subtractionAssignOperator;
Function Float4_multiplicationAssignOperator;
Function Float4_divisionAssignOperator;
Function Float4_ltOperator;
Function Float4_lteOperator;
Function Float4_gtOperator;
Function Float4_gteOperator;
Function Float4_eOperator;
Function Float4_neOperator;
Function Float4_elementAccessOperatorInt;
Function Float4_elementAccessOperatorUInt;
Float4 Float4Type;

Function Mat2x2_identityConstructor;
Function Mat2x2_floatConstructor;
Function Mat2x2_vectorConstructor;
Function Mat2x2_additionOperator;
Function Mat2x2_subtractionOperator;
Function Mat2x2_multiplyOperator;
Function Mat2x2_additionAssignOperator;
Function Mat2x2_subtractionAssignOperator;
Function Mat2x2_multiplyAssignOperator;
Function Mat2x2_vectorTransformOperator;
Function Mat2x2_scaleOperator;
Function Mat2x2_assignOperator;
Function Mat2x2_elementAccessOperatorInt;
Function Mat2x2_elementAccessOperatorUInt;
Mat2x2 Mat2x2Type;

Function Mat2x3_floatConstructor;
Function Mat2x3_vectorConstructor;
Function Mat2x3_additionOperator;
Function Mat2x3_subtractionOperator;
Function Mat2x3_multiplyOperator;
Function Mat2x3_additionAssignOperator;
Function Mat2x3_subtractionAssignOperator;
Function Mat2x3_multiplyAssignOperator;
Function Mat2x3_vectorTransformOperator;
Function Mat2x3_scaleOperator;
Function Mat2x3_assignOperator;
Function Mat2x3_elementAccessOperatorInt;
Function Mat2x3_elementAccessOperatorUInt;
Mat2x3 Mat2x3Type;

Function Mat2x4_floatConstructor;
Function Mat2x4_vectorConstructor;
Function Mat2x4_additionOperator;
Function Mat2x4_subtractionOperator;
Function Mat2x4_multiplyOperator;
Function Mat2x4_additionAssignOperator;
Function Mat2x4_subtractionAssignOperator;
Function Mat2x4_multiplyAssignOperator;
Function Mat2x4_vectorTransformOperator;
Function Mat2x4_scaleOperator;
Function Mat2x4_assignOperator;
Function Mat2x4_elementAccessOperatorInt;
Function Mat2x4_elementAccessOperatorUInt;
Mat2x4 Mat2x4Type;

Function Mat3x2_floatConstructor;
Function Mat3x2_vectorConstructor;
Function Mat3x2_additionOperator;
Function Mat3x2_subtractionOperator;
Function Mat3x2_multiplyOperator;
Function Mat3x2_additionAssignOperator;
Function Mat3x2_subtractionAssignOperator;
Function Mat3x2_multiplyAssignOperator;
Function Mat3x2_vectorTransformOperator;
Function Mat3x2_scaleOperator;
Function Mat3x2_assignOperator;
Function Mat3x2_elementAccessOperatorInt;
Function Mat3x2_elementAccessOperatorUInt;
Mat3x2 Mat3x2Type;

Function Mat3x3_identityConstructor;
Function Mat3x3_floatConstructor;
Function Mat3x3_vectorConstructor;
Function Mat3x3_additionOperator;
Function Mat3x3_subtractionOperator;
Function Mat3x3_multiplyOperator;
Function Mat3x3_additionAssignOperator;
Function Mat3x3_subtractionAssignOperator;
Function Mat3x3_multiplyAssignOperator;
Function Mat3x3_vectorTransformOperator;
Function Mat3x3_scaleOperator;
Function Mat3x3_assignOperator;
Function Mat3x3_elementAccessOperatorInt;
Function Mat3x3_elementAccessOperatorUInt;
Mat3x3 Mat3x3Type;

Function Mat3x4_floatConstructor;
Function Mat3x4_vectorConstructor;
Function Mat3x4_additionOperator;
Function Mat3x4_subtractionOperator;
Function Mat3x4_multiplyOperator;
Function Mat3x4_additionAssignOperator;
Function Mat3x4_subtractionAssignOperator;
Function Mat3x4_multiplyAssignOperator;
Function Mat3x4_vectorTransformOperator;
Function Mat3x4_scaleOperator;
Function Mat3x4_assignOperator;
Function Mat3x4_elementAccessOperatorInt;
Function Mat3x4_elementAccessOperatorUInt;
Mat3x4 Mat3x4Type;


Function Mat4x2_floatConstructor;
Function Mat4x2_vectorConstructor;
Function Mat4x2_additionOperator;
Function Mat4x2_subtractionOperator;
Function Mat4x2_multiplyOperator;
Function Mat4x2_additionAssignOperator;
Function Mat4x2_subtractionAssignOperator;
Function Mat4x2_multiplyAssignOperator;
Function Mat4x2_vectorTransformOperator;
Function Mat4x2_scaleOperator;
Function Mat4x2_assignOperator;
Function Mat4x2_elementAccessOperatorInt;
Function Mat4x2_elementAccessOperatorUInt;
Mat4x2 Mat4x2Type;

Function Mat4x3_floatConstructor;
Function Mat4x3_vectorConstructor;
Function Mat4x3_additionOperator;
Function Mat4x3_subtractionOperator;
Function Mat4x3_multiplyOperator;
Function Mat4x3_additionAssignOperator;
Function Mat4x3_subtractionAssignOperator;
Function Mat4x3_multiplyAssignOperator;
Function Mat4x3_vectorTransformOperator;
Function Mat4x3_scaleOperator;
Function Mat4x3_assignOperator;
Function Mat4x3_elementAccessOperatorInt;
Function Mat4x3_elementAccessOperatorUInt;
Mat4x3 Mat4x3Type;

Function Mat4x4_identityConstructor;
Function Mat4x4_floatConstructor;
Function Mat4x4_vectorConstructor;
Function Mat4x4_additionOperator;
Function Mat4x4_subtractionOperator;
Function Mat4x4_multiplyOperator;
Function Mat4x4_additionAssignOperator;
Function Mat4x4_subtractionAssignOperator;
Function Mat4x4_multiplyAssignOperator;
Function Mat4x4_vectorTransformOperator;
Function Mat4x4_scaleOperator;
Function Mat4x4_assignOperator;
Function Mat4x4_elementAccessOperatorInt;
Function Mat4x4_elementAccessOperatorUInt;
Mat4x4 Mat4x4Type;

CompareModeType CompareModeTypeType;
StencilOpType StencilOpTypeType;
ExecutionScopeType ExecutionScopeTypeType;
MemorySemanticsType MemorySemanticsTypeType;

Variable StencilState_fail;
Variable StencilState_pass;
Variable StencilState_depthFail;
Variable StencilState_compare;
Variable StencilState_compareMask;
Variable StencilState_writeMask;
Variable StencilState_referenceMask;
StencilStateType StencilStateTypeType;

Enumeration RenderState_polygonModeEnum;
Enumeration RenderState_cullModeEnum;
Enumeration RenderState_windingOrderModeEnum;
Enumeration RenderState_logicOpModeEnum;
Enumeration RenderState_blendFactorModeEnum;
Enumeration RenderState_blendOpModeEnum;
Enumeration RenderState_colorComponentMaskEnum;
Variable RenderState_depthClampEnabled;
Variable RenderState_noRasterization;
Variable RenderState_polygonMode;
Variable RenderState_cullMode;
Variable RenderState_windingOrder;
Variable RenderState_depthBiasEnabled;
Variable RenderState_depthBiasFactor;
Variable RenderState_depthBiasClamp;
Variable RenderState_depthBiasSlopeFactor;
Variable RenderState_lineWidth;
Variable RenderState_depthTestEnabled;
Variable RenderState_depthWriteEnabled;
Variable RenderState_depthTestFunction;
Variable RenderState_depthBoundsTestEnabled;
Variable RenderState_minDepthBounds;
Variable RenderState_maxDepthBounds;
Variable RenderState_scissorEnabled;
Variable RenderState_stencilEnabled;
Variable RenderState_logicOpEnabled;
Variable RenderState_logicOp;
Variable RenderState_blendEnabled;
Variable RenderState_sourceBlend;
Variable RenderState_destinationBlend;
Variable RenderState_sourceAlphaBlend;
Variable RenderState_destinationAlphaBlend;
Variable RenderState_blendOp;
Variable RenderState_blendOpAlpha;
Variable RenderState_colorComponentMask;
Variable RenderState_frontStencilState;
Variable RenderState_backStencilState;
RenderStateType RenderStateTypeType;

Enumeration SamplerState_filterModeEnum;
Enumeration SamplerState_addressModeEnum;
Enumeration SamplerState_colorEnum;
Variable SamplerState_allAddress;
Variable SamplerState_addressU;
Variable SamplerState_addressV;
Variable SamplerState_addressW;
Variable SamplerState_allFilter;
Variable SamplerState_minFilter;
Variable SamplerState_magFilter;
Variable SamplerState_mipFilter;
Variable SamplerState_mipLodBias;
Variable SamplerState_anisotropicEnabled;
Variable SamplerState_maxAnisotropy;
Variable SamplerState_compareSamplerEnabled;
Variable SamplerState_compareMode;
Variable SamplerState_minLod;
Variable SamplerState_maxLod;
Variable SamplerState_borderColor;
Variable SamplerState_unnormalizedSamplingEnabled;
SamplerStateType SamplerStateTypeType;

Variable Program_renderState;
Variable Program_vertexShader;
Variable Program_hullShader;
Variable Program_domainShader;
Variable Program_geometryShader;
Variable Program_pixelShader;
Variable Program_computeShader;
ProgramType ProgramTypeType;

FunctionType FunctionTypeType;

Void VoidType;

Texture1D Texture1DType;
Texture1DArray Texture1DArrayType;
Texture2D Texture2DType;
Texture2DArray Texture2DArrayType;
Texture2DMS Texture2DMSType;
Texture2DMSArray Texture2DMSArrayType;
Texture3D Texture3DType;
Texture3DArray Texture3DArrayType;
TextureCube TextureCubeType;
TextureCubeArray TextureCubeArrayType;
SamplerType SamplerTypeType;
PixelCacheType PixelCacheTypeType;
PixelCacheMS PixelCacheMSType;
AccelerationStructureType AccelerationStructureTypeType;

Function* activeFunction = nullptr;

#define __BEGIN_TYPES__ Type* newType = nullptr; TransientArray<Symbol*> types(100);

#define __MAKE_TYPE_CUSTOM(t1, t2)\
newType = &t2##Type;\
newType->name = ConstantString(#t1);\
types.Append(newType);

#define __ADD_LOOKUP(name) DefaultTypes[#name] = newType;


//------------------------------------------------------------------------------
/**
*/
bool 
IsImageFormatInteger(ImageFormat format)
{
    return format >= Rgba32I && format <= R8I;
}

//------------------------------------------------------------------------------
/**
*/
bool 
IsImageFormatUnsigned(ImageFormat format)
{
    return format >= Rgba32U && format <= R8U;
}

//------------------------------------------------------------------------------
/**
*/
Type::Type()
{
    this->symbolType = Symbol::TypeType;
    this->baseType = TypeCode::InvalidType;
    this->rowSize = 1;
    this->columnSize = 1;
    this->byteSize = 4;
    this->category = Type::InvalidCategory;
    this->resolved = nullptr;
    this->scope.type = Scope::ScopeType::Type;
    this->scope.owningSymbol = this;
}

//------------------------------------------------------------------------------
/**
*/
Type::~Type()
{
}

const StaticMap codeToStringMapping =
std::array{
    std::pair{ TypeCode::Void, ConstantString("void") }
    , std::pair{ TypeCode::Float, ConstantString("f32") }
    , std::pair{ TypeCode::Float16, ConstantString("f16") }
    , std::pair{ TypeCode::Int, ConstantString("i32") }
    , std::pair{ TypeCode::Int16, ConstantString("i16") }
    , std::pair{ TypeCode::UInt, ConstantString("u32") }
    , std::pair{ TypeCode::UInt16, ConstantString("u16") }
    , std::pair{ TypeCode::Bool, ConstantString("b8") }
    , std::pair{ TypeCode::Texture1D, ConstantString("texture1D") }
    , std::pair{ TypeCode::Texture1DArray, ConstantString("texture1DArray") }
    , std::pair{ TypeCode::Texture2D, ConstantString("texture2D") }
    , std::pair{ TypeCode::Texture2DArray, ConstantString("texture2DArray") }
    , std::pair{ TypeCode::Texture2DMS, ConstantString("texture2DMS") }
    , std::pair{ TypeCode::Texture2DMSArray, ConstantString("texture2DMSArray") }
    , std::pair{ TypeCode::Texture3D, ConstantString("texture3D") }
    , std::pair{ TypeCode::Texture3DArray, ConstantString("texture3DArray") }
    , std::pair{ TypeCode::TextureCube, ConstantString("textureCube") }
    , std::pair{ TypeCode::TextureCubeArray, ConstantString("textureCubeArray") }
    , std::pair{ TypeCode::PixelCache, ConstantString("pixelCache") }
    , std::pair{ TypeCode::PixelCacheMS, ConstantString("pixelCacheMS") }
    , std::pair{ TypeCode::Sampler, ConstantString("sampler") }
};

const ConstantString NoCode = "";

//------------------------------------------------------------------------------
/**
*/
const ConstantString&
Type::CodeToString(const TypeCode& code)
{
    auto it = codeToStringMapping.Find(code);
    if (it == codeToStringMapping.end())
        return NoCode;
    else
        return it->second;
}

const StaticMap categoryToStringMapping =
std::array{
    std::pair{ Type::Category::TextureCategory, ConstantString("Texture") }
    , std::pair{ Type::Category::PixelCacheCategory, ConstantString("PixelCache") }
    , std::pair{ Type::Category::ScalarCategory, ConstantString("Scalar") }
    , std::pair{ Type::Category::UserTypeCategory, ConstantString("Buffer") }
    , std::pair{ Type::Category::EnumCategory, ConstantString("Enum") }
    , std::pair{ Type::Category::VoidCategory, ConstantString("Void") }
    , std::pair{ Type::Category::SamplerCategory, ConstantString("Sampler") }
    , std::pair{ Type::Category::AccelerationStructureCategory, ConstantString("AccelerationStructure") }
};

//------------------------------------------------------------------------------
/**
*/
ConstantString
Type::CategoryToString(const Category& cat)
{
    auto it = categoryToStringMapping.Find(cat);
    if (it == categoryToStringMapping.end())
        return "";
    else
        return it->second;
}

//------------------------------------------------------------------------------
/**
*/
Symbol* 
Type::GetSymbol(const FixedString& str)
{
    auto it = this->scope.symbolLookup.Find(str);
    if (it != this->scope.symbolLookup.end())
        return it->second;
    else
        return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*> 
Type::GetSymbols(const FixedString& str)
{
    std::vector<Symbol*> ret;
    auto range = this->scope.symbolLookup.FindRange(str);
    for (auto it = range.first; it != range.second; it++)
        ret.push_back((*it).second);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
Symbol*
Type::GetSymbol(const TransientString& str)
{
    auto it = this->scope.symbolLookup.Find(str);
    if (it != this->scope.symbolLookup.end())
        return it->second;
    else
        return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Symbol*>
Type::GetSymbols(const TransientString& str)
{
    std::vector<Symbol*> ret;
    auto range = this->scope.symbolLookup.FindRange(str);
    for (auto it = range.first; it != range.second; it++)
        ret.push_back((*it).second);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
StaticArray<Symbol*> DefaultTypes;
void
Type::SetupDefaultTypes()
{
    SYMBOL_STATIC_ALLOC = true;
    __BEGIN_TYPES__

    __MAKE_TYPE_CUSTOM(i32, GPULang::Int);
    __MAKE_TYPE_CUSTOM(i32x2, GPULang::Int2);
    __MAKE_TYPE_CUSTOM(i32x3, GPULang::Int3);
    __MAKE_TYPE_CUSTOM(i32x4, GPULang::Int4);
    __MAKE_TYPE_CUSTOM(u32, GPULang::UInt);
    __MAKE_TYPE_CUSTOM(u32x2, GPULang::UInt2);
    __MAKE_TYPE_CUSTOM(u32x3, GPULang::UInt3);
    __MAKE_TYPE_CUSTOM(u32x4, GPULang::UInt4);
    __MAKE_TYPE_CUSTOM(b8, GPULang::Bool);
    __MAKE_TYPE_CUSTOM(b8x2, GPULang::Bool2);
    __MAKE_TYPE_CUSTOM(b8x3, GPULang::Bool3);
    __MAKE_TYPE_CUSTOM(b8x4, GPULang::Bool4);

    __MAKE_TYPE_CUSTOM(f32, GPULang::Float);
    __MAKE_TYPE_CUSTOM(f32x2, GPULang::Float2);
    __MAKE_TYPE_CUSTOM(f32x3, GPULang::Float3);
    __MAKE_TYPE_CUSTOM(f32x4, GPULang::Float4);
    __MAKE_TYPE_CUSTOM(f32x2x2, GPULang::Mat2x2);
    __MAKE_TYPE_CUSTOM(f32x2x3, GPULang::Mat2x3);
    __MAKE_TYPE_CUSTOM(f32x2x4, GPULang::Mat2x4);
    __MAKE_TYPE_CUSTOM(f32x3x2, GPULang::Mat3x2);
    __MAKE_TYPE_CUSTOM(f32x3x3, GPULang::Mat3x3);
    __MAKE_TYPE_CUSTOM(f32x3x4, GPULang::Mat3x4);
    __MAKE_TYPE_CUSTOM(f32x4x2, GPULang::Mat4x2);
    __MAKE_TYPE_CUSTOM(f32x4x3, GPULang::Mat4x3);
    __MAKE_TYPE_CUSTOM(f32x4x4, GPULang::Mat4x4);

    __MAKE_TYPE_CUSTOM(texture1D, GPULang::Texture1D);
    __MAKE_TYPE_CUSTOM(texture1DArray, GPULang::Texture1DArray);
    __MAKE_TYPE_CUSTOM(texture2D, GPULang::Texture2D);
    __MAKE_TYPE_CUSTOM(texture2DArray, GPULang::Texture2DArray);
    __MAKE_TYPE_CUSTOM(texture2DMS, GPULang::Texture2DMS);
    __MAKE_TYPE_CUSTOM(texture2DMSArray, GPULang::Texture2DMSArray);
    __MAKE_TYPE_CUSTOM(texture3D, GPULang::Texture3D);
    __MAKE_TYPE_CUSTOM(textureCube, GPULang::TextureCube);
    __MAKE_TYPE_CUSTOM(textureCubeArray, GPULang::TextureCubeArray);
    __MAKE_TYPE_CUSTOM(pixelCache, GPULang::PixelCacheType);
    __MAKE_TYPE_CUSTOM(pixelCacheMS, GPULang::PixelCacheMS);

    __MAKE_TYPE_CUSTOM(accelerationStructure, GPULang::AccelerationStructureType);

#define __ADD_ENUM(val) labels.Append(#val); values.Append(nullptr);
#define __FINISH_ENUM(enum) enum.labels = StaticArray<FixedString>(labels); enum.values = StaticArray<Expression*>(values); labels.size = 0; values.size = 0;
    
    types.Append(&CompareModeTypeType);
    types.Append(&StencilOpTypeType);

    __MAKE_TYPE_CUSTOM(function, GPULang::FunctionType);
    __MAKE_TYPE_CUSTOM(stencilState, GPULang::StencilStateType);
    __MAKE_TYPE_CUSTOM(renderState, GPULang::RenderStateType);
    __MAKE_TYPE_CUSTOM(samplerState, GPULang::SamplerStateType);
    __MAKE_TYPE_CUSTOM(program, GPULang::ProgramType);

    __MAKE_TYPE_CUSTOM(sampler, GPULang::SamplerType);
    types.Append(&ExecutionScopeTypeType);
    types.Append(&MemorySemanticsTypeType);

    __MAKE_TYPE_CUSTOM(void, Void);
    DefaultTypes = StaticArray(types);
    SYMBOL_STATIC_ALLOC = false;
}

const StaticMap singleComponentToVectorMap =
std::array{
    std::pair{ TypeCode::Float,      StaticArray{ ConstantString("f32"),    ConstantString("f32x2"),    ConstantString("f32x3"),    ConstantString("f32x4") } }
    , std::pair{ TypeCode::Float16,  StaticArray{ ConstantString("f16"),    ConstantString("f16x2"),    ConstantString("f16x3"),    ConstantString("f16x4") } }
    , std::pair{ TypeCode::Int,      StaticArray{ ConstantString("i32"),    ConstantString("i32x2"),    ConstantString("i32x3"),    ConstantString("i32x4") } }
    , std::pair{ TypeCode::Int16,    StaticArray{ ConstantString("i16"),    ConstantString("i16x2"),    ConstantString("i16x3"),    ConstantString("i16x4") } }
    , std::pair{ TypeCode::UInt,     StaticArray{ ConstantString("u32"),    ConstantString("u32x2"),    ConstantString("u32x3"),    ConstantString("u32x4") } }
    , std::pair{ TypeCode::UInt16,   StaticArray{ ConstantString("u16"),    ConstantString("u16x2"),    ConstantString("u16x3"),    ConstantString("u16x4") } }
    , std::pair{ TypeCode::Bool,     StaticArray{ ConstantString("b8"),     ConstantString("b8x2"),     ConstantString("b8x3"),     ConstantString("b8x4")  } }

};

//------------------------------------------------------------------------------
/**
*/
const ConstantString
Type::ToVector(const TypeCode baseType, unsigned members)
{
    if (members > 4)
        return "";
    else
        return singleComponentToVectorMap.Find(baseType)->second[members - 1];
}

//------------------------------------------------------------------------------
/**
*/
const bool
Type::IsVector() const
{
    return this->category == Type::ScalarCategory && this->columnSize > 1 && this->rowSize <= 1;
}

//------------------------------------------------------------------------------
/**
*/
const bool 
Type::IsMatrix() const
{
    return this->category == Type::ScalarCategory && this->columnSize > 1 && this->rowSize > 1;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
Type::CalculateSize() const
{
    return this->byteSize;
}

//------------------------------------------------------------------------------
/**
    Rounds up to next power of 2
*/
forceinline unsigned int
roundtopow2(unsigned int val)
{
    val--;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val++;
    return val;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
Type::CalculateAlignment() const
{
    uint32_t baseAlignment = (this->byteSize / this->rowSize) / this->columnSize;
    uint32_t roundedColumns = roundtopow2(this->columnSize);
    return baseAlignment * roundedColumns;// * this->rowSize;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
Type::CalculateStride() const
{
    uint32_t baseAlignment = (this->byteSize / this->rowSize) / this->columnSize;
    uint32_t roundedColumns = roundtopow2(this->columnSize);
    return baseAlignment * roundedColumns * this->rowSize;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t 
Type::Align(uint32_t alignant, uint32_t alignment)
{
    return (alignant + alignment - 1) & ~(alignment - 1);
}

//------------------------------------------------------------------------------
/**
*/
bool 
Type::StringToSwizzleMask(const FixedString& str, SwizzleMask& out)
{
    // convert to swizzle mask
    out.bits.x = SwizzleMask::Invalid;
    out.bits.y = SwizzleMask::Invalid;
    out.bits.z = SwizzleMask::Invalid;
    out.bits.w = SwizzleMask::Invalid;
    for (size_t i = 0; i < str.len && i <= 4; i++)
    {
        char c = str.buf[i];
        switch (c)
        {
            case 'x':
            case 'r':
                switch (i)
                {
                    case 0:
                        out.bits.x = SwizzleMask::X;
                        break;
                    case 1:
                        out.bits.y = SwizzleMask::X;
                        break;
                    case 2:
                        out.bits.z = SwizzleMask::X;
                        break;
                    case 3:
                        out.bits.w = SwizzleMask::X;
                        break;
                }
                break;
            case 'y':
            case 'g':
                switch (i)
                {
                    case 0:
                        out.bits.x = SwizzleMask::Y;
                        break;
                    case 1:
                        out.bits.y = SwizzleMask::Y;
                        break;
                    case 2:
                        out.bits.z = SwizzleMask::Y;
                        break;
                    case 3:
                        out.bits.w = SwizzleMask::Y;
                        break;
                }
                break;
            case 'z':
            case 'b':
                switch (i)
                {
                    case 0:
                        out.bits.x = SwizzleMask::Z;
                        break;
                    case 1:
                        out.bits.y = SwizzleMask::Z;
                        break;
                    case 2:
                        out.bits.z = SwizzleMask::Z;
                        break;
                    case 3:
                        out.bits.w = SwizzleMask::Z;
                        break;
                }
                break;
            case 'w':
            case 'a':
                switch (i)
                {
                    case 0:
                        out.bits.x = SwizzleMask::W;
                        break;
                    case 1:
                        out.bits.y = SwizzleMask::W;
                        break;
                    case 2:
                        out.bits.z = SwizzleMask::W;
                        break;
                    case 3:
                        out.bits.w = SwizzleMask::W;
                        break;
                }
                break;
            default:
                return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
unsigned 
Type::SwizzleMaskComponents(SwizzleMask mask)
{
    int numComponents = 0;
    if (mask.bits.x != SwizzleMask::Invalid)
        numComponents++;
    if (mask.bits.y != SwizzleMask::Invalid)
        numComponents++;
    if (mask.bits.z != SwizzleMask::Invalid)
        numComponents++;
    if (mask.bits.w != SwizzleMask::Invalid)
        numComponents++;
    return numComponents;
}

//------------------------------------------------------------------------------
/**
*/
unsigned
Type::SwizzleMaskBiggestComponent(SwizzleMask mask)
{
    unsigned ret = 0;

    // Find the max value, but make sure to mask out Invalid
    ret = max(mask.bits.x & ~SwizzleMask::Invalid, ret);
    ret = max(mask.bits.y & ~SwizzleMask::Invalid, ret);
    ret = max(mask.bits.z & ~SwizzleMask::Invalid, ret);
    ret = max(mask.bits.w & ~SwizzleMask::Invalid, ret);
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
TransientString
Type::FullType::ToString(bool includeLiteral) const
{
    TransientString base;
    for (size_t i = 0; i < this->modifiers.size(); i++)
    {
        if (this->modifiers[i] == Modifier::Pointer)
        {
            base.Append("*");
        }
        else if (this->modifiers[i] == Modifier::Array)
        {
            if (this->modifierValues[i] == nullptr)
                base.Append("[]");
            else
            {
                uint32_t size;
                ValueUnion value;
                this->modifierValues[i]->EvalValue(value);
                value.Store(size);
                base.Concatenate<false>("[", size, "]");
            }
        }
    }
    if (this->literal && includeLiteral)
        base.Append("literal ");
    if (this->mut)
        base.Append("mutable ");
    if (this->sampled)
        base.Append("sampled ");
    
    if (this->swizzleName.len > 0)
        return TransientString(base, this->swizzleName);
    else
        return TransientString(base, this->name);
}

//------------------------------------------------------------------------------
/**
*/
bool
Type::FullType::Assignable(const Type::FullType& rhs) const
{
    if (this->literal)
        return false;
    if (this->sampled)
        return false;
    if (this->modifiers != rhs.modifiers)
        return false;
    for (size_t i = 0; i < this->modifierValues.size(); i++)
        if (this->modifierValues[i] != rhs.modifierValues[i])
        {
            uint32_t lhsSize = UINT32_MAX, rhsSize;
            if (this->modifierValues[i] != nullptr)
            {
                ValueUnion value;
                this->modifierValues[i]->EvalValue(value);
                value.Store(lhsSize);
            }
            if (rhs.modifierValues[i] != nullptr)
            {
                ValueUnion value;
                rhs.modifierValues[i]->EvalValue(value);
                value.Store(rhsSize);
            }

            if (lhsSize != rhsSize)
                return false;
        }
        else
            return false;
    FixedString lhsName = this->swizzleName.len == 0 ? this->name : this->swizzleName;
    FixedString rhsName = rhs.swizzleName.len == 0 ? rhs.name : rhs.swizzleName;
    if (lhsName != rhsName)
        return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
Type::FullType::Constructible(const FullType& rhs) const
{
    if (this->literal)
        return false;
    if (this->sampled)
        return false;
    if (this->modifiers != rhs.modifiers)
        return false;
    for (size_t i = 0; i < this->modifierValues.size(); i++)
        if (this->modifierValues[i] != rhs.modifierValues[i])
        {
            uint32_t lhsSize = UINT32_MAX, rhsSize;
            // If we are constructing an unbound array to a bound sized array, that's fine
            if (this->modifierValues[i] == nullptr && rhs.modifierValues[i] != nullptr)
                continue;
            else
            {
                if (this->modifierValues[i] != nullptr)
                {
                    ValueUnion value;
                    this->modifierValues[i]->EvalValue(value);
                    value.Store(lhsSize);
                }
                if (rhs.modifierValues[i] != nullptr)
                {
                    ValueUnion value;
                    rhs.modifierValues[i]->EvalValue(value);
                    value.Store(rhsSize);
                }

                if (lhsSize != rhsSize)
                    return false;    
            }
        }
        else
            return false;
    FixedString lhsName = this->swizzleName.len == 0 ? this->name : this->swizzleName;
    FixedString rhsName = rhs.swizzleName.len == 0 ? rhs.name : rhs.swizzleName;
    if (lhsName != rhsName)
        return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
Type::FullType::operator==(const FullType& rhs) const
{
    if (this->literal && !rhs.literal)
        return false;
    if (this->mut && !rhs.mut)
        return false;
    if (this->sampled != rhs.sampled)
        return false;
    if (this->modifiers != rhs.modifiers)
        return false;
    for (size_t i = 0; i < this->modifierValues.size(); i++)
        if (this->modifierValues[i] != rhs.modifierValues[i])
        {
            uint32_t lhsSize, rhsSize;
            if (this->modifierValues[i] != nullptr)
            {
                ValueUnion value;
                this->modifierValues[i]->EvalValue(value);
                value.Store(lhsSize);
            }
            if (rhs.modifierValues[i] != nullptr)
            {
                ValueUnion value;
                rhs.modifierValues[i]->EvalValue(value);
                value.Store(rhsSize);
            }
            if (lhsSize != rhsSize)
                return false;
        }
    FixedString lhsName = this->swizzleName.len == 0 ? this->name : this->swizzleName;
    FixedString rhsName = rhs.swizzleName.len == 0 ? rhs.name : rhs.swizzleName;
    if (lhsName != rhsName)
        return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
const bool 
Type::FullType::IsPointer() const
{
    auto it = this->modifiers.crbegin();
    while (it != this->modifiers.crend())
    {
        if (*it == Type::FullType::Modifier::Pointer)
            return true;
        else if (*it == Type::FullType::Modifier::Array)
            return false;
        it++;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
const bool 
Type::FullType::IsMutable() const
{
    return this->mut;
}

//------------------------------------------------------------------------------
/**
*/
const Type::FullType::Modifier 
Type::FullType::LastIndirectionModifier() const
{
    auto it = this->modifiers.crbegin();
    while (it != this->modifiers.crend())
    {
        switch (*it)
        {
            case Type::FullType::Modifier::Array:
            case Type::FullType::Modifier::Pointer:
                return *it;
        }
        it++;
    }
    return Type::FullType::Modifier::Invalid;
}

} // namespace GPULang
