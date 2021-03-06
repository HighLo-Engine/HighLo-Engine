// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "VulkanDevice.h"
#include "VulkanContext.h"
#include "VulkanShaderDefs.h"

// Macro to get a procedure address based on a vulkan instance
#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                        \
{                                                                       \
	fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetInstanceProcAddr(inst, "vk"#entrypoint)); \
	HL_ASSERT(fp##entrypoint);                                     \
}

// Macro to get a procedure address based on a vulkan device
#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                           \
{                                                                       \
	fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetDeviceProcAddr(dev, "vk"#entrypoint));   \
	HL_ASSERT(fp##entrypoint);                                     \
}


inline PFN_vkSetDebugUtilsObjectNameEXT fpSetDebugUtilsObjectNameEXT;
inline PFN_vkCmdBeginDebugUtilsLabelEXT fpCmdBeginDebugUtilsLabelEXT;
inline PFN_vkCmdEndDebugUtilsLabelEXT fpCmdEndDebugUtilsLabelEXT;
inline PFN_vkCmdInsertDebugUtilsLabelEXT fpCmdInsertDebugUtilsLabelEXT;

inline PFN_vkCmdSetCheckpointNV fpCmdSetCheckpointNV;

// We do have this function inside vulkan, but as of this "https://vulkan.lunarg.com/issue/home?limit=10;q=;mine=false;org=false;khronos=false;lunarg=false;indie=false;status=new,open"
// it seems that the function is not part of the exported library files
inline PFN_vkGetQueueCheckpointDataNV fpGetQueueCheckpointDataNV;

namespace highlo::utils
{
	inline void LoadDebugExtensions(VkInstance instance)
	{
		fpGetQueueCheckpointDataNV = (PFN_vkGetQueueCheckpointDataNV)(vkGetInstanceProcAddr(instance, "vkGetQueueCheckpointDataNV"));
		if (!fpGetQueueCheckpointDataNV)
		{
			fpGetQueueCheckpointDataNV = [](VkQueue queue, uint32 *retrieveCount, VkCheckpointDataNV *data) -> void
			{
			};
		}

		fpSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
		if (!fpSetDebugUtilsObjectNameEXT)
		{
			fpSetDebugUtilsObjectNameEXT = [](VkDevice device, const VkDebugUtilsObjectNameInfoEXT *pNameInfo) -> VkResult
			{
				return VK_SUCCESS;
			};
		}

		fpCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
		if (!fpCmdBeginDebugUtilsLabelEXT)
		{
			fpCmdBeginDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT *pLabelInfo)
			{
			};
		}

		fpCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
		if (!fpCmdEndDebugUtilsLabelEXT)
		{
			fpCmdEndDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer)
			{
			};
		}

		fpCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));
		if (!fpCmdInsertDebugUtilsLabelEXT)
		{
			fpCmdInsertDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT *pLabelInfo)
			{
			};
		}

		fpCmdSetCheckpointNV = (PFN_vkCmdSetCheckpointNV)(vkGetInstanceProcAddr(instance, "vkCmdSetCheckpointNV"));
		if (!fpCmdSetCheckpointNV)
		{
			fpCmdSetCheckpointNV = [](VkCommandBuffer commandBuffer, const void *marker)
			{

			};
		}
	}

	void RetrieveDiagnosticCheckpoints();

	inline const char *VulkanStageToString(VkPipelineStageFlagBits flags)
	{
		switch (flags)
		{
			case VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT: return "VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT";
			case VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT: return "VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT";
		}

		HL_ASSERT(false);
		return nullptr;
	}

	inline const char *VulkanResultToString(VkResult result)
	{
		switch (result)
		{
			case VK_SUCCESS: return "VK_SUCCESS";
			case VK_NOT_READY: return "VK_NOT_READY";
			case VK_TIMEOUT: return "VK_TIMEOUT";
			case VK_EVENT_SET: return "VK_EVENT_SET";
			case VK_EVENT_RESET: return "VK_EVENT_RESET";
			case VK_INCOMPLETE: return "VK_INCOMPLETE";
			case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
			case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
			case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
			case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
			case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
			case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
			case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
			case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
			case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
			case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
			case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
			case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
			case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
			case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
			case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
			case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
			case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
			case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
			case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
			case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
			case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
			case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV";
			case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
			case VK_ERROR_NOT_PERMITTED_EXT: return "VK_ERROR_NOT_PERMITTED_EXT";
			case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
			case VK_THREAD_IDLE_KHR: return "VK_THREAD_IDLE_KHR";
			case VK_THREAD_DONE_KHR: return "VK_THREAD_DONE_KHR";
			case VK_OPERATION_DEFERRED_KHR: return "VK_OPERATION_DEFERRED_KHR";
			case VK_OPERATION_NOT_DEFERRED_KHR: return "VK_OPERATION_NOT_DEFERRED_KHR";
			case VK_PIPELINE_COMPILE_REQUIRED_EXT: return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
		}

		HL_ASSERT(false);
		return nullptr;
	}

	inline const char *VulkanObjectTypeToString(VkObjectType obj)
	{
		switch (obj)
		{
			case VK_OBJECT_TYPE_COMMAND_BUFFER: return "VK_OBJECT_TYPE_COMMAND_BUFFER";
			case VK_OBJECT_TYPE_PIPELINE: return "VK_OBJECT_TYPE_PIPELINE";
			case VK_OBJECT_TYPE_FRAMEBUFFER: return "VK_OBJECT_TYPE_FRAMEBUFFER";
			case VK_OBJECT_TYPE_IMAGE: return "VK_OBJECT_TYPE_IMAGE";
			case VK_OBJECT_TYPE_QUERY_POOL: return "VK_OBJECT_TYPE_QUERY_POOL";
			case VK_OBJECT_TYPE_RENDER_PASS: return "VK_OBJECT_TYPE_RENDER_PASS";
			case VK_OBJECT_TYPE_COMMAND_POOL: return "VK_OBJECT_TYPE_COMMAND_POOL";
			case VK_OBJECT_TYPE_PIPELINE_CACHE: return "VK_OBJECT_TYPE_PIPELINE_CACHE";
			case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR: return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR";
			case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV: return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV";
			case VK_OBJECT_TYPE_BUFFER: return "VK_OBJECT_TYPE_BUFFER";
			case VK_OBJECT_TYPE_BUFFER_VIEW: return "VK_OBJECT_TYPE_BUFFER_VIEW";
			case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT: return "VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT";
			case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT: return "VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT";
			case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR: return "VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR";
			case VK_OBJECT_TYPE_DESCRIPTOR_POOL: return "VK_OBJECT_TYPE_DESCRIPTOR_POOL";
			case VK_OBJECT_TYPE_DESCRIPTOR_SET: return "VK_OBJECT_TYPE_DESCRIPTOR_SET";
			case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT: return "VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT";
			case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE: return "VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE";
			case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT: return "VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT";
			case VK_OBJECT_TYPE_DEVICE: return "VK_OBJECT_TYPE_DEVICE";
			case VK_OBJECT_TYPE_DEVICE_MEMORY: return "VK_OBJECT_TYPE_DEVICE_MEMORY";
			case VK_OBJECT_TYPE_PIPELINE_LAYOUT: return "VK_OBJECT_TYPE_PIPELINE_LAYOUT";
			case VK_OBJECT_TYPE_DISPLAY_KHR: return "VK_OBJECT_TYPE_DISPLAY_KHR";
			case VK_OBJECT_TYPE_DISPLAY_MODE_KHR: return "VK_OBJECT_TYPE_DISPLAY_MODE_KHR";
			case VK_OBJECT_TYPE_PHYSICAL_DEVICE: return "VK_OBJECT_TYPE_PHYSICAL_DEVICE";
			case VK_OBJECT_TYPE_EVENT: return "VK_OBJECT_TYPE_EVENT";
			case VK_OBJECT_TYPE_FENCE: return "VK_OBJECT_TYPE_FENCE";
			case VK_OBJECT_TYPE_IMAGE_VIEW: return "VK_OBJECT_TYPE_IMAGE_VIEW";
			case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV: return "VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV";
			case VK_OBJECT_TYPE_INSTANCE: return "VK_OBJECT_TYPE_INSTANCE";
			case VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL: return "VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL";
			case VK_OBJECT_TYPE_QUEUE: return "VK_OBJECT_TYPE_QUEUE";
			case VK_OBJECT_TYPE_SAMPLER: return "VK_OBJECT_TYPE_SAMPLER";
			case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION: return "VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION";
			case VK_OBJECT_TYPE_SEMAPHORE: return "VK_OBJECT_TYPE_SEMAPHORE";
			case VK_OBJECT_TYPE_SHADER_MODULE: return "VK_OBJECT_TYPE_SHADER_MODULE";
			case VK_OBJECT_TYPE_SURFACE_KHR: return "VK_OBJECT_TYPE_SURFACE_KHR";
			case VK_OBJECT_TYPE_SWAPCHAIN_KHR: return "VK_OBJECT_TYPE_SWAPCHAIN_KHR";
			case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT: return "VK_OBJECT_TYPE_VALIDATION_CACHE_EXT";
			case VK_OBJECT_TYPE_UNKNOWN: return "VK_OBJECT_TYPE_UNKNOWN";
			case VK_OBJECT_TYPE_MAX_ENUM: return "VK_OBJECT_TYPE_MAX_ENUM";
		}

		HL_ASSERT(false);
		return nullptr;
	}

	inline static void VulkanCheckResult(VkResult result)
	{
		if (result != VK_SUCCESS)
		{
			HL_CORE_ERROR("VkResult is '{0}' in {1}:{2}", ::highlo::utils::VulkanResultToString(result), __FILE__, __LINE__);
			if (result == VK_ERROR_DEVICE_LOST)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(3s);
				::highlo::utils::RetrieveDiagnosticCheckpoints();
				::highlo::utils::DumpGPUInfos();
			}
			HL_ASSERT(result == VK_SUCCESS);
		}
	}

	inline static void SetDebugUtilsObjectName(const VkDevice &device, const VkObjectType obj, const HLString &name, const void *handle)
	{
		VkDebugUtilsObjectNameInfoEXT nameInfo;
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = obj;
		nameInfo.pObjectName = name.IsEmpty() ? "" : *name;
		nameInfo.objectHandle = (uint64)handle;
		nameInfo.pNext = VK_NULL_HANDLE;

		VkResult result = fpSetDebugUtilsObjectNameEXT(device, &nameInfo);
		VulkanCheckResult(result);
	}

	// Shader helpers

	static VkShaderStageFlagBits ShaderTypeToVulkanStage(ShaderType type)
	{
		switch (type)
		{
			case ShaderType::Vertex:		 return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderType::Fragment:		 return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderType::Compute:		 return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderType::TessControl:	 return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case ShaderType::TessEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			case ShaderType::Geometry:		 return VK_SHADER_STAGE_GEOMETRY_BIT;
		}

		HL_ASSERT(false);
		return VK_SHADER_STAGE_ALL;
	}

	static ShaderType VulkanStageToShaderType(const VkShaderStageFlagBits stage)
	{
		switch (stage)
		{
			case VK_SHADER_STAGE_VERTEX_BIT:				  return ShaderType::Vertex;
			case VK_SHADER_STAGE_FRAGMENT_BIT:				  return ShaderType::Fragment;
			case VK_SHADER_STAGE_COMPUTE_BIT:				  return ShaderType::Compute;
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:	  return ShaderType::TessControl;
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return ShaderType::TessEvaluation;
			case VK_SHADER_STAGE_GEOMETRY_BIT:				  return ShaderType::Geometry;
		}

		HL_ASSERT(false);
		return ShaderType::None;
	}

	static HLString ShaderStageToMacro(const VkShaderStageFlagBits stage)
	{
		switch (stage)
		{
			case VK_SHADER_STAGE_VERTEX_BIT:                    return "__VERTEX_STAGE__";
			case VK_SHADER_STAGE_FRAGMENT_BIT:                  return "__FRAGMENT_STAGE__";
			case VK_SHADER_STAGE_COMPUTE_BIT:                   return "__COMPUTE_STAGE__";
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:      return "__TESS_CONTROL_STAGE__";
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:   return "__TESS_EVAL_STAGE__";
			case VK_SHADER_STAGE_GEOMETRY_BIT:                  return "__GEOMETRY_STAGE__";
		}

		HL_ASSERT(false);
		return "";
	}

	static VkShaderStageFlagBits ShaderStageFromString(const HLString &str)
	{
		HLString type = str.ToLowerCase();

		if (type == "vertex")
			return VK_SHADER_STAGE_VERTEX_BIT;

		if (type == "fragment" || type == "pixel")
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		if (type == "compute")
			return VK_SHADER_STAGE_COMPUTE_BIT;

		if (type == "tesscontrol")
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

		if (type == "tessevaluation")
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

		if (type == "geometry")
			return VK_SHADER_STAGE_GEOMETRY_BIT;

		return VK_SHADER_STAGE_ALL;
	}

	static HLString ShaderStageStringToMacro(const HLString &str)
	{
		HLString type = str.ToLowerCase();

		if (type == "vertex")
			return "__VERTEX_STAGE__";

		if (type == "fragment" || type == "pixel")
			return "__FRAGMENT_STAGE__";

		if (type == "compute")
			return "__COMPUTE_STAGE__";

		if (type == "tesscontrol")
			return "__TESS_CONTROL_STAGE__";

		if (type == "tessevaluation")
			return "__TESS_EVAL_STAGE__";

		if (type == "geometry")
			return "__GEOMETRY_STAGE__";

		HL_ASSERT(false);
		return "";
	}

	static HLString ShaderStageToString(const VkShaderStageFlagBits stage)
	{
		switch (stage)
		{
			case VK_SHADER_STAGE_VERTEX_BIT:                    return "Vertex";
			case VK_SHADER_STAGE_FRAGMENT_BIT:                  return "Pixel";
			case VK_SHADER_STAGE_COMPUTE_BIT:                   return "Compute";
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:      return "TessControl";
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:   return "TessEvalulation";
			case VK_SHADER_STAGE_GEOMETRY_BIT:                  return "Geometry";
		}

		HL_ASSERT(false);
		return "";
	}

	static std::string_view VKStageToShaderMacro(const VkShaderStageFlagBits stage)
	{
		switch (stage)
		{
			case VK_SHADER_STAGE_VERTEX_BIT:					return "__VERTEX_STAGE__";
			case VK_SHADER_STAGE_FRAGMENT_BIT:					return "__FRAGMENT_STAGE__";
			case VK_SHADER_STAGE_COMPUTE_BIT:					return "__COMPUTE_STAGE__";
			case VK_SHADER_STAGE_GEOMETRY_BIT:					return "__GEOMETRY_STAGE__";
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:		return "__TESS_CONTROL_STAGE__";
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:	return "__TESS_EVAL_STAGE__";
		}

		HL_ASSERT(false, "Unknown shader stage.");
		return "";
	}

	static VkShaderStageFlagBits StageToVKShaderStage(const std::string_view stage)
	{
		if (stage == "vertex")		return VK_SHADER_STAGE_VERTEX_BIT;
		if (stage == "fragment")	return VK_SHADER_STAGE_FRAGMENT_BIT;
		if (stage == "compute")		return VK_SHADER_STAGE_COMPUTE_BIT;
		if (stage == "geometry")	return VK_SHADER_STAGE_GEOMETRY_BIT;
		if (stage == "tesscontrol") return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		if (stage == "tesseval")	return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		
		HL_ASSERT(false, "Unknown shader stage.");
		return VK_SHADER_STAGE_ALL;
	}

	static std::unordered_map<VkShaderStageFlagBits, HLString> ConvertShaderTypeToVulkanStage(const std::unordered_map<ShaderType, HLString> &sources)
	{
		std::unordered_map<VkShaderStageFlagBits, HLString> result;
		for (auto &[type, source] : sources)
		{
			VkShaderStageFlagBits stage = utils::ShaderTypeToVulkanStage(type);
			result.insert({ stage, source });
		}

		return result;
	}

	static std::unordered_map<ShaderType, HLString> ConvertVulkanStageToShaderType(const std::unordered_map<VkShaderStageFlagBits, HLString> &sources)
	{
		std::unordered_map<ShaderType, HLString> result;
		for (auto &[stage, source] : sources)
		{
			ShaderType type = utils::VulkanStageToShaderType(stage);
			result.insert({ type, source });
		}

		return result;
	}

	static std::vector<VkPushConstantRange> PushConstantRangeToVulkanPushConstantRange(const std::vector<VulkanShaderPushConstantRange> &pushConstants)
	{
		std::vector<VkPushConstantRange> result(pushConstants.size());
		for (uint32 i = 0; i < pushConstants.size(); ++i)
		{
			const auto &pushConstantRange = pushConstants[i];
			auto &entry = result[i];

			entry.stageFlags = pushConstantRange.Stage;
			entry.offset = pushConstantRange.Offset;
			entry.size = pushConstantRange.Size;
		}

		return result;
	}

	// Texture helpers

	static VkFormat VulkanTextureFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RED8UN:					return VK_FORMAT_R8_UNORM;
			case TextureFormat::RED8UI:					return VK_FORMAT_R8_UINT;
			case TextureFormat::RED16UI:				return VK_FORMAT_R16_UINT;
			case TextureFormat::RED32UI:				return VK_FORMAT_R32_UINT;
			case TextureFormat::RED32F:					return VK_FORMAT_R32_SFLOAT;
			case TextureFormat::RG8:				    return VK_FORMAT_R8G8_UNORM;
			case TextureFormat::RG16F:					return VK_FORMAT_R16G16_SFLOAT;
			case TextureFormat::RG32F:					return VK_FORMAT_R32G32_SFLOAT;
			case TextureFormat::RGBA:					return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA8:					return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA16F:				return VK_FORMAT_R16G16B16A16_SFLOAT;
			case TextureFormat::RGBA32F:				return VK_FORMAT_R32G32B32A32_SFLOAT;
			case TextureFormat::B10R11G11UF:			return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
			case TextureFormat::DEPTH32FSTENCIL8UINT:	return VK_FORMAT_D32_SFLOAT_S8_UINT;
			case TextureFormat::DEPTH32F:				return VK_FORMAT_D32_SFLOAT;
			case TextureFormat::DEPTH24STENCIL8:		return VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetDepthFormat();
		}

		HL_ASSERT(false);
		return VK_FORMAT_UNDEFINED;
	}

	static VkSamplerAddressMode VulkanSamplerWrap(TextureWrap wrap)
	{
		switch (wrap)
		{
			case TextureWrap::Clamp:    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case TextureWrap::Repeat:   return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}

		HL_ASSERT(false);
		return (VkSamplerAddressMode)0;
	}

	static VkFilter VulkanSamplerFilter(TextureFilter filter)
	{
		switch (filter)
		{
			case TextureFilter::Linear:     return VK_FILTER_LINEAR;
			case TextureFilter::Nearest:    return VK_FILTER_NEAREST;
			case TextureFilter::Cubic:      return VK_FILTER_CUBIC_IMG;
		}

		HL_ASSERT(false);
		return (VkFilter)0;
	}

	static void InsertImageMemoryBarrier(
		VkCommandBuffer cmdbuffer,
		VkImage image,
		VkAccessFlags srcAccessMask,
		VkAccessFlags dstAccessMask,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		VkImageSubresourceRange subresourceRange)
	{
		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.pNext = nullptr;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		imageMemoryBarrier.srcAccessMask = srcAccessMask;
		imageMemoryBarrier.dstAccessMask = dstAccessMask;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}

	static void SetImageLayout(
		VkCommandBuffer cmdbuffer,
		VkImage image,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkImageSubresourceRange subresourceRange,
		VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)
	{
		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.pNext = nullptr;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		switch (oldImageLayout)
		{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				imageMemoryBarrier.srcAccessMask = 0;
				break;

			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;

			default:
				break;
		}

		switch (newImageLayout)
		{
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				imageMemoryBarrier.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				if (imageMemoryBarrier.srcAccessMask == 0)
				{
					imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
				}
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;

			default:
				break;
		}

		vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}

	static void SetImageLayout(
		VkCommandBuffer cmdbuffer,
		VkImage image,
		VkImageAspectFlags aspectMask,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)
	{
		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = aspectMask;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;
		SetImageLayout(cmdbuffer, image, oldImageLayout, newImageLayout, subresourceRange, srcStageMask, dstStageMask);
	}


	// Allocation helpers

	void InitAllocator(const Ref<VulkanDevice> &device);
	void ShutdownAllocator();

	void FreeAllocation(VmaAllocation allocation);

	VmaAllocation AllocateBuffer(VkBufferCreateInfo createInfo, VmaMemoryUsage usage, VkBuffer &outBuffer);
	void DestroyBuffer(VkBuffer buffer, VmaAllocation allocation);

	VmaAllocation AllocateImage(VkImageCreateInfo createInfo, VmaMemoryUsage usage, VkImage &outImage);
	void DestroyImage(VkImage image, VmaAllocation allocation);

	VmaAllocator &GetVMAAllocator();

	template<typename T>
	static T *MapMemory(VmaAllocation allocation)
	{
		T *mappedMemory;
		vmaMapMemory(GetVMAAllocator(), allocation, (void**)&mappedMemory);
		return mappedMemory;
	}

	void UnmapMemory(VmaAllocation allocation);
}

#define VK_CHECK_RESULT(f)\
{\
	VkResult res = (f);\
	::highlo::utils::VulkanCheckResult(res);\
}

#endif // HIGHLO_API_VULKAN

