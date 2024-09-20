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

#define DECLARE_TEXTURE_INTRINSIC(variant, ty) extern Function* Texture##variant##_##ty;
#define DEFINE_TEXTURE_INTRINSIC(variant, ty) Function* Texture##variant##_##ty;

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

#define INT_LIST\
    X(i32, 4)\
    X(i32x2, 5)\
    X(i32x3, 6)\
    X(i32x4, 7)\
    X(u32, 8)\
    X(u32x2, 9)\
    X(u32x3, 10)\
    X(u32x4, 11)

#define BOOL_LIST\
    X(b8, 12)\
    X(b8x2, 13)\
    X(b8x3, 14)\
    X(b8x4, 15)

#define DECLARE_SCALAR_INTRINSIC(fun, ty) extern Function* fun##_##ty;
#define DEFINE_SCALAR_INTRINSIC(fun, ty) Function* fun##_##ty;
    

namespace GPULang
{

namespace Intrinsics {

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Mad, ty)
    SCALAR_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(Dot, ty)
    FLOAT_LIST
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

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(DDX, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(DDY, ty)
    FLOAT_LIST
#undef X

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(FWidth, ty)
    FLOAT_LIST
#undef X

extern Function* SubgroupId_;
extern Function* SubgroupFirstActive_;
extern Function* SubgroupFirst_;
extern Function* SubgroupBallot_;
extern Function* SubgroupInverseBallot_;
extern Function* SubgroupBallotBitCount_;
extern Function* SubgroupBallotBit_;

#define X(ty, index) DECLARE_SCALAR_INTRINSIC(SubgroupSwapDiagonal, ty)
    SCALAR_LIST
#undef X
#define X(ty, index) DECLARE_SCALAR_INTRINSIC(SubgroupSwapVertical, ty)
    SCALAR_LIST
#undef X
#define X(ty, index) DECLARE_SCALAR_INTRINSIC(SubgroupSwapHorizontal, ty)
    SCALAR_LIST
#undef X

extern Function* PixelCacheLoad_;
extern Function* PixelCacheLoadMS_;

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
}

} // namespace GPULang
