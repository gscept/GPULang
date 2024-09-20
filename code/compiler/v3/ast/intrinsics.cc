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

Function* Mad;
Function* Dot;
Function* Min;
Function* Max;
Function* Clamp;
Function* Ceil;
Function* Floor;
Function* Frac;
Function* Saturate;
Function* Truncate;
Function* DDX;
Function* DDY;
Function* FWidth;
Function* SubgroupId;
Function* SubgroupFirstActive;
Function* SubgroupFirst;
Function* SubgroupBallot;
Function* SubgroupInverseBallot;
Function* SubgroupBallotBitCount;
Function* SubgroupBallotBit;
Function* SubgroupSwapDiagonal;
Function* SubgroupSwapVertical;
Function* SubgroupSwapHorizontal;
Function* PixelCacheLoad;
Function* PixelCacheLoadMS;

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

/*
DEFINE_TEXTURE_SAMPLE_INTRINSIC(Base);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(Offset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(Bias);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(BiasCompare);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(BiasOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(BiasProj);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(BiasProjCompare);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(BiasProjOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(Compare);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(CompareOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(Grad);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(GradCompare);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(GradCompareOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(GradOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(GradProj);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(GradProjCompareOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(GradProjOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(Lod);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(LodCompare);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(LodCompareOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(LodOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(LodProj);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(LodProjCompare);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(LodProjCompareOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(LodProjOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(Proj);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(ProjCompare);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(ProjCompareOffset);
DEFINE_TEXTURE_SAMPLE_INTRINSIC(ProjOffset);
*/
}

} // namespace GPULang
