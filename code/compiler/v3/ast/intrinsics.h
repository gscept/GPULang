#pragma once
//------------------------------------------------------------------------------
/**
    List of intrinsic functions

    @copyright
    (C) 2024 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "function.h"

namespace GPULang
{

namespace Intrinsics {

extern Function* Mad;
extern Function* Dot;
extern Function* Min;
extern Function* Max;
extern Function* Clamp;
extern Function* Ceil;
extern Function* Floor;
extern Function* Frac;
extern Function* Saturate;
extern Function* Truncate;
extern Function* DDX;
extern Function* DDY;
extern Function* FWidth;
extern Function* SubgroupId;
extern Function* SubgroupFirstActive;
extern Function* SubgroupFirst;
extern Function* SubgroupBallot;
extern Function* SubgroupInverseBallot;
extern Function* SubgroupBallotBitCount;
extern Function* SubgroupBallotBit;
extern Function* SubgroupSwapDiagonal;
extern Function* SubgroupSwapVertical;
extern Function* SubgroupSwapHorizontal;
extern Function* TextureLoad;
extern Function* TextureLoadLod;
extern Function* TextureStore;
extern Function* TextureStoreLod;
extern Function* PixelCacheLoad;
extern Function* TextureSample;
extern Function* TextureSampleOffset;
extern Function* TextureSampleBias;
extern Function* TextureSampleBiasCompare;
extern Function* TextureSampleBiasOffset;
extern Function* TextureSampleBiasProj;
extern Function* TextureSampleBiasProjCompare;
extern Function* TextureSampleBiasProjOffset;
extern Function* TextureSampleCompare;
extern Function* TextureSampleCompareOffset;
extern Function* TextureSampleGrad;
extern Function* TextureSampleGradCompare;
extern Function* TextureSampleGradCompareOffset;
extern Function* TextureSampleGradOffset;
extern Function* TextureSampleGradProj;
extern Function* TextureSampleGradProjCompare;
extern Function* TextureSampleGradProjCompareOffset;
extern Function* TextureSampleGradProjOffset;
extern Function* TextureSampleLod;
extern Function* TextureSampleLodCompare;
extern Function* TextureSampleLodCompareOffset;
extern Function* TextureSampleLodOffset;
extern Function* TextureSampleLodProj;
extern Function* TextureSampleLodProjCompare;
extern Function* TextureSampleLodProjCompareOffset;
extern Function* TextureSampleLodProjOffset;
extern Function* TextureSampleProj;
extern Function* TextureSampleProjCompare;
extern Function* TextureSampleProjCompareOffset;
extern Function* TextureSampleProjOffset;

}

} // namespace GPULang
