#pragma once
//------------------------------------------------------------------------------
/**
    List of intrinsic functions

    @copyright
    (C) 2024 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "intrinsics.h"
#include "function.h"

namespace GPULang
{

namespace Intrinsics {


#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Pow, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Sqrt, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(InvSqrt, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Log, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Log2, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Exp, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Exp2, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Sin, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Cos, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Tan, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(ASin, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(ACos, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(ATan, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Mad, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Dot, ty)
    FLOAT_VEC_LIST
#undef X

Function* Cross_f32x3;

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Normalize, ty)
    FLOAT_VEC_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Length, ty)
    FLOAT_VEC_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Min, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Max, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Clamp, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Lerp, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Step, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(SmoothStep, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Ceil, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Floor, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Fract, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Saturate, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Truncate, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Abs, ty)
    SIGN_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Sign, ty)
    SIGN_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(DDX, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(DDY, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(FWidth, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(CastToU32, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(CastToI32, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(CastToF32, ty)
    INT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(Any, ty)
    BOOL_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(All, ty)
    BOOL_LIST
#undef X
        
Function* SetOutputLayer;
Function* GetOutputLayer;
Function* SetOutputViewport;
Function* GetOutputViewport;
Function* ExportVertexCoordinates;
Function* GetVertexIndex;
Function* GetInstanceIndex;

Function* GetBaseVertexIndex;
Function* GetBaseInstanceIndex;
Function* GetDrawIndex;

Function* GetPixelCoordinates;
Function* SetPixelDepth;

Function* ExportVertex;
Function* ExportPrimitive;

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(ExportColor, ty)
    SCALAR_LIST
#undef X

Function* GetLocalInvocationIndex;
Function* GetGlobalInvocationIndex;
Function* GetWorkGroupIndex;
Function* GetWorkGroupDimensions;

Function* GetSubgroupId;
Function* GetSubgroupSize;
Function* GetNumSubgroups;
Function* GetSubgroupLocalInvocationMask;
Function* GetSubgroupLocalInvocationAndLowerMask;
Function* GetSubgroupLowerMask;
Function* GetSubgroupLocalInvocationAndGreaterMask;
Function* GetSubgroupGreaterMask;

Function* SubgroupFirstInvocation;
Function* SubgroupRead;
Function* SubgroupBallot;
Function* SubgroupInverseBallot;
Function* SubgroupBallotBitCount;
Function* SubgroupBallotBit;
Function* SubgroupBallotFirstOne;
Function* SubgroupBallotLastOne;

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicLoad, ty)
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicStore, ty)
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicExchange, ty)
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicCompareExchange, ty)
    SINGLE_COMPONENT_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicIncrement, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicDecrement, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicAdd, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicSub, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicMin, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicMax, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicAnd, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicOr, ty)
    INT_SINGLE_LIST
#undef X

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(AtomicXor, ty)
    INT_SINGLE_LIST
#undef X

Function* BitInsert;
Function* BitSExtract;
Function* BitUExtract;
Function* BitReverse;
Function* BitCount;

Function* ExecutionBarrier;
Function* ExecutionBarrierSubgroup;
Function* ExecutionBarrierWorkgroup;
Function* MemoryExecutionBarrier;
Function* MemoryExecutionBarrierSubgroup;
Function* MemoryExecutionBarrierWorkgroup;

#define X(ty, index) DEFINE_SCALAR_INTRINSIC(SubgroupSwapDiagonal, ty)
    SCALAR_LIST
#undef X
#define X(ty, index) DEFINE_SCALAR_INTRINSIC(SubgroupSwapVertical, ty)
    SCALAR_LIST
#undef X
#define X(ty, index) DEFINE_SCALAR_INTRINSIC(SubgroupSwapHorizontal, ty)
    SCALAR_LIST
#undef X

Function* PixelCacheLoad;
Function* PixelCacheLoadMS;

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(GetSize, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(GetSizeMip, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(GetSampledMip, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(GetMips, ty)
    TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(GetSamples, ty)
    TEXTURE_INTRINSIC_ONLY_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(LoadBase, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(LoadMip, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(StoreBase, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(StoreMip, ty)
    TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(FetchBase, ty)
    TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(FetchSample, ty)
    TEXTURE_INTRINSIC_ONLY_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(GatherBase, ty)
    TEXTURE_INTRINSIC_GATHER_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(GatherOffsets, ty)
    TEXTURE_INTRINSIC_2D_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleBase, ty)
TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleOffset, ty)
TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleBias, ty)
TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleBiasCompare, ty)
TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleBiasOffset, ty)
TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleBiasProj, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleBiasProjCompare, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleBiasProjOffset, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleCompare, ty)
TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleCompareOffset, ty)
TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleGrad, ty)
TEXTURE_INTRINSIC_NO_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleGradCompare, ty)
TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleGradOffset, ty)
TEXTURE_INTRINSIC_NO_CUBE_MS_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleGradProj, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleGradProjCompare, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleGradProjCompareOffset, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleGradProjOffset, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleLod, ty)
TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleLodCompare, ty)
TEXTURE_INTRINSIC_ALL_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleLodCompareOffset, ty)
TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleLodOffset, ty)
TEXTURE_INTRINSIC_NO_CUBE_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleLodProj, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleLodProjCompare, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleLodProjCompareOffset, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleLodProjOffset, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleProj, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleProjCompare, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleProjCompareOffset, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

#define X(ty, index) DEFINE_TEXTURE_INTRINSIC(SampleProjOffset, ty)
TEXTURE_INTRINSIC_PLAIN_LIST
#undef X

Function* TraceRay;
Function* ExportRayIntersection;
Function* ExecuteCallable;

Function* RayLaunchIndex;
Function* RayLaunchSize;
Function* BLASPrimitiveIndex;
Function* BLASGeometryIndex;
Function* TLASInstanceIndex;
Function* TLASInstanceCustomIndex;
Function* RayWorldOrigin;
Function* RayWorldDirection;
Function* RayObjectOrigin;
Function* RayObjectDirection;
Function* RayMin;
Function* RayMax;
Function* RayFlags;
Function* RayHitDistance;
Function* RayHitKind;
Function* TLASObjectToWorld;
Function* TLASWorldToObject;
}

} // namespace GPULang
