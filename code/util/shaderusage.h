#pragma once
//------------------------------------------------------------------------------
/**
    Shader usage defines

    @copyright
    (C) 2024 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <cstdint>
namespace GPULang
{

union ShaderUsage
{
    struct
    {
        uint32_t vertexShader : 1;
        uint32_t hullShader : 1;
        uint32_t domainShader : 1;
        uint32_t geometryShader : 1;
        uint32_t pixelShader : 1;
        uint32_t computeShader : 1;
        uint32_t taskShader : 1;
        uint32_t meshShader : 1;
        uint32_t rayGenerationShader : 1;
        uint32_t rayMissShader : 1;
        uint32_t rayClosestHitShader : 1;
        uint32_t rayAnyHitShader : 1;
        uint32_t rayIntersectionShader : 1;
        uint32_t rayCallableShader : 1;

    } flags;
    uint32_t bits;

    ShaderUsage()
        : bits(0x0)
    {
        // empty
    }
};

} // namespace GPULang
