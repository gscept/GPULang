//------------------------------------------------------------------------------
//  @file pipelineassembly.cc
//  @copyright (C) 2024 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "pipelineassembly.h"
#include <assert.h>

#if __WIN32__
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WIN32_WINNT 0x0601
#include <windows.h>
#endif

namespace GPULang
{


//------------------------------------------------------------------------------
/**
*/
uint32_t 
FirstOne(uint32_t mask)
{
#if __WIN32__
    DWORD count = 0;
    _BitScanReverse(&count, mask);
#else
    int count = 31 - __builtin_clz(size);
#endif
    return count;
}

//------------------------------------------------------------------------------
/**
*/
VulkanPipelineInfo 
SetupVulkan(const VkDevice device, const Deserialize::Program* prog, GPULang::Deserialize::Variable** variables, uint32_t numVariables, const VulkanFunctions& functionBindings)
{
    VulkanPipelineInfo ret;

    static const VkShaderStageFlagBits ShaderLookup[] =
    {
        VK_SHADER_STAGE_VERTEX_BIT,
        VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
        VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
        VK_SHADER_STAGE_GEOMETRY_BIT,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        VK_SHADER_STAGE_COMPUTE_BIT,
        VK_SHADER_STAGE_TASK_BIT_EXT,
        VK_SHADER_STAGE_MESH_BIT_EXT,
        VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
        VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
        VK_SHADER_STAGE_MISS_BIT_KHR,
        VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
        VK_SHADER_STAGE_CALLABLE_BIT_KHR,
    };
    ret.shaderCount = 0;
    for (uint32_t i = 0; i < Deserialize::Program::ShaderStages::NumShaders; i++)
    {
        if (prog->shaders[i].binary != nullptr)
        {
            VkShaderModuleCreateInfo shaderInfo =
            {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0x0,
                .codeSize = prog->shaders[i].binaryLength,
                .pCode = prog->shaders[i].binary
            };
            VkResult res = functionBindings.vkCreateShaderModule(device, &shaderInfo, nullptr, &ret.shaders[ret.shaderCount]);
            assert(res == VK_SUCCESS);
            ret.shaderInfos[ret.shaderCount] =
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0x0,
                .stage = ShaderLookup[i],
                .module = ret.shaders[ret.shaderCount],
                .pName = "main",
                .pSpecializationInfo = nullptr
            };
            ret.shaderCount++;
        }
    }

    static const VkDescriptorType descriptorTypeTable[] =
    {
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        VK_DESCRIPTOR_TYPE_SAMPLER
    };

    static const VkShaderStageFlagBits shaderStageTable[] =
    {
        VK_SHADER_STAGE_VERTEX_BIT,
        VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
        VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
        VK_SHADER_STAGE_GEOMETRY_BIT,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        VK_SHADER_STAGE_COMPUTE_BIT,
        VK_SHADER_STAGE_TASK_BIT_EXT,
        VK_SHADER_STAGE_MESH_BIT_EXT,
        VK_SHADER_STAGE_RAYGEN_BIT_KHR,
        VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
        VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
        VK_SHADER_STAGE_MISS_BIT_KHR,
        VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
        VK_SHADER_STAGE_CALLABLE_BIT_KHR,
    };

    memset(ret.groupBindings, 0x0, sizeof(ret.groupBindings));
    memset(ret.groupBindingCounter, 0x0, sizeof(ret.groupBindingCounter));

    ret.poolSizeCounter = 0;
    VkDescriptorPoolSize poolSizes[8] = {};

    //ret.groupBindingCounter = { 0 };
    for (uint32_t i = 0; i < numVariables; i++)
    {
        GPULang::Deserialize::Variable* var = variables[i];
        VkPipelineStageFlags accessBits = 0x0;
        uint32_t bit = 1;
        while (bit <= var->visibility.bits)
        {
            uint32_t mask = var->visibility.bits & bit;
            if (mask != 0x0)
                accessBits |= shaderStageTable[FirstOne(mask)];
            bit = bit << 1;
        }

        VkDescriptorPoolSize& pool = poolSizes[var->bindingType];
        pool.type = descriptorTypeTable[var->bindingType];
        pool.descriptorCount += 1;

        VkDescriptorSetLayoutBinding& binding = ret.groupBindings[var->group][ret.groupBindingCounter[var->group]++];
        binding.binding = var->binding;
        binding.descriptorCount = max(1u, var->arraySizes[var->arraySizeCount - 1]);
        binding.descriptorType = descriptorTypeTable[var->bindingType];
        
        binding.stageFlags = accessBits;
        binding.pImmutableSamplers = nullptr;
    }

    for (uint32_t i = 0; i < 8; i++)
    {
        ret.descriptorPoolSizes[i] =
        {
            .type = VK_DESCRIPTOR_TYPE_MAX_ENUM,
            .descriptorCount = 0
        };
        if (poolSizes[i].descriptorCount != 0)
            ret.descriptorPoolSizes[ret.poolSizeCounter++] = poolSizes[i];
    }

    memset(ret.groupLayouts, 0x0, sizeof(ret.groupLayouts));
    ret.groupLayoutCounter = 0;
    for (uint32_t i = 0; i < 8; i++)
    {
        if (ret.groupBindingCounter[i] > 0)
        {
            VkDescriptorSetLayoutCreateInfo layoutInfo =
            {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0x0,
                .bindingCount = ret.groupBindingCounter[i],
                .pBindings = ret.groupBindings[i]
            };
            functionBindings.vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &ret.groupLayouts[i]);
            ret.groupLayoutCounter = max(ret.groupLayoutCounter, i + 1);
        }
        else
        {
            ret.groupLayouts[i] = VK_NULL_HANDLE;
        }
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .setLayoutCount = ret.groupLayoutCounter,
        .pSetLayouts = ret.groupLayouts,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    };
    functionBindings.vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &ret.pipelineLayout);

    ret.tessellationInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .patchControlPoints = 0
    };

    static const VkPolygonMode polygonModeTable[] =
    {
        VK_POLYGON_MODE_MAX_ENUM,
        VK_POLYGON_MODE_FILL,
        VK_POLYGON_MODE_LINE,
        VK_POLYGON_MODE_POINT
    };

    static const uint32_t cullModeTable[] =
    {
        VK_CULL_MODE_FLAG_BITS_MAX_ENUM,
        VK_CULL_MODE_NONE,
        VK_CULL_MODE_FRONT_BIT,
        VK_CULL_MODE_BACK_BIT,
        VK_CULL_MODE_FRONT_AND_BACK
    };

    static const VkFrontFace frontFaceTable[] =
    {
        VK_FRONT_FACE_MAX_ENUM,
        VK_FRONT_FACE_CLOCKWISE,
        VK_FRONT_FACE_COUNTER_CLOCKWISE
    };

    ret.rasterizationInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .depthClampEnable = prog->renderState->depthClampEnabled,
        .rasterizerDiscardEnable = prog->renderState->noPixels,
        .polygonMode = polygonModeTable[prog->renderState->polygonMode],
        .cullMode = cullModeTable[prog->renderState->cullMode],
        .frontFace = frontFaceTable[prog->renderState->windingOrderMode],
        .depthBiasEnable = prog->renderState->depthBiasEnabled,
        .depthBiasConstantFactor = prog->renderState->depthBiasFactor,
        .depthBiasClamp = prog->renderState->depthBiasClamp,
        .depthBiasSlopeFactor = prog->renderState->depthBiasSlopeFactor,
        .lineWidth = prog->renderState->lineWidth
    };

    static const VkCompareOp compareTable[] =
    {
        VK_COMPARE_OP_MAX_ENUM,
        VK_COMPARE_OP_NEVER,
        VK_COMPARE_OP_LESS,
        VK_COMPARE_OP_EQUAL,
        VK_COMPARE_OP_LESS_OR_EQUAL,
        VK_COMPARE_OP_GREATER,
        VK_COMPARE_OP_NOT_EQUAL,
        VK_COMPARE_OP_GREATER_OR_EQUAL,
        VK_COMPARE_OP_ALWAYS
    };

    static const VkStencilOp stencilOpTable[] =
    {
        VK_STENCIL_OP_MAX_ENUM,
        VK_STENCIL_OP_KEEP,
        VK_STENCIL_OP_ZERO,
        VK_STENCIL_OP_REPLACE,
        VK_STENCIL_OP_INCREMENT_AND_CLAMP,
        VK_STENCIL_OP_DECREMENT_AND_CLAMP,
        VK_STENCIL_OP_INVERT,
        VK_STENCIL_OP_INCREMENT_AND_WRAP,
        VK_STENCIL_OP_DECREMENT_AND_WRAP
    };

    ret.depthStencilInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .depthTestEnable = prog->renderState->depthTestEnabled,
        .depthWriteEnable = prog->renderState->depthWriteEnabled,
        .depthCompareOp = compareTable[prog->renderState->depthCompare],
        .depthBoundsTestEnable = prog->renderState->depthBoundsTestEnabled,
        .stencilTestEnable = prog->renderState->stencilEnabled,
        .front = 
        {
            .failOp = stencilOpTable[prog->renderState->frontStencilState.fail],
            .passOp = stencilOpTable[prog->renderState->frontStencilState.pass],
            .depthFailOp = stencilOpTable[prog->renderState->frontStencilState.depthFail],
            .compareOp = compareTable[prog->renderState->frontStencilState.compare],
            .compareMask = prog->renderState->frontStencilState.compareMask,
            .writeMask = prog->renderState->frontStencilState.writeMask,
            .reference = prog->renderState->frontStencilState.referenceMask
        },
        .back =
        {
            .failOp = stencilOpTable[prog->renderState->backStencilState.fail],
            .passOp = stencilOpTable[prog->renderState->backStencilState.pass],
            .depthFailOp = stencilOpTable[prog->renderState->backStencilState.depthFail],
            .compareOp = compareTable[prog->renderState->backStencilState.compare],
            .compareMask = prog->renderState->backStencilState.compareMask,
            .writeMask = prog->renderState->backStencilState.writeMask,
            .reference = prog->renderState->backStencilState.referenceMask
        },
        .minDepthBounds = prog->renderState->minDepthBounds,
        .maxDepthBounds = prog->renderState->maxDepthBounds,
    };

    static const VkBlendOp blendOpTable[] =
    {
        VK_BLEND_OP_MAX_ENUM,
        VK_BLEND_OP_ADD,
        VK_BLEND_OP_SUBTRACT,
        VK_BLEND_OP_REVERSE_SUBTRACT,
        VK_BLEND_OP_MIN,
        VK_BLEND_OP_MAX
    };
    static const VkBlendFactor blendFactorTable[] =
    {
        VK_BLEND_FACTOR_MAX_ENUM,
        VK_BLEND_FACTOR_ZERO,
        VK_BLEND_FACTOR_ONE,
        VK_BLEND_FACTOR_SRC_COLOR,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
        VK_BLEND_FACTOR_DST_COLOR,
        VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
        VK_BLEND_FACTOR_SRC_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        VK_BLEND_FACTOR_DST_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
        VK_BLEND_FACTOR_CONSTANT_COLOR,
        VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
        VK_BLEND_FACTOR_CONSTANT_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
        VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
        VK_BLEND_FACTOR_SRC1_COLOR,
        VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
        VK_BLEND_FACTOR_SRC1_ALPHA,
        VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA,
    };

    for (uint32_t i = 0; i < 8; i++)
    {
        ret.attachmentBlendInfo[i] =
        {
            .blendEnable = prog->renderState->blendStates[i].blendEnabled,
            .srcColorBlendFactor = blendFactorTable[prog->renderState->blendStates[i].sourceColorBlendFactor],
            .dstColorBlendFactor = blendFactorTable[prog->renderState->blendStates[i].destinationColorBlendFactor],
            .colorBlendOp = blendOpTable[prog->renderState->blendStates[i].colorBlendOp],
            .srcAlphaBlendFactor = blendFactorTable[prog->renderState->blendStates[i].sourceAlphaBlendFactor],
            .dstAlphaBlendFactor = blendFactorTable[prog->renderState->blendStates[i].destinationAlphaBlendFactor],
            .alphaBlendOp = blendOpTable[prog->renderState->blendStates[i].alphaBlendOp],
            .colorWriteMask = prog->renderState->blendStates[i].colorComponentMask
        };
    }

    VkLogicOp logicOpTable[] =
    {
        VK_LOGIC_OP_MAX_ENUM,
        VK_LOGIC_OP_CLEAR,
        VK_LOGIC_OP_AND,
        VK_LOGIC_OP_AND_REVERSE,
        VK_LOGIC_OP_COPY,
        VK_LOGIC_OP_AND_INVERTED,
        VK_LOGIC_OP_NO_OP,
        VK_LOGIC_OP_XOR,
        VK_LOGIC_OP_OR,
        VK_LOGIC_OP_NOR,
        VK_LOGIC_OP_EQUIVALENT,
        VK_LOGIC_OP_INVERT,
        VK_LOGIC_OP_OR_REVERSE,
        VK_LOGIC_OP_COPY_INVERTED,
        VK_LOGIC_OP_OR_INVERTED,
        VK_LOGIC_OP_NAND,
        VK_LOGIC_OP_SET
    };

    ret.blendInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .logicOpEnable = prog->renderState->logicOpEnabled,
        .logicOp = logicOpTable[prog->renderState->logicOp],
        .attachmentCount = 8,
        .pAttachments = ret.attachmentBlendInfo,
        .blendConstants = { prog->renderState->blendConstants[0], prog->renderState->blendConstants[1], prog->renderState->blendConstants[2], prog->renderState->blendConstants[3] }
    };

    return ret;
}

//------------------------------------------------------------------------------
/**
*/
VkGraphicsPipelineCreateInfo 
GetGraphicsPipeline(const VulkanPipelineInfo& info)
{
    return
    {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .stageCount = info.shaderCount,
        .pStages = info.shaderInfos,
        .pVertexInputState = nullptr,
        .pInputAssemblyState = nullptr,
        .pTessellationState = &info.tessellationInfo,
        .pViewportState = nullptr,
        .pRasterizationState = &info.rasterizationInfo,
        .pMultisampleState = nullptr,
        .pDepthStencilState = &info.depthStencilInfo,
        .pColorBlendState = &info.blendInfo,
        .pDynamicState = nullptr,
        .layout = info.pipelineLayout,
        .renderPass = VK_NULL_HANDLE,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = 0
    };
}

} // namespace GPULang

