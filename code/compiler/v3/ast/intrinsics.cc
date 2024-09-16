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
Function* TextureLoad;
Function* TextureLoadLod;
Function* TextureStore;
Function* TextureStoreLod;
Function* PixelCacheLoad;
Function* TextureSample;
Function* TextureSampleOffset;
Function* TextureSampleBias;
Function* TextureSampleBiasCompare;
Function* TextureSampleBiasOffset;
Function* TextureSampleBiasProj;
Function* TextureSampleBiasProjCompare;
Function* TextureSampleBiasProjOffset;
Function* TextureSampleCompare;
Function* TextureSampleCompareOffset;
Function* TextureSampleGrad;
Function* TextureSampleGradCompare;
Function* TextureSampleGradCompareOffset;
Function* TextureSampleGradOffset;
Function* TextureSampleGradProj;
Function* TextureSampleGradProjCompare;
Function* TextureSampleGradProjCompareOffset;
Function* TextureSampleGradProjOffset;
Function* TextureSampleLod;
Function* TextureSampleLodCompare;
Function* TextureSampleLodCompareOffset;
Function* TextureSampleLodOffset;
Function* TextureSampleLodProj;
Function* TextureSampleLodProjCompare;
Function* TextureSampleLodProjCompareOffset;
Function* TextureSampleLodProjOffset;
Function* TextureSampleProj;
Function* TextureSampleProjCompare;
Function* TextureSampleProjCompareOffset;
Function* TextureSampleProjOffset;

}

} // namespace GPULang
