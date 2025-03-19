#pragma once
//------------------------------------------------------------------------------
/**
    List of intrinsic functions

    @copyright
    (C) 2024 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "function.h"

#define TEXTURE_INTRINSIC_ALL_LIST\
    X(texture1D, 0)\
    X(texture2D, 1)\
    X(texture2DMS, 2)\
    X(texture3D, 3)\
    X(textureCube, 4)\
    X(texture1DArray, 5)\
    X(texture2DArray, 6)\
    X(texture2DMSArray, 7)\
    X(textureCubeArray, 8)

#define TEXTURE_INTRINSIC_NO_MS_LIST\
    X(texture1D, 0)\
    X(texture2D, 1)\
    X(texture3D, 3)\
    X(textureCube, 4)\
    X(texture1DArray, 5)\
    X(texture2DArray, 6)\
    X(textureCubeArray, 8)

#define TEXTURE_INTRINSIC_NO_CUBE_LIST\
    X(texture1D, 0)\
    X(texture2D, 1)\
    X(texture2DMS, 2)\
    X(texture3D, 3)\
    X(texture1DArray, 5)\
    X(texture2DArray, 6)\
    X(texture2DMSArray, 7)

#define TEXTURE_INTRINSIC_NO_CUBE_MS_LIST\
    X(texture1D, 0)\
    X(texture2D, 1)\
    X(texture3D, 3)\
    X(texture1DArray, 5)\
    X(texture2DArray, 6)

#define TEXTURE_INTRINSIC_PLAIN_LIST\
    X(texture1D, 0)\
    X(texture2D, 1)\
    X(texture3D, 3)\
    X(textureCube, 4)

#define TEXTURE_INTRINSIC_GATHER_LIST\
    X(texture2D, 1)\
    X(textureCube, 4)\
    X(texture2DArray, 6)\
    X(textureCubeArray, 8)

#define TEXTURE_INTRINSIC_2D_LIST\
    X(texture2D, 1)\
    X(texture2DArray, 6)

#define TEXTURE_INTRINSIC_ONLY_MS_LIST\
    X(texture2DMS, 2)\
    X(texture2DMSArray, 7)\

#define DECLARE_TEXTURE_INTRINSIC(variant, ty) \
    extern Function* Texture##variant##_##ty; \
    extern Function* SampledTexture##variant##_##ty;

#define DEFINE_TEXTURE_INTRINSIC(variant, ty) \
    Function* Texture##variant##_##ty; \
    Function* SampledTexture##variant##_##ty;

#define SCALAR_LIST\
    X(f32, 0)\
    X(f32x2, 1)\
    X(f32x3, 2)\
    X(f32x4, 3)\
    X(i32, 4)\
    X(i32x2, 5)\
    X(i32x3, 6)\
    X(i32x4, 7)\
    X(u32, 8)\
    X(u32x2, 9)\
    X(u32x3, 10)\
    X(u32x4, 11)

#define FLOAT_LIST\
    X(f32, 0)\
    X(f32x2, 1)\
    X(f32x3, 2)\
    X(f32x4, 3)

#define FLOAT_VEC_LIST\
    X(f32x2, 1)\
    X(f32x3, 2)\
    X(f32x4, 3)

#define SIGN_LIST\
    X(f32, 0)\
    X(f32x2, 1)\
    X(f32x3, 2)\
    X(f32x4, 3)\
    X(i32, 4)\
    X(i32x2, 5)\
    X(i32x3, 6)\
    X(i32x4, 7)

#define INT_LIST\
    X(i32, 4)\
    X(i32x2, 5)\
    X(i32x3, 6)\
    X(i32x4, 7)\
    X(u32, 8)\
    X(u32x2, 9)\
    X(u32x3, 10)\
    X(u32x4, 11)

#define INT_SINGLE_LIST\
    X(i32, 4)\
    X(u32, 8)

#define SINGLE_COMPONENT_LIST\
    X(f32, 0)\
    X(i32, 4)\
    X(u32, 8)

#define BOOL_LIST\
    X(b8, 12)\
    X(b8x2, 13)\
    X(b8x3, 14)\
    X(b8x4, 15)

#define MATRIX_LIST\
    X(f32x2x2, 16)\
    X(f32x2x3, 17)\
    X(f32x2x4, 18)\
    X(f32x3x2, 19)\
    X(f32x3x3, 20)\
    X(f32x3x4, 21)\
    X(f32x4x2, 22)\
    X(f32x4x3, 23)\
    X(f32x4x4, 24)

#define DECLARE_SCALAR_INTRINSIC(fun, ty) extern Function* fun##_##ty;
#define DEFINE_SCALAR_INTRINSIC(fun, ty) Function* fun##_##ty;
    

namespace GPULang
{

namespace Intrinsics {


#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Pow, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Sqrt, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(InvSqrt, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Log, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Log2, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Exp, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Exp2, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Sin, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Cos, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Tan, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(ASin, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(ACos, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(ATan, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(ATan2, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(SinH, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(CosH, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(TanH, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(ASinH, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(ACosH, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(ATanH, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Mad, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Dot, ty)
    FLOAT_VEC_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Reflect, ty)
    FLOAT_VEC_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Refract, ty)
    FLOAT_VEC_LIST
#undef X

extern Function* Cross_f32x3;

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Normalize, ty)
    FLOAT_VEC_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Length, ty)
    FLOAT_VEC_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Distance, ty)
    FLOAT_VEC_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Min, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Max, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Clamp, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Lerp, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Step, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(SmoothStep, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Ceil, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Floor, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Fract, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Saturate, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Truncate, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Abs, ty)
    SIGN_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Sign, ty)
    SIGN_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(DDX, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(DDY, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(FWidth, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(CastToU32, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(CastToI32, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(CastToF32, ty)
    INT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Any, ty)
    BOOL_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(All, ty)
    BOOL_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Inverse, ty)
    MATRIX_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Transpose, ty)
    MATRIX_LIST
#undef X

//------------------------------------------------------------------------------
/**
    Builtins, like:
    gl_Position, gl_Layer, etc.

    In GPULang these are Get/Set functions to make it clear which ones are modifiable in the shader
*/
//------------------------------------------------------------------------------
extern Function* SetOutputLayer;
extern Function* GetOutputLayer;
extern Function* SetOutputViewport;
extern Function* GetOutputViewport;
extern Function* ExportVertexCoordinates;
extern Function* GetVertexIndex;
extern Function* GetInstanceIndex;

