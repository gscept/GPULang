
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"

#include "loader.h"
#include "reflection/vk/pipelineassembly.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STRINGIFY(x) #x
#define MERGE(x, y) STRINGIFY(x/##y)

#include MERGE(SHADER_HEADER_FOLDER, basicgraphics.h)
#include MERGE(SHADER_HEADER_FOLDER, computewithstore.h)

#include "GLFW/glfw3.h"

#include <cassert>
#include <stdio.h>
#include <cstdlib>
#include <time.h>

#include <vector>
#define _IMP_VK_BASE(name) { name = (PFN_##name)vkGetInstanceProcAddr(VK_NULL_HANDLE, #name);assert(name != nullptr); }
#define _IMP_VK(name) { name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);assert(name != nullptr); }
#define _DEF_VK(name) PFN_##name name = nullptr;
#define _DEC_VK(name) extern PFN_##name name;

_DEF_VK(vkGetInstanceProcAddr);
_DEF_VK(vkCreateInstance);
_DEF_VK(vkEnumerateInstanceExtensionProperties);
_DEF_VK(vkEnumerateInstanceLayerProperties);
_DEF_VK(vkEnumerateInstanceVersion);

_DEF_VK(vkGetPhysicalDeviceQueueFamilyProperties);
_DEF_VK(vkGetPhysicalDeviceMemoryProperties);
_DEF_VK(vkEnumeratePhysicalDevices);

_DEF_VK(vkGetPhysicalDeviceSurfaceFormatsKHR);
_DEF_VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
_DEF_VK(vkGetPhysicalDeviceSurfacePresentModesKHR);
_DEF_VK(vkCreateSwapchainKHR);
_DEF_VK(vkGetSwapchainImagesKHR);
_DEF_VK(vkAcquireNextImageKHR);
_DEF_VK(vkQueuePresentKHR);

_DEF_VK(vkGetImageMemoryRequirements);
_DEF_VK(vkGetBufferMemoryRequirements);

_DEF_VK(vkCreateDevice);
_DEF_VK(vkGetPhysicalDeviceFeatures2);
_DEF_VK(vkGetDeviceQueue);

_DEF_VK(vkAllocateMemory);
_DEF_VK(vkFreeMemory);
_DEF_VK(vkMapMemory);
_DEF_VK(vkUnmapMemory);
_DEF_VK(vkBindBufferMemory);
_DEF_VK(vkBindImageMemory);

_DEF_VK(vkCreateBuffer);
_DEF_VK(vkCreateImage);
_DEF_VK(vkCreateImageView);
_DEF_VK(vkCreateSampler);
_DEF_VK(vkCreateCommandPool);
_DEF_VK(vkAllocateCommandBuffers);
_DEF_VK(vkFreeCommandBuffers);

_DEF_VK(vkCreateDescriptorPool);
_DEF_VK(vkAllocateDescriptorSets);
_DEF_VK(vkFreeDescriptorSets);
_DEF_VK(vkUpdateDescriptorSets);

_DEF_VK(vkCreateFence);
_DEF_VK(vkCreateSemaphore);

_DEF_VK(vkCreateShaderModule);
_DEF_VK(vkCreatePipelineLayout);
_DEF_VK(vkCreateDescriptorSetLayout);
_DEF_VK(vkCreateGraphicsPipelines);
_DEF_VK(vkCreateComputePipelines);

_DEF_VK(vkQueueSubmit);
_DEF_VK(vkWaitForFences);
_DEF_VK(vkResetFences);

_DEF_VK(vkCreateDebugUtilsMessengerEXT);
_DEF_VK(vkDestroyDebugUtilsMessengerEXT);

_DEF_VK(vkCmdPipelineBarrier);
_DEF_VK(vkCmdClearColorImage);
_DEF_VK(vkCmdBindPipeline);
_DEF_VK(vkCmdBindVertexBuffers);
_DEF_VK(vkCmdBindIndexBuffer);
_DEF_VK(vkCmdBindDescriptorSets);
_DEF_VK(vkCmdCopyBufferToImage);
_DEF_VK(vkCmdDraw);
_DEF_VK(vkCmdDispatch);
_DEF_VK(vkBeginCommandBuffer);
_DEF_VK(vkEndCommandBuffer);
_DEF_VK(vkCmdBeginRendering);
_DEF_VK(vkCmdEndRendering);

template <class FLAGS, class BITS>
constexpr bool
AllBits(const FLAGS flags, const BITS bits)
{
    return (flags & bits) == bits;
}

//------------------------------------------------------------------------------
/**
*/
static constexpr uint64_t
operator"" _KB(const unsigned long long val)
{
    return val * 1024;
}

//------------------------------------------------------------------------------
/**
*/
static constexpr uint64_t
operator"" _MB(const unsigned long long val)
{
    return val * 1024 * 1024;
}

//------------------------------------------------------------------------------
/**
*/
static constexpr uint64_t
operator"" _GB(const unsigned long long val)
{
    return val * 1024 * 1024 * 1024;
}


//------------------------------------------------------------------------------
/**
*/
uint64_t
Align(uint64_t alignant, uint64_t alignment)
{
    return (alignant + alignment - 1) & ~(alignment - 1);
}

