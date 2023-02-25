// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanContext.h"

#define VULKAN_CONTEXT_LOG_PREFIX "Context>      "
#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#endif // HIGHLO_API_GLFW

namespace highlo
{
#if HL_DEBUG
    static bool s_Validate = true;
#else
    static bool s_Validate = false;
#endif // HL_DEBUG

    namespace utils
    {
        constexpr const char *VkDebugUtilsMessageType(const VkDebugUtilsMessageTypeFlagsEXT type)
        {
            switch (type)
            {
                case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:		return "General";
                case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:	return "Validation";
                case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:	return "Performance";
                default:												return "Unknown";
            }
        }

        constexpr const char *VkDebugUtilsMessageSeverity(const VkDebugUtilsMessageSeverityFlagBitsEXT severity)
        {
            switch (severity)
            {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:		return "error";
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:	return "warning";
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:		return "info";
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:	return "verbose";
                default:												return "unknown";
            }
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugUtilsMessengerCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, const VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
        {
            (void)pUserData; //Unused argument

            HLString labels = "";
            HLString objects = "";
            if (pCallbackData->cmdBufLabelCount)
            {
                labels = fmt::format("\tLabels({}): \n", pCallbackData->cmdBufLabelCount).c_str();
                for (uint32 i = 0; i < pCallbackData->cmdBufLabelCount; ++i)
                {
                    const auto &label = pCallbackData->pCmdBufLabels[i];
                    const HLString colorStr = fmt::format("[ {}, {}, {}, {} ]", label.color[0], label.color[1], label.color[2], label.color[3]).c_str();
                    labels.Append(fmt::format("\t\t- Command Buffer Label[{0}]: name: {1}, color: {2}\n", i, label.pLabelName ? label.pLabelName : "NULL", colorStr).c_str());
                }
            }

            if (pCallbackData->objectCount)
            {
                objects = fmt::format("\tObjects({}): \n", pCallbackData->objectCount).c_str();
                for (uint32 i = 0; i < pCallbackData->objectCount; ++i)
                {
                    const auto &object = pCallbackData->pObjects[i];
                    objects.Append(fmt::format("\t\t- Object[{0}] name: {1}, type: {2}, handle: {3:#x}\n", i, object.pObjectName ? object.pObjectName : "NULL", utils::VulkanObjectTypeToString(object.objectType), object.objectHandle).c_str());
                }
            }

            HL_CORE_WARN("{0} {1} message: \n\t{2}\n{3} {4}", VkDebugUtilsMessageType(messageType), VkDebugUtilsMessageSeverity(messageSeverity), pCallbackData->pMessage, *labels, *objects);
            HL_ASSERT(false, "temporary assert to fix all vulkan issues one by one");

            return VK_FALSE;
        }
    }

    VulkanContext::VulkanContext()
    {
    }
    
    VulkanContext::VulkanContext(void *handle, WindowData &data)
        : m_VulkanWindowHandle(handle), m_WindowData(data)
    {
    }

    VulkanContext::~VulkanContext()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyPipelineCache(device, m_PipelineCache, nullptr);

        m_Device->Destroy();

        vkDestroyInstance(s_VulkanInstance, nullptr);
        s_VulkanInstance = nullptr;
    }
    
    void VulkanContext::Init()
    {
        HL_CORE_INFO(VULKAN_CONTEXT_LOG_PREFIX "[+] VulkanContext::Init [+]");
        
    #ifdef HIGHLO_API_GLFW
        HL_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");
    #endif // HIGHLO_API_GLFW

        // Check driver api version support
        uint32 instanceVersion;
        vkEnumerateInstanceVersion(&instanceVersion);
        if (instanceVersion < VK_API_VERSION_1_3)
        {
            HL_CORE_FATAL(VULKAN_CONTEXT_LOG_PREFIX "[-] Your API version is too low, expected at least {0}, but got {1} [-]", VK_API_VERSION_1_3, instanceVersion);
            HL_ASSERT(false);
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "HighLo"; // TODO: Maybe retrieve the game/client program name here?
        appInfo.pEngineName = "HighLo";
        appInfo.apiVersion = VK_API_VERSION_1_3;

        std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
    #ifdef HL_DEBUG
        instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        instanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    #endif // HL_DEBUG

        VkValidationFeatureEnableEXT enables[] = { VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT };
        VkValidationFeaturesEXT features = {};
        features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
        features.enabledValidationFeatureCount = 1;
        features.pEnabledValidationFeatures = enables;

        VkInstanceCreateInfo instanceCreation = {};
        instanceCreation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreation.pNext = nullptr;
        instanceCreation.pApplicationInfo = &appInfo;
        instanceCreation.enabledExtensionCount = (uint32)instanceExtensions.size();
        instanceCreation.ppEnabledExtensionNames = instanceExtensions.data();

        if (s_Validate)
        {
            // Check if this layer is available at instance level
            const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
            bool validationLayerPresent = false;

            uint32 instanceLayerCount;
            vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);

            std::vector<VkLayerProperties> layerProperties(instanceLayerCount);
            vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());

            HL_CORE_TRACE(VULKAN_CONTEXT_LOG_PREFIX "[+] Vulkan Instance Layers: [+]");
            for (const VkLayerProperties &layer : layerProperties)
            {
                HL_CORE_TRACE(VULKAN_CONTEXT_LOG_PREFIX "[+]    {0} [+]", layer.layerName);
                if (strcmp(layer.layerName, validationLayerName) == 0)
                {
                    validationLayerPresent = true;
                    break;
                }
            }

            if (validationLayerPresent)
            {
                instanceCreation.ppEnabledLayerNames = &validationLayerName;
                instanceCreation.enabledLayerCount = 1;
            }
            else
            {
                HL_CORE_ERROR(VULKAN_CONTEXT_LOG_PREFIX "[-] Validation layer {0} not present! Validation is disabled! [-]", validationLayerName);
            }
        }

        // Instance and surface creation
        VK_CHECK_RESULT(vkCreateInstance(&instanceCreation, nullptr, &s_VulkanInstance));
        utils::LoadDebugExtensions(s_VulkanInstance);

        if (s_Validate)
        {
            auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_VulkanInstance, "vkCreateDebugUtilsMessengerEXT");
            HL_ASSERT(vkCreateDebugUtilsMessengerEXT != NULL);

            VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo = {};
            debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugUtilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugUtilsCreateInfo.pfnUserCallback = utils::VulkanDebugUtilsMessengerCallback;
            debugUtilsCreateInfo.messageSeverity = 
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT /*| 
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    | 
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT*/;

            VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(s_VulkanInstance, &debugUtilsCreateInfo, nullptr, &m_DebugUtilsMessenger));
        }

        Ref<PhysicalDevice> physicalDevice = PhysicalDevice::Create();
        m_PhysicalDevice = physicalDevice.As<VulkanPhysicalDevice>();

        VkPhysicalDeviceFeatures enabledFeatures = {};
        memset(&enabledFeatures, 0, sizeof(VkPhysicalDeviceFeatures));
        enabledFeatures.samplerAnisotropy = true;
        enabledFeatures.wideLines = true;
        enabledFeatures.fillModeNonSolid = true;
        enabledFeatures.independentBlend = true;
        enabledFeatures.pipelineStatisticsQuery = true;
        
        // TODO: If we move the enabled Features into the physical device,
        // we can delete this InitDevice function entirely and move its content into the constructor
        Ref<Device> device = Device::Create(physicalDevice);
        m_Device = device.As<VulkanDevice>();

        m_Device->InitDevice(enabledFeatures);

        // Pipeline cache
        VkPipelineCacheCreateInfo pipelineCreate = {};
        pipelineCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        VK_CHECK_RESULT(vkCreatePipelineCache(m_Device->GetNativeDevice(), &pipelineCreate, nullptr, &m_PipelineCache));
    }
    
    // All of this below is handled by the SwapChain implementation
    void VulkanContext::SwapBuffers()
    {
        // TODO
    }
    
    void VulkanContext::MakeCurrent()
    {
        // TODO
    }
    
    void VulkanContext::SetSwapInterval(bool bEnabled)
    {
        // TODO
    }
    
    void *VulkanContext::GetCurrentContext()
    {
        // TODO
        return nullptr;
    }
}

#endif // HIGHLO_API_VULKAN