extern Function* GetBaseVertexIndex;
extern Function* GetBaseInstanceIndex;
extern Function* GetDrawIndex;

extern Function* GetPixelCoordinates;
extern Function* SetPixelDepth;
extern Function* GetPixelDepth;

extern Function* ExportVertex;
extern Function* ExportPrimitive;

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(ExportColor, ty)
    SCALAR_LIST
#undef X

extern Function* GetLocalInvocationIndex;
extern Function* GetGlobalInvocationIndex;
extern Function* GetWorkGroupIndex;
extern Function* GetWorkGroupDimensions;

extern Function* GetSubgroupId;
extern Function* GetSubgroupSize;
extern Function* GetNumSubgroups;
extern Function* GetSubgroupLocalInvocationMask;
extern Function* GetSubgroupLocalInvocationAndLowerMask;
extern Function* GetSubgroupLowerMask;
extern Function* GetSubgroupLocalInvocationAndGreaterMask;
extern Function* GetSubgroupGreaterMask;

extern Function* SubgroupFirstInvocation;
extern Function* SubgroupRead;
extern Function* SubgroupBallot;
extern Function* SubgroupInverseBallot;
extern Function* SubgroupBallotBitCount;
extern Function* SubgroupBallotBit;
extern Function* SubgroupBallotFirstOne;
extern Function* SubgroupBallotLastOne;

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(SubgroupSwapDiagonal, ty)
    SCALAR_LIST
#undef X
#define X(ty, index) DECLARE_SCALAR_INTRINSIC(SubgroupSwapVertical, ty)
    SCALAR_LIST
#undef X
#define X(ty, index) DECLARE_SCALAR_INTRINSIC(SubgroupSwapHorizontal, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicLoad, ty)
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicStore, ty)
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicExchange, ty)
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicCompareExchange, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicIncrement, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicDecrement, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicAdd, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicSub, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicMin, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicMax, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicAnd, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicOr, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(AtomicXor, ty)
    INT_SINGLE_LIST
#undef X

extern Function* BitInsert;
extern Function* BitSExtract;
extern Function* BitUExtract;
extern Function* BitReverse;
extern Function* BitCount;

extern Function* ExecutionBarrier;
extern Function* ExecutionBarrierSubgroup;
extern Function* ExecutionBarrierWorkgroup;
extern Function* MemoryExecutionBarrier;
extern Function* MemoryExecutionBarrierSubgroup;
extern Function* MemoryExecutionBarrierWorkgroup;

extern Function* PixelCacheLoad;
extern Function* PixelCacheLoadMS;

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(GetSize, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(GetSizeMip, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(GetSampledMip, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(GetMips, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(GetSamples, ty)
    TEXTURE_INTRINSIC_ONLY_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(LoadBase, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(LoadMip, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(StoreBase, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(StoreMip, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(FetchBase, ty)
    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(FetchSample, ty)
    TEXTURE_INTRINSIC_ONLY_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(GatherBase, ty)
    TEXTURE_INTRINSIC_GATHER_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(GatherOffsets, ty)
    TEXTURE_INTRINSIC_2D_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleBase, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleOffset, ty)
    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleBias, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleBiasCompare, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleBiasOffset, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleBiasProj, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleBiasProjCompare, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleBiasProjOffset, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleCompare, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleCompareOffset, ty)
    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleGrad, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleGradCompare, ty)
    TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleGradOffset, ty)
    TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleGradProj, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleGradProjCompare, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleGradProjCompareOffset, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleGradProjOffset, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleLod, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleLodCompare, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleLodCompareOffset, ty)
     TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleLodOffset, ty)
    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleLodProj, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleLodProjCompare, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleLodProjCompareOffset, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleLodProjOffset, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleProj, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleProjCompare, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleProjCompareOffset, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DECLARE_TEXTURE_INTRINSIC(SampleProjOffset, ty)
    TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

extern Function* TraceRay;
extern Function* ExportRayIntersection;
extern Function* ExecuteCallable;

extern Function* RayLaunchIndex;
extern Function* RayLaunchSize;
extern Function* BLASPrimitiveIndex;
extern Function* BLASGeometryIndex;
extern Function* TLASInstanceIndex;
extern Function* TLASInstanceCustomIndex;
extern Function* RayWorldOrigin;
extern Function* RayWorldDirection;
extern Function* RayObjectOrigin;
extern Function* RayObjectDirection;
extern Function* RayMin;
extern Function* RayMax;
extern Function* RayFlags;
extern Function* RayHitDistance;
extern Function* RayHitKind;
extern Function* TLASObjectToWorld;
extern Function* TLASWorldToObject;


}

} // namespace GPULang
