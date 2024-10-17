
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"

#include "loader.h"

#include "GLFW/glfw3.h"

#include <cassert>
#include <stdio.h>
#include <cstdlib>

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

_DEF_VK(vkCreateDevice);
_DEF_VK(vkGetPhysicalDeviceFeatures2);
_DEF_VK(vkGetDeviceQueue);

_DEF_VK(vkAllocateMemory);
_DEF_VK(vkFreeMemory);
_DEF_VK(vkMapMemory);
_DEF_VK(vkUnmapMemory);

_DEF_VK(vkCreateBuffer);
_DEF_VK(vkCreateImage);
_DEF_VK(vkCreateImageView);
_DEF_VK(vkCreateCommandPool);
_DEF_VK(vkAllocateCommandBuffers);
_DEF_VK(vkFreeCommandBuffers);

_DEF_VK(vkCreateFence);
_DEF_VK(vkCreateSemaphore);

_DEF_VK(vkCreateShaderModule);
_DEF_VK(vkCreateGraphicsPipelines);
_DEF_VK(vkCreateComputePipelines);

_DEF_VK(vkQueueSubmit);
_DEF_VK(vkWaitForFences);
_DEF_VK(vkResetFences);

_DEF_VK(vkCreateDebugUtilsMessengerEXT);
_DEF_VK(vkDestroyDebugUtilsMessengerEXT);

_DEF_VK(vkCmdPipelineBarrier);
_DEF_VK(vkCmdClearColorImage);
_DEF_VK(vkBeginCommandBuffer);
_DEF_VK(vkEndCommandBuffer);

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

enum MemoryHeap
{
    HostVisible,
    DeviceLocal,
    Coherent,
    NumHeaps
};
VkDeviceMemory MemoryHeaps[MemoryHeap::NumHeaps] = { VK_NULL_HANDLE };
void* MappedMemory[MemoryHeap::NumHeaps];
uint64_t HeapSizes[MemoryHeap::NumHeaps] =
{
    64_MB,
    512_MB,
    16_MB
};

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

    _IMP_VK(vkCreateDevice);
    _IMP_VK(vkGetPhysicalDeviceFeatures2);
    _IMP_VK(vkGetDeviceQueue);

    _IMP_VK(vkAllocateMemory);
    _IMP_VK(vkFreeMemory);
    _IMP_VK(vkMapMemory);
    _IMP_VK(vkUnmapMemory);

    _IMP_VK(vkCreateShaderModule);
    _IMP_VK(vkCreateGraphicsPipelines);
    _IMP_VK(vkCreateComputePipelines);

    _IMP_VK(vkQueueSubmit);
    _IMP_VK(vkWaitForFences);
    _IMP_VK(vkResetFences);

    _IMP_VK(vkCreateBuffer);
    _IMP_VK(vkCreateImage);
    _IMP_VK(vkCreateImageView);
    _IMP_VK(vkCreateCommandPool);
    _IMP_VK(vkAllocateCommandBuffers);
    _IMP_VK(vkFreeCommandBuffers);

    _IMP_VK(vkCreateFence);
    _IMP_VK(vkCreateSemaphore);

    _IMP_VK(vkCmdPipelineBarrier);
    _IMP_VK(vkCmdClearColorImage);
    _IMP_VK(vkBeginCommandBuffer);
    _IMP_VK(vkEndCommandBuffer);
}

struct Program
{
    char* memory;
    GPULang::Loader loader;

    enum Shader
    {
        VertexShader,
        HullShader,
        DomainShader,
        GeometryShader,
        PixelShader,
        ComputeShader,
        TaskShader,
        MeshShader,
        RayGenShader,
        RayAnyHitShader,
        RayClosestHitShader,
        RayMissShader,
        RayIntersectionShader,
        RayCallableShader,

        NumShaders
    };

    VkShaderModule shaders[Shader::NumShaders];
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
    Program ret;

#define VK_INIT_SHADER(x, shader) \
if (prog->##x.binaryLength > 0) {\
    VkShaderModuleCreateInfo x##ShaderInfo =\
    {\
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,\
        .pNext = nullptr,\
        .flags = 0x0,\
        .codeSize = prog->##x.binaryLength,\
        .pCode = prog->##x.binary\
    };\
    VERIFY(vkCreateShaderModule(device, &##x##ShaderInfo, nullptr, &ret.shaders[Program::##shader]));\
}

    VK_INIT_SHADER(vs, VertexShader);
    VK_INIT_SHADER(hs, HullShader);
    VK_INIT_SHADER(ds, DomainShader);
    VK_INIT_SHADER(gs, GeometryShader);
    VK_INIT_SHADER(ps, PixelShader);
    VK_INIT_SHADER(cs, ComputeShader);
    VK_INIT_SHADER(ts, TaskShader);
    VK_INIT_SHADER(ms, MeshShader);
    VK_INIT_SHADER(rgs, RayGenShader);
    VK_INIT_SHADER(rahs, RayAnyHitShader);
    VK_INIT_SHADER(rchs, RayClosestHitShader);
    VK_INIT_SHADER(rms, RayMissShader);
    VK_INIT_SHADER(ris, RayIntersectionShader);
    VK_INIT_SHADER(rcs, RayCallableShader);

    fclose(file);

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

    VkPhysicalDeviceVulkan11Features vk11Features =
    {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .pNext = nullptr,
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

    std::vector<const char*> deviceFeatures = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    
    VkDeviceCreateInfo deviceInfo =
    {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        nullptr,
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

    Program program = LoadShader(device, "basicgraphics.gplb");

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

    GLFWwindow* window = glfwCreateWindow(1024, 768, "GPULang app", nullptr, nullptr);
    glfwSwapInterval(1);

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

    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (uint32_t i = 0; i < numPresentModes; i++)
    {
        switch (presentModes[i])
        {
            case VK_PRESENT_MODE_FIFO_KHR:
                numPresentModes = 0;
                break;
            default:
                continue;
        }
    }
    assert(numPresentModes == 0);

    VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    VkExtent2D swapchainExtent = { .width = 1024, .height = 768 };
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

    static bool terminate = false;

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            terminate = true;
    });

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

    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &presentFences[0]));
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &presentFences[1]));
    VERIFY(vkCreateFence(device, &fenceInfo, nullptr, &presentFences[2]));

    // Update loop
    VkCommandBuffer frameBuffers[3] = { VK_NULL_HANDLE };
    uint32_t frameBufferCounter = 0;

    float colorShift = 0.0f;
    while (!terminate)
    {
        glfwPollEvents();

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

        colorShift += 0.01f;
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
            .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = backbufferImages[frameBufferCounter],
            .subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
        };
        vkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0x0, 0, nullptr, 0, nullptr, 1, &imageBarrier);

        vkCmdClearColorImage(cmdBuf, backbufferImages[frameBufferCounter], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear, 1, &range);

        imageBarrier = 
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
            .dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = backbufferImages[frameBufferCounter],
            .subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
        };
        vkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0x0, 0, nullptr, 0, nullptr, 1, &imageBarrier);

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