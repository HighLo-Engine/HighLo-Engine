// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanContext.h"

#include "VulkanAllocator.h"

#ifdef HIGHLO_API_VULKAN

#ifdef HL_DEBUG
static bool s_Validation = true;
#else
static bool s_Validation = false;
#endif

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#endif // HIGHLO_API_GLFW 

#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"

namespace highlo
{
	namespace utils
	{
		constexpr const char *VulkanDebugUtilsMessageType(const VkDebugUtilsMessageTypeFlagsEXT type)
		{
			switch (type)
			{
				case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:		return "General";
				case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:	return "Validation";
				case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:	return "Performance";
				default:												return "Unknown";
			}
		}

		constexpr const char *VulkanDebugUtilsMessageSeverity(const VkDebugUtilsMessageSeverityFlagBitsEXT severity)
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

			const bool performanceWarnings = false;
			if (!performanceWarnings)
			{
				if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
					return VK_FALSE;
			}

			HLString labels, objects;
			if (pCallbackData->cmdBufLabelCount)
			{
				labels = fmt::format("\tLabels({}): \n", pCallbackData->cmdBufLabelCount);
				for (uint32 i = 0; i < pCallbackData->cmdBufLabelCount; ++i)
				{
					const auto &label = pCallbackData->pCmdBufLabels[i];
					const HLString colorStr = fmt::format("[ {}, {}, {}, {} ]", label.color[0], label.color[1], label.color[2], label.color[3]);
					labels.Append(fmt::format("\t\t- Command Buffer Label[{0}]: name: {1}, color: {2}\n", i, label.pLabelName ? label.pLabelName : "NULL", colorStr));
				}
			}

			if (pCallbackData->objectCount)
			{
				objects = fmt::format("\tObjects({}): \n", pCallbackData->objectCount);
				for (uint32 i = 0; i < pCallbackData->objectCount; ++i)
				{
					const auto &object = pCallbackData->pObjects[i];
					objects.Append(fmt::format("\t\t- Object[{0}] name: {1}, type: {2}, handle: {3:#x}\n", i, object.pObjectName ? object.pObjectName : "NULL", utils::VulkanObjectTypeToString(object.objectType), object.objectHandle));
				}
			}

			HL_CORE_WARN("{0} {1} message: \n\t{2}\n {3} {4}", VulkanDebugUtilsMessageType(messageType), VulkanDebugUtilsMessageSeverity(messageSeverity), pCallbackData->pMessage, labels, objects);
			
			return VK_FALSE;
		}

		static bool CheckDriverAPIVersionSupport(uint32 minimumSupportedVersion)
		{
			uint32 instanceVersion;
			vkEnumerateInstanceVersion(&instanceVersion);

			if (instanceVersion < minimumSupportedVersion)
			{
				HL_CORE_FATAL("Incompatible Vulkan driver version!");
				HL_CORE_FATAL("  You have {}.{}.{}", VK_API_VERSION_MAJOR(instanceVersion), VK_API_VERSION_MINOR(instanceVersion), VK_API_VERSION_PATCH(instanceVersion));
				HL_CORE_FATAL("  You need at least {}.{}.{}", VK_API_VERSION_MAJOR(minimumSupportedVersion), VK_API_VERSION_MINOR(minimumSupportedVersion), VK_API_VERSION_PATCH(minimumSupportedVersion));

				return false;
			}

			return true;
		}
	}

	VulkanContext::VulkanContext(void *handle, WindowData &data)
	{
	}
	
	VulkanContext::~VulkanContext()
	{
		VulkanAllocator::Shutdown();

		vkDestroyInstance(s_VulkanInstance, nullptr);
		s_VulkanInstance = nullptr;
	}
	
	void VulkanContext::Init()
	{
		HL_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

		if (!utils::CheckDriverAPIVersionSupport(VK_API_VERSION_1_2))
		{
			HL_ASSERT(false);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Application Info
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = HLApplication::Get().GetApplicationSettings().WindowTitle; // TODO: Replace this with new name property
		appInfo.pEngineName = "HighLo";
		appInfo.apiVersion = VK_API_VERSION_1_2;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Extensions and Validation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// TODO: move the VK_KHR_WIN32_SURFACE_EXTENSION_NAME to the windows platform layer, otherwise other platforms will not work!
		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Very little performance hit, can be used in Release.
		if (s_Validation)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			instanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		}

		VkValidationFeatureEnableEXT enables[] = { VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT };
		VkValidationFeaturesEXT features = {};
		features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		features.enabledValidationFeatureCount = 1;
		features.pEnabledValidationFeatures = enables;

		VkInstanceCreateInfo instanceCreateInfo = {};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = nullptr; // &features;
		instanceCreateInfo.pApplicationInfo = &appInfo;
		instanceCreateInfo.enabledExtensionCount = (uint32)instanceExtensions.size();
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

		if (s_Validation)
		{
			const char *validationLayerName = "VK_LAYER_KHRONOS_validation";
			// Check if this layer is available at instance level
			uint32 instanceLayerCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			
			std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
			
			bool validationLayerPresent = false;
			HL_CORE_TRACE("Vulkan Instance Layers:");
			for (const VkLayerProperties &layer : instanceLayerProperties)
			{
				HL_CORE_TRACE("  {0}", layer.layerName);
				if (strcmp(layer.layerName, validationLayerName) == 0)
				{
					validationLayerPresent = true;
					break;
				}
			}
			if (validationLayerPresent)
			{
				instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
				instanceCreateInfo.enabledLayerCount = 1;
			}
			else
			{
				HL_CORE_TRACE("Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Instance and Surface Creation
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &s_VulkanInstance));
		utils::VulkanLoadDebugUtilsExtensions(s_VulkanInstance);

		if (s_Validation)
		{
			auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_VulkanInstance, "vkCreateDebugUtilsMessengerEXT");
			HL_ASSERT(vkCreateDebugUtilsMessengerEXT != NULL, "");

			VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo{};
			debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugUtilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugUtilsCreateInfo.pfnUserCallback = utils::VulkanDebugUtilsMessengerCallback;
			debugUtilsCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT 
				/*  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
					| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT*/;

			VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(s_VulkanInstance, &debugUtilsCreateInfo, nullptr, &m_DebugUtilsMessenger));
		}

		m_PhysicalDevice = VulkanPhysicalDevice::Create();

		VkPhysicalDeviceFeatures enabledFeatures = { 0 };
		enabledFeatures.samplerAnisotropy = true;
		enabledFeatures.wideLines = true;
		enabledFeatures.fillModeNonSolid = true;
		enabledFeatures.independentBlend = true;
		enabledFeatures.pipelineStatisticsQuery = true;
		m_Device = Ref<VulkanDevice>(new VulkanDevice(m_PhysicalDevice, enabledFeatures));

		VulkanAllocator::Init(m_Device);

		// Pipeline Cache
		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VK_CHECK_RESULT(vkCreatePipelineCache(m_Device->GetVulkanDevice(), &pipelineCacheCreateInfo, nullptr, &m_PipelineCache));
	}
}

#endif // HIGHLO_API_VULKAN