enum MemoryHeap
{
    HostVisible,
    DeviceLocal,
    Coherent,
    NumHeaps
};
VkDeviceMemory MemoryHeaps[MemoryHeap::NumHeaps] = { VK_NULL_HANDLE };
void* MappedMemory[MemoryHeap::NumHeaps];
uint64_t MemoryIterators[MemoryHeap::NumHeaps];
uint64_t HeapSizes[MemoryHeap::NumHeaps] =
{
    64_MB,
    512_MB,
    16_MB
};

//------------------------------------------------------------------------------
/**
*/
uint64_t 
Alloc(MemoryHeap heap, uint32_t size, uint32_t alignment)
{
    uint64_t alignedOffset = Align(MemoryIterators[heap], alignment);
    assert(alignedOffset + size < HeapSizes[heap]);
    uint64_t ret = alignedOffset;
    MemoryIterators[heap] = alignedOffset + size;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
void
Upload(MemoryHeap heap, T* data, uint32_t count = 1)
{
    assert(MappedMemory[heap] != nullptr);
    assert(MemoryIterators[heap] + sizeof(T) * count < HeapSizes[heap]);
    memcpy((char*)MappedMemory[heap] + MemoryIterators[heap], data, sizeof(T) * count);
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
uint64_t 
AllocAndUpload(MemoryHeap heap, T* data, uint32_t alignment, uint32_t count = 1)
{
    uint64_t alignedOffset = Align(MemoryIterators[heap], alignment);
    assert(MappedMemory[heap] != nullptr);
    assert(alignedOffset + sizeof(T) * count < HeapSizes[heap]);
    uint64_t ret = alignedOffset;
    memcpy((char*)MappedMemory[heap] + alignedOffset, data, sizeof(T) * count);
    MemoryIterators[heap] = alignedOffset + sizeof(T) * count;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
void
LoadVulkanModule()
{
#if __WIN32__
    HMODULE vulkanLib = LoadLibraryA("vulkan-1.dll");
    if (!vulkanLib)
    {
        printf("Could not find 'vulkan-1.dll', make sure you have installed vulkan");
        exit(1);
    }

    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkanLib, "vkGetInstanceProcAddr");
#elif __linux__
    void* vulkanLib = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
    if (!vulkanLib) vulkanLib = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    if (!vulkanLib)
    {
        n_error("Could not find 'libvulkan', make sure you have installed vulkan");
    }

    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(vulkanLib, "vkGetInstanceProcAddr");
#elif ( __OSX__ || __APPLE__ )
    void* vulkanLib = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
    if (!vulkanLib)
        vulkanLib = dlopen("libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL);
    if (!vulkanLib)
        vulkanLib = dlopen("libMoltenVK.dylib", RTLD_NOW | RTLD_LOCAL);
    else
        n_error("Could not find 'libvulkan', make sure you have installed vulkan");

    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(vulkanLib, "vkGetInstanceProcAddr");
#else
#error "Vulkan not supported on your platform!"
#endif
    
    _IMP_VK_BASE(vkCreateInstance);
    _IMP_VK_BASE(vkEnumerateInstanceExtensionProperties);
    _IMP_VK_BASE(vkEnumerateInstanceLayerProperties);
    _IMP_VK_BASE(vkEnumerateInstanceVersion);
}

#define VERIFY(x) { VkResult res = x; assert(res == VK_SUCCESS); }

//------------------------------------------------------------------------------
/**
*/
void
SetupVulkanBindings(VkInstance instance)
{
    _IMP_VK(vkGetPhysicalDeviceQueueFamilyProperties);
    _IMP_VK(vkGetPhysicalDeviceMemoryProperties);
    _IMP_VK(vkEnumeratePhysicalDevices);

    _IMP_VK(vkGetPhysicalDeviceSurfaceFormatsKHR);
    _IMP_VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
    _IMP_VK(vkGetPhysicalDeviceSurfacePresentModesKHR);
    _IMP_VK(vkCreateSwapchainKHR);
    _IMP_VK(vkGetSwapchainImagesKHR);
    _IMP_VK(vkAcquireNextImageKHR);
    _IMP_VK(vkQueuePresentKHR);

    _IMP_VK(vkGetImageMemoryRequirements);
    _IMP_VK(vkGetBufferMemoryRequirements);

    _IMP_VK(vkCreateDevice);
    _IMP_VK(vkGetPhysicalDeviceFeatures2);
    _IMP_VK(vkGetDeviceQueue);

    _IMP_VK(vkAllocateMemory);
    _IMP_VK(vkFreeMemory);
    _IMP_VK(vkMapMemory);
    _IMP_VK(vkUnmapMemory);
    _IMP_VK(vkBindBufferMemory);
    _IMP_VK(vkBindImageMemory);

    _IMP_VK(vkCreateShaderModule);
    _IMP_VK(vkCreatePipelineLayout);
    _IMP_VK(vkCreateDescriptorSetLayout);
    _IMP_VK(vkCreateGraphicsPipelines);
    _IMP_VK(vkCreateComputePipelines);

    _IMP_VK(vkQueueSubmit);
    _IMP_VK(vkWaitForFences);
    _IMP_VK(vkResetFences);

    _IMP_VK(vkCreateBuffer);
    _IMP_VK(vkCreateImage);
    _IMP_VK(vkCreateImageView);
    _IMP_VK(vkCreateSampler);
    _IMP_VK(vkCreateCommandPool);
    _IMP_VK(vkAllocateCommandBuffers);
    _IMP_VK(vkFreeCommandBuffers);

    _IMP_VK(vkCreateDescriptorPool);
    _IMP_VK(vkAllocateDescriptorSets);
    _IMP_VK(vkFreeDescriptorSets);
    _IMP_VK(vkUpdateDescriptorSets);

    _IMP_VK(vkCreateFence);
    _IMP_VK(vkCreateSemaphore);

    _IMP_VK(vkCmdPipelineBarrier);
    _IMP_VK(vkCmdClearColorImage);
    _IMP_VK(vkCmdBindPipeline);
    _IMP_VK(vkCmdBindVertexBuffers);
    _IMP_VK(vkCmdBindIndexBuffer);
    _IMP_VK(vkCmdBindDescriptorSets);
    _IMP_VK(vkCmdCopyBufferToImage);
    _IMP_VK(vkCmdDraw);
    _IMP_VK(vkCmdDispatch);

    _IMP_VK(vkBeginCommandBuffer);
    _IMP_VK(vkEndCommandBuffer);
    _IMP_VK(vkCmdBeginRendering);
    _IMP_VK(vkCmdEndRendering);
}

struct Program
{
    char* memory;
    GPULang::Loader loader;
    GPULang::VulkanPipelineInfo pipelineInfo;
};

//------------------------------------------------------------------------------
/**
*/
Program
LoadShader(VkDevice device, std::string shader)
{
    std::string path = std::string(SHADER_FOLDER) + "/" + shader;
    FILE* file = fopen(path.c_str(), "rb");
    GPULang::Loader loader;
    int res = fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    char* buf = new char[size];
    fread(buf, sizeof(char), size, file);
    loader.Load(buf, size);

    auto prog = loader.Get<GPULang::ProgramObject>("TestProgram");

    GPULang::VulkanPipelineInfo vulkanPipeline = GPULang::SetupVulkan(
        device
        , prog
        , loader.variables.data()
        , loader.variables.size()
        , GPULang::VulkanFunctions{ .vkCreateShaderModule = vkCreateShaderModule, .vkCreatePipelineLayout = vkCreatePipelineLayout, .vkCreateDescriptorSetLayout = vkCreateDescriptorSetLayout }
    );
    Program ret;

    fclose(file);
    ret.pipelineInfo = vulkanPipeline;
    ret.memory = buf;
    ret.loader = loader;
    return ret;
}

//------------------------------------------------------------------------------
/**
*/
VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanErrorCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData)
{
    printf("%s\n", callbackData->pMessage);
    return VK_FALSE;
}

#if __WIN32__
#define PLATFORM_MAIN WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif

int PLATFORM_MAIN
{
    glfwInit();

    // setup application
    VkApplicationInfo appInfo =
    {
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        nullptr,
        "GPULang Test App",
        1,															// application version
        "GPULang",													// engine name
        1,															// engine version
        VK_API_VERSION_1_3											// API version
    };

    uint32_t requiredExtensionsNum;
    const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionsNum);

    std::vector<const char*> extensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    for (uint32_t i = 0; i < requiredExtensionsNum; i++)
        extensions.push_back(requiredExtensions[i]);

    std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };

    // Load Vulkan DLL so we can bind the functions we need
    LoadVulkanModule();

    // setup non-instance calls
    VkInstance instance = VK_NULL_HANDLE;

    VkInstanceCreateInfo instanceInfo =
    {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,		// type of struct
        nullptr,										// pointer to next
        0,											// flags
        &appInfo,									// application
        (uint32_t)layers.size(),
        layers.data(),
        (uint32_t)extensions.size(),
        extensions.data(),
    };
    VERIFY(vkCreateInstance(&instanceInfo, nullptr, &instance));

    SetupVulkanBindings(instance);

    _IMP_VK(vkCreateDebugUtilsMessengerEXT);
    _IMP_VK(vkDestroyDebugUtilsMessengerEXT);
    VkDebugUtilsMessengerCreateInfoEXT dbgInfo;
    dbgInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    dbgInfo.flags = 0;
    dbgInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    dbgInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    dbgInfo.pNext = nullptr;
    dbgInfo.pfnUserCallback = VulkanErrorCallback;
    dbgInfo.pUserData = nullptr;
    VkDebugUtilsMessengerEXT VkErrorDebugMessageHandle;
    VERIFY(vkCreateDebugUtilsMessengerEXT(instance, &dbgInfo, NULL, &VkErrorDebugMessageHandle));

    VkPhysicalDevice devices[4];
    uint32_t count = 4;
    VERIFY(vkEnumeratePhysicalDevices(instance, &count, devices));

    VkQueueFamilyProperties queueFamilyProps[64];
    uint32_t queueFamilyCount = 64;
    vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, queueFamilyProps);

    uint32_t selectedQueue = UINT_MAX;
    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilyProps[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
            selectedQueue = i;
    }
    assert(selectedQueue != UINT_MAX);

    VkPhysicalDeviceDynamicRenderingFeatures dynamicRendering =
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
        .pNext = nullptr,
        .dynamicRendering = true
    };

    VkPhysicalDeviceVulkan11Features vk11Features =
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .pNext = &dynamicRendering,
    };

    VkPhysicalDeviceVulkan12Features vk12Features =
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .pNext = &vk11Features,
    };

    VkPhysicalDeviceFeatures2 features2 =
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
        .pNext = &vk12Features
    };
    vkGetPhysicalDeviceFeatures2(devices[0], &features2);

    float priorities[64] = { 1.0f };
    VkDeviceQueueCreateInfo queueInfo =
    {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        nullptr,
        0,
        selectedQueue,
        1,
        priorities
    };

    std::vector<const char*> deviceFeatures = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME };
    
    VkDeviceCreateInfo deviceInfo =
    {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        &vk12Features,
        0x0,
        1,
        &queueInfo,
        (uint32_t)layers.size(),
        layers.data(),
        (uint32_t)deviceFeatures.size(),
        deviceFeatures.data(),
        &features2.features
    };
    VkDevice device;
    VERIFY(vkCreateDevice(devices[0], &deviceInfo, nullptr, &device));

    Program graphicsProgram = LoadShader(device, "basicgraphics.gplb");
    Program computeProgram = LoadShader(device, "computewithstore.gplb");

    VkQueue queue;
    vkGetDeviceQueue(device, selectedQueue, 0, &queue);

    // Setup heaps, statically allocate a bunch of memory for this app
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties(devices[0], &memProps);

    for (uint32_t i = 0; i < memProps.memoryTypeCount; i++)
    {
        VkMemoryType type = memProps.memoryTypes[i];
        VkMemoryHeap heap = memProps.memoryHeaps[type.heapIndex];
        if (AllBits(type.propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) && MemoryHeaps[MemoryHeap::HostVisible] == VK_NULL_HANDLE)
        {
            VkMemoryAllocateInfo allocInfo =
            {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                nullptr,
                HeapSizes[MemoryHeap::HostVisible],
                i
            };
            VERIFY(vkAllocateMemory(device, &allocInfo, nullptr, &MemoryHeaps[MemoryHeap::HostVisible]));
            vkMapMemory(device, MemoryHeaps[MemoryHeap::HostVisible], 0, HeapSizes[MemoryHeap::HostVisible], 0x0, &MappedMemory[MemoryHeap::HostVisible]);
        }
        else if (AllBits(type.propertyFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) && MemoryHeaps[MemoryHeap::DeviceLocal] == VK_NULL_HANDLE)
        {
            VkMemoryAllocateInfo allocInfo =
            {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                nullptr,
                HeapSizes[MemoryHeap::DeviceLocal],
                i
            };
            VERIFY(vkAllocateMemory(device, &allocInfo, nullptr, &MemoryHeaps[MemoryHeap::DeviceLocal]));
        }
        else if (AllBits(type.propertyFlags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) && MemoryHeaps[MemoryHeap::Coherent] == VK_NULL_HANDLE)
        {
            VkMemoryAllocateInfo allocInfo =
            {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                nullptr,
                HeapSizes[MemoryHeap::Coherent],
                i
            };
            VERIFY(vkAllocateMemory(device, &allocInfo, nullptr, &MemoryHeaps[MemoryHeap::Coherent]));
            vkMapMemory(device, MemoryHeaps[MemoryHeap::Coherent], 0, HeapSizes[MemoryHeap::Coherent], 0x0, &MappedMemory[MemoryHeap::Coherent]);
        }
    }

    // Create window
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    uint32_t windowWidth = 1024, windowHeight = 768;

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "GPULang app", nullptr, nullptr);
    glfwSwapInterval(0);

    float xScale, yScale;
    glfwGetWindowContentScale(window, &xScale, &yScale);
    windowWidth = uint32_t(windowWidth * xScale);
    windowHeight = uint32_t(windowHeight * yScale);

    VkSurfaceKHR surface;
    VERIFY(glfwCreateWindowSurface(instance, window, nullptr, &surface));

    uint32_t numFormats = 256;
    VkSurfaceFormatKHR formats[256];
    VERIFY(vkGetPhysicalDeviceSurfaceFormatsKHR(devices[0], surface, &numFormats, formats));

    VkFormat format = formats[0].format;
    VkColorSpaceKHR colorSpace = formats[0].colorSpace;
    VkComponentMapping mapping = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
    for (uint32_t i = 0; i < numFormats; i++)
    {
        if (formats[i].format == VK_FORMAT_R8G8B8A8_SRGB)
        {
            format = formats[i].format;
            colorSpace = formats[i].colorSpace;
            //mapping.b = VK_COMPONENT_SWIZZLE_R;
            //mapping.r = VK_COMPONENT_SWIZZLE_B;
            break;
        }
    }

    VkSurfaceCapabilitiesKHR surfCaps;
    VERIFY(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[0], surface, &surfCaps));

    uint32_t numPresentModes = 16;
    VkPresentModeKHR presentModes[16];
    VERIFY(vkGetPhysicalDeviceSurfacePresentModesKHR(devices[0], surface, &numPresentModes, presentModes));

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    for (uint32_t i = 0; i < numPresentModes; i++)
    {
        switch (presentModes[i])
        {
            case VK_PRESENT_MODE_IMMEDIATE_KHR:
                numPresentModes = 0;
                break;
            default:
                continue;
        }
    }
    assert(numPresentModes == 0);

    VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    VkExtent2D swapchainExtent = { .width = windowWidth, .height = windowHeight };
    VkSwapchainCreateInfoKHR swapchainInfo =
    {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        nullptr,
        0,
        surface,
        3,
        format,
        colorSpace,
        swapchainExtent,
        1,
        usageFlags,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        nullptr,
        transform,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        swapchainPresentMode,
        VK_TRUE,
        VK_NULL_HANDLE
    };

    VkSwapchainKHR swapchain;
    VERIFY(vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain));

    uint32_t numBuffers = 3;
    VkImage backbufferImages[3];
    VkImageView backbufferImageViews[3];
    VERIFY(vkGetSwapchainImagesKHR(device, swapchain, &numBuffers, backbufferImages));

    VkCommandPoolCreateInfo poolInfo =
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        .queueFamilyIndex = selectedQueue,
    };
    VkCommandPool commandPool;
    VERIFY(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool));

    VkCommandBufferAllocateInfo cmdBufferAllocInfo =
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = commandPool,
        .level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    VkCommandBuffer cmdBuf;
    VERIFY(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, &cmdBuf));

    VkCommandBufferBeginInfo beginInfo =
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr
    };
    VERIFY(vkBeginCommandBuffer(cmdBuf, &beginInfo));

    for (uint32_t i = 0; i < numBuffers; i++)
    {
        VkImageMemoryBarrier imageBarrier;
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageBarrier.pNext = nullptr;
        imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.image = backbufferImages[i];
        imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        imageBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        vkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0x0, 0, nullptr, 0, nullptr, 1, &imageBarrier);

        // setup view
        VkImageViewCreateInfo backbufferViewInfo =
        {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            nullptr,
            0,
            backbufferImages[i],
            VK_IMAGE_VIEW_TYPE_2D,
            format,
            mapping,
            { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
        };
        VERIFY(vkCreateImageView(device, &backbufferViewInfo, nullptr, &backbufferImageViews[i]));
    }

    // Create graphics graphicsPipeline
    VkPipelineRenderingCreateInfo passInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .pNext = nullptr,
        .viewMask = 0x1,
        .colorAttachmentCount = 1,
        .pColorAttachmentFormats = &format,
        .depthAttachmentFormat = VK_FORMAT_D32_SFLOAT_S8_UINT,
        .stencilAttachmentFormat = VK_FORMAT_D32_SFLOAT_S8_UINT
    };
    VkVertexInputBindingDescription vertexBinding =
    {
        .binding = 0,
        .stride = Basicgraphics::BasicVertex::VERTEX_STRIDE,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };
    VkVertexInputAttributeDescription vertexAttributes[] =
    {
        {
            .location = Basicgraphics::BasicVertex::Position_BINDING,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = Basicgraphics::BasicVertex::Position_OFFSET
        },
        {
            .location = Basicgraphics::BasicVertex::UV_BINDING,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = Basicgraphics::BasicVertex::UV_OFFSET
        }
    };
    VkPipelineVertexInputStateCreateInfo vertexInput =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &vertexBinding,
        .vertexAttributeDescriptionCount = 2,
        .pVertexAttributeDescriptions = vertexAttributes

    };
    VkPipelineInputAssemblyStateCreateInfo inputAssembly =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = false,
    };
    VkPipelineMultisampleStateCreateInfo multiSample =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .rasterizationSamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = false,
        .minSampleShading = 1.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = false,
        .alphaToOneEnable = false
    };

    VkViewport viewport = { .width = (float)windowWidth, .height =  (float)windowHeight };
    VkRect2D scissor = { .offset = { .x = 0, .y = 0 }, .extent = { .width = windowWidth, .height = windowHeight } };
    VkPipelineViewportStateCreateInfo viewportInfo =
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor
    };
    graphicsProgram.pipelineInfo.blendInfo.attachmentCount = passInfo.colorAttachmentCount;
    VkGraphicsPipelineCreateInfo gfxPsoInfo = GPULang::GetGraphicsPipeline(graphicsProgram.pipelineInfo);
    gfxPsoInfo.pInputAssemblyState = &inputAssembly;
    gfxPsoInfo.pVertexInputState = &vertexInput;
    gfxPsoInfo.pNext = &passInfo;
    gfxPsoInfo.pViewportState = &viewportInfo;
    gfxPsoInfo.pMultisampleState = &multiSample;
    gfxPsoInfo.basePipelineHandle = VK_NULL_HANDLE;
    gfxPsoInfo.basePipelineIndex = -1;
    VkPipeline graphicsPipeline;
    VERIFY(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &gfxPsoInfo, nullptr, &graphicsPipeline));

    VkComputePipelineCreateInfo cmpPsoInfo = GPULang::GetComputePipeline(computeProgram.pipelineInfo);
    VkPipeline computePipeline;
    VERIFY(vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &cmpPsoInfo, nullptr, &computePipeline));

    static bool terminate = false;

    float vbo[] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

    VkBufferCreateInfo bufInfo =
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .size = sizeof(vbo),
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr
    };
    VkBuffer vboBuf, iboBuf;
    VERIFY(vkCreateBuffer(device, &bufInfo, nullptr, &vboBuf));
    {
        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(device, vboBuf, &memReqs);
        uint64_t offset = AllocAndUpload(MemoryHeap::Coherent, vbo, memReqs.alignment, _countof(vbo));
        VERIFY(vkBindBufferMemory(device, vboBuf, MemoryHeaps[MemoryHeap::Coherent], offset));
    }

    float ibo[] =
    {
        0, 1, 2
    };
    bufInfo.size = sizeof(ibo);
    bufInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    VERIFY(vkCreateBuffer(device, &bufInfo, nullptr, &iboBuf));
    {
        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(device, iboBuf, &memReqs);
        uint64_t offset = AllocAndUpload(MemoryHeap::Coherent, ibo, memReqs.alignment, _countof(ibo));
        VERIFY(vkBindBufferMemory(device, iboBuf, MemoryHeaps[MemoryHeap::Coherent], offset));
    }

    int x, y, comp;
    stbi_uc* image = stbi_load(std::string(std::string(TEXTURE_FOLDER) + "/isolfacion.png").c_str(), &x, &y, &comp, 0);
    VkImageCreateInfo imageInfo =
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .imageType = VkImageType::VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_R8G8B8A8_SRGB,
        .extent = { .width = (uint32_t)x, .height = (uint32_t)y, .depth = 1 },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };
    VkImage texture;
    VERIFY(vkCreateImage(device, &imageInfo, nullptr, &texture));
    uint64_t texByteSize = 0;
    {
        VkMemoryRequirements memReqs;
        vkGetImageMemoryRequirements(device, texture, &memReqs);
        uint64_t offset = Alloc(MemoryHeap::DeviceLocal, memReqs.size, memReqs.alignment);
        VERIFY(vkBindImageMemory(device, texture, MemoryHeaps[MemoryHeap::DeviceLocal], offset));
        texByteSize = memReqs.size;
    }

    VkBufferCreateInfo uploadBufferInfo =
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .size = texByteSize,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr
    };
    VkBuffer uploadBuffer;
    VERIFY(vkCreateBuffer(device, &uploadBufferInfo, nullptr, &uploadBuffer));
    {
        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(device, uploadBuffer, &memReqs);
        uint64_t offset = AllocAndUpload(MemoryHeap::HostVisible, (const char*)image, memReqs.alignment, x * y * comp);
        VERIFY(vkBindBufferMemory(device, uploadBuffer, MemoryHeaps[MemoryHeap::HostVisible], offset));
    }

    {
        VkImageMemoryBarrier imageBarrier;
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageBarrier.pNext = nullptr;
        imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.image = texture;
        imageBarrier.srcAccessMask = VK_ACCESS_NONE;
        imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        vkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0x0, 0, nullptr, 0, nullptr, 1, &imageBarrier);
    }

    VkBufferImageCopy copy =
    {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .mipLevel = 0, .baseArrayLayer = 0, .layerCount = 1 },
        .imageOffset = { .x = 0, .y = 0, .z = 0 },
        .imageExtent = { .width = (uint32_t)x, .height = (uint32_t)y, .depth = 1 }
    };
    vkCmdCopyBufferToImage(cmdBuf, uploadBuffer, texture, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);

    {
        VkImageMemoryBarrier imageBarrier;
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageBarrier.pNext = nullptr;
        imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.image = texture;
        imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        vkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, 0x0, 0, nullptr, 0, nullptr, 1, &imageBarrier);
    }

    VkImageViewCreateInfo viewCreate =
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .image = texture,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = imageInfo.format,
        .components = VK_COMPONENT_SWIZZLE_IDENTITY,
        .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
    };
    VkImageView view;
    VERIFY(vkCreateImageView(device, &viewCreate, nullptr, &view));

    VkImageCreateInfo depthBufferInfo =
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .imageType = VkImageType::VK_IMAGE_TYPE_2D,
        .format = VK_FORMAT_D32_SFLOAT_S8_UINT,
        .extent = { .width = (uint32_t)windowWidth, .height = (uint32_t)windowHeight, .depth = 1 },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };
    VkImage depthBuffer;
    VERIFY(vkCreateImage(device, &depthBufferInfo, nullptr, &depthBuffer));
    {
        VkMemoryRequirements memReqs;
        vkGetImageMemoryRequirements(device, depthBuffer, &memReqs);
        uint64_t offset = Alloc(MemoryHeap::DeviceLocal, memReqs.size, memReqs.alignment);
        VERIFY(vkBindImageMemory(device, depthBuffer, MemoryHeaps[MemoryHeap::DeviceLocal], offset));
    }
    VkImageViewCreateInfo depthBufferViewInfo =
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .image = depthBuffer,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = depthBufferInfo.format,
        .components = VK_COMPONENT_SWIZZLE_IDENTITY,
        .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
    };
    VkImageView depthBufferView;
    VERIFY(vkCreateImageView(device, &depthBufferViewInfo, nullptr, &depthBufferView));

    {
        VkImageMemoryBarrier imageBarrier;
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageBarrier.pNext = nullptr;
        imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageBarrier.image = depthBuffer;
        imageBarrier.srcAccessMask = VK_ACCESS_NONE;
        imageBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        imageBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 };
        vkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, 0x0, 0, nullptr, 0, nullptr, 1, &imageBarrier);
    }

    VERIFY(vkEndCommandBuffer(cmdBuf));

    VkFenceCreateInfo fenceInfo =
    {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0
    };

    VkFence fence;
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &fence));
    VkSubmitInfo initSubmit =
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = nullptr,
        .commandBufferCount = 1,
        .pCommandBuffers = &cmdBuf,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = nullptr
    };
    VERIFY(vkQueueSubmit(queue, 1, &initSubmit, fence));

    VERIFY(vkWaitForFences(device, 1, &fence, true, UINT64_MAX));

    // Safe to delete when the submission has finished
    vkFreeCommandBuffers(device, commandPool, 1, &cmdBuf);

    VkSemaphore frameSemaphores[3];
    VkSemaphoreCreateInfo semInfo =
    {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
    };
    VERIFY(vkCreateSemaphore(device, &semInfo, nullptr, &frameSemaphores[0]));
    VERIFY(vkCreateSemaphore(device, &semInfo, nullptr, &frameSemaphores[1]));
    VERIFY(vkCreateSemaphore(device, &semInfo, nullptr, &frameSemaphores[2]));

    VkFence frameFences[3];
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &frameFences[0]));
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &frameFences[1]));
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &frameFences[2]));

    VkFence presentFences[3];

    //fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &presentFences[0]));
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &presentFences[1]));
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &presentFences[2]));

    VkDescriptorPoolSize poolSizes[8];
    uint32_t poolSizeCounter = 0;

    VkDescriptorPoolCreateInfo descriptorPoolInfo =
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .maxSets = 64,
        .poolSizeCount = graphicsProgram.pipelineInfo.poolSizeCounter,
        .pPoolSizes = graphicsProgram.pipelineInfo.descriptorPoolSizes
    };
    VkDescriptorPool descriptorPool;
    VERIFY(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));

    VkDescriptorSetAllocateInfo descriptorAlloc =
    {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = descriptorPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &graphicsProgram.pipelineInfo.groupLayouts[0]
    };
    VkDescriptorSet descriptors;
    vkAllocateDescriptorSets(device, &descriptorAlloc, &descriptors);

    VkSamplerCreateInfo samplerInfo =
    {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .magFilter = VkFilter::VK_FILTER_LINEAR,
        .minFilter = VkFilter::VK_FILTER_LINEAR,
        .mipmapMode = VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .addressModeU = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .addressModeV = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .addressModeW = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .mipLodBias = 0.0f,
        .anisotropyEnable = true,
        .maxAnisotropy = 16.0f,
        .compareEnable = false,
        .compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS,
        .minLod = -FLT_MAX,
        .maxLod = FLT_MAX,
        .borderColor = VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
        .unnormalizedCoordinates = false
    };
    VkSampler sampler;
    VERIFY(vkCreateSampler(device, &samplerInfo, nullptr, &sampler));

    uint32_t cameraBufSize = Basicgraphics::UniformBuffer_camera::SIZE;
    uint32_t objectBufSize = Basicgraphics::UniformBuffer_object::SIZE;
    VkBufferCreateInfo shaderBufferInfo =
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .size = cameraBufSize,
        .usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr
    };
    VkBuffer cameraBuf, objectBuf;
    VERIFY(vkCreateBuffer(device, &shaderBufferInfo, nullptr, &cameraBuf));

    Basicgraphics::camera::STRUCT cameraData;
    cameraData =
    {
        .viewProjection = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        },
        .position = { 0, 0, 0, 1 }
    };
    {
        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(device, cameraBuf, &memReqs);
        uint64_t offset = AllocAndUpload(MemoryHeap::Coherent, &cameraData, memReqs.alignment);
        VERIFY(vkBindBufferMemory(device, cameraBuf, MemoryHeaps[MemoryHeap::Coherent], offset));
    }

    Basicgraphics::object::STRUCT objectData =
    {
        .model =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        }
    };
    shaderBufferInfo.size = objectBufSize;
    VERIFY(vkCreateBuffer(device, &shaderBufferInfo, nullptr, &objectBuf));
    {
        VkMemoryRequirements memReqs;
        vkGetBufferMemoryRequirements(device, objectBuf, &memReqs);
        uint64_t offset = AllocAndUpload(MemoryHeap::Coherent, &objectData, memReqs.alignment);
        VERIFY(vkBindBufferMemory(device, objectBuf, MemoryHeaps[MemoryHeap::Coherent], offset));
    }

    VkDescriptorImageInfo descriptorImageInfo =
    {
        .sampler = sampler,
        .imageView = view,
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    VkDescriptorBufferInfo descriptorCameraBufferInfo =
    {
        .buffer = cameraBuf,
        .offset = 0,
        .range = VK_WHOLE_SIZE
    };
    VkDescriptorBufferInfo descriptorObjectBufferInfo =
    {
        .buffer = objectBuf,
        .offset = 0,
        .range = VK_WHOLE_SIZE
    };
    VkWriteDescriptorSet writes[] =
    {
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = descriptors,
            .dstBinding = Basicgraphics::Albedo::BINDING,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
            .pImageInfo = &descriptorImageInfo,
            .pBufferInfo = nullptr,
            .pTexelBufferView = nullptr
        },
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = descriptors,
            .dstBinding = Basicgraphics::Material::BINDING,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &descriptorImageInfo,
            .pBufferInfo = nullptr,
            .pTexelBufferView = nullptr
        },
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = descriptors,
            .dstBinding = Basicgraphics::Sampler::BINDING,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
            .pImageInfo = &descriptorImageInfo,
            .pBufferInfo = nullptr,
            .pTexelBufferView = nullptr
        },
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = descriptors,
            .dstBinding = Basicgraphics::camera::BINDING,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pImageInfo = nullptr,
            .pBufferInfo = &descriptorCameraBufferInfo,
            .pTexelBufferView = nullptr
        },
        {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = descriptors,
            .dstBinding = Basicgraphics::object::BINDING,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pImageInfo = nullptr,
            .pBufferInfo = &descriptorObjectBufferInfo,
            .pTexelBufferView = nullptr
        },
        
    };
    vkUpdateDescriptorSets(device, 5, writes, 0, nullptr);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            terminate = true;
    });

    // Update loop
    VkCommandBuffer frameBuffers[3] = { VK_NULL_HANDLE };
    uint32_t frameBufferCounter = 0;

    float colorShift = 0.0f;
    clock_t before = clock();
    while (!terminate)
    {
        clock_t now = clock();
        float diff = (now - before) * 1000.0f / CLOCKS_PER_SEC;
        before = now;
        printf("%f", diff);

        glfwPollEvents();
        terminate |= glfwWindowShouldClose(window);

        uint32_t index = 0;
        VkResult acquireRes = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, VK_NULL_HANDLE, presentFences[frameBufferCounter], &index);
        switch (acquireRes)
        {
            case VK_SUCCESS:
            case VK_ERROR_OUT_OF_DATE_KHR:
            case VK_SUBOPTIMAL_KHR:
                break;
            default:
                exit(1);
        }

        if (frameBuffers[frameBufferCounter] != VK_NULL_HANDLE)
        {
            // Wait for previous frame
            vkWaitForFences(device, 1, &frameFences[frameBufferCounter], true, UINT64_MAX);
            vkFreeCommandBuffers(device, commandPool, 1, &frameBuffers[frameBufferCounter]);
            frameBuffers[frameBufferCounter] = VK_NULL_HANDLE;
        }

        VkCommandBufferAllocateInfo cmdBufferAllocInfo =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = commandPool,
            .level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };

        VERIFY(vkAllocateCommandBuffers(device, &cmdBufferAllocInfo, &frameBuffers[frameBufferCounter]));
        VkCommandBuffer cmdBuf = frameBuffers[frameBufferCounter];

        VkCommandBufferBeginInfo beginInfo =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr
        };
        VERIFY(vkBeginCommandBuffer(cmdBuf, &beginInfo));

        colorShift += diff / 1000.0f;
        VkClearColorValue clear = { .float32 = { 0.5f + (float)sin(colorShift * 2.0f), 0.5f + (float)sin(colorShift * 0.5f), 0.5f + (float)sin(colorShift), 1.0f } };
        VkImageSubresourceRange range =
        {
            .aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };

        VkImageMemoryBarrier imageBarrier = 
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = backbufferImages[frameBufferCounter],
            .subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
        };
        vkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0x0, 0, nullptr, 0, nullptr, 1, &imageBarrier);

        VkRenderingAttachmentInfo colorAttachment =
        {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = backbufferImageViews[frameBufferCounter],
            .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .resolveMode = VK_RESOLVE_MODE_NONE,
            .resolveImageView = VK_NULL_HANDLE,
            .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .clearValue = VkClearValue{.color = clear }
        };
        VkRenderingAttachmentInfo depthAttachment =
        {
            .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .pNext = nullptr,
            .imageView = depthBufferView,
            .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .resolveMode = VK_RESOLVE_MODE_NONE,
            .resolveImageView = VK_NULL_HANDLE,
            .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .clearValue = VkClearValue{.depthStencil = {.depth = 1.0f }}
        };
        VkRenderingInfo renderInfo =
        {
            .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
            .pNext = nullptr,
            .flags = 0x0,
            .renderArea = VkRect2D{ .offset = { .x = 0, .y = 0 }, .extent = { .width = windowWidth, .height = windowHeight } },
            .layerCount = 1,
            .viewMask = 0x1,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachment,
            .pDepthAttachment = &depthAttachment,
            .pStencilAttachment = &depthAttachment
        };

        vkCmdBeginRendering(cmdBuf, &renderInfo);
        vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        uint64_t offset = 0;
        vkCmdBindVertexBuffers(cmdBuf, 0, 1, &vboBuf, &offset);
        vkCmdBindIndexBuffer(cmdBuf, iboBuf, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsProgram.pipelineInfo.pipelineLayout, 0, 1, &descriptors, 0, nullptr);
        vkCmdDraw(cmdBuf, 3, 1, 0, 0);
        vkCmdEndRendering(cmdBuf);

        imageBarrier =
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = backbufferImages[frameBufferCounter],
            .subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
        };
        vkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0x0, 0, nullptr, 0, nullptr, 1, &imageBarrier);

        VERIFY(vkEndCommandBuffer(cmdBuf));

        VkSubmitInfo initSubmit =
        {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &cmdBuf,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr
        };
        VERIFY(vkResetFences(device, 1, &frameFences[frameBufferCounter]));
        VERIFY(vkQueueSubmit(queue, 1, &initSubmit, frameFences[frameBufferCounter]));

        VERIFY(vkWaitForFences(device, 1, &presentFences[frameBufferCounter], true, UINT64_MAX));
        VERIFY(vkResetFences(device, 1, &presentFences[frameBufferCounter]));

        VkPresentInfoKHR presentInfo =
        {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .swapchainCount = 1,
            .pSwapchains = &swapchain,
            .pImageIndices = &index
        };
        VkResult presentRes = vkQueuePresentKHR(queue, &presentInfo);
        switch (presentRes)
        {
            case VK_SUCCESS:
            case VK_ERROR_OUT_OF_DATE_KHR:
            case VK_SUBOPTIMAL_KHR:
                break;
            default:
                exit(1);
        }

        frameBufferCounter = (frameBufferCounter + 1) % 3;
    }

    vkDestroyDebugUtilsMessengerEXT(instance, VkErrorDebugMessageHandle, nullptr);

    glfwTerminate();

    return 0;
}