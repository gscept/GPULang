#pragma once
//------------------------------------------------------------------------------
/**
    Helper functions to convert a GPULang objects to Vulkan

    @copyright
    (C) 2024 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <vulkan/vulkan.h>
#include "util/serialize.h"
namespace GPULang
{

struct VulkanPipelineInfo
{
    VkPipelineShaderStageCreateInfo shaderInfos[Deserialize::Program::ShaderStages::NumShaders];
    VkShaderModule shaders[Deserialize::Program::ShaderStages::NumShaders];
    uint32_t shaderCount;
    VkPipelineTessellationStateCreateInfo tessellationInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

    VkPipelineColorBlendAttachmentState attachmentBlendInfo[8];
    VkPipelineColorBlendStateCreateInfo blendInfo;

    uint32_t groupLayoutCounter;
    VkDescriptorSetLayout groupLayouts[8];
    uint32_t groupBindingCounter[8];
    VkDescriptorSetLayoutBinding groupBindings[8][64];

    VkPipelineLayout pipelineLayout;

    uint32_t poolSizeCounter;
    VkDescriptorPoolSize descriptorPoolSizes[8];

    enum PipelineType
    {
        Graphics,
        Compute,
        Raytracing
    } type;
};

/// Convert a GPULang program to a partially filled in graphics pipeline info
struct VulkanFunctions
{
    PFN_vkCreateShaderModule vkCreateShaderModule;
    PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
    PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
};

/// Assemble a program to a vulkan pipeline setup
VulkanPipelineInfo SetupVulkan(const VkDevice device, const Deserialize::Program* prog, GPULang::Deserialize::Variable** variables, uint32_t numVariables, const VulkanFunctions& functionBindings);

/// Finalize and create a graphics pipeline info
VkGraphicsPipelineCreateInfo GetGraphicsPipeline(const VulkanPipelineInfo& info);
/// Finalize and create a compute pipeline
VkComputePipelineCreateInfo GetComputePipeline(const VulkanPipelineInfo& info);


} // namespace GPULang
