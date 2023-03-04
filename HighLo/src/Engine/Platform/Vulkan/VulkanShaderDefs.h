// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace highlo
{
	struct VulkanShaderMaterialDescriptorSet
	{
		VkDescriptorPool Pool = nullptr;
		std::vector<VkDescriptorSet> Descriptors;
	};

	struct VulkanShaderUniformBuffer
	{
		VkDescriptorBufferInfo Descriptor;
		uint32 Size = 0;
		uint32 BindingPoint = 0;
		HLString Name;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct VulkanShaderStorageBuffer
	{
		VkDescriptorBufferInfo Descriptor;
		uint32 Size = 0;
		uint32 BindingPoint = 0;
		HLString Name;
		VmaAllocation Allocation = nullptr;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct VulkanShaderImageSampler
	{
		uint32 BindingPoint = 0;
		uint32 DescriptorSet = 0;
		uint32 ArraySize = 0;
		HLString Name;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct VulkanShaderPushConstantRange
	{
		uint32 Offset = 0;
		uint32 Size = 0;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct VulkanShaderDescriptorSet
	{
		std::unordered_map<uint32, VulkanShaderUniformBuffer*> UniformBuffers;
		std::unordered_map<uint32, VulkanShaderStorageBuffer*> StorageBuffers;
		std::unordered_map<uint32, VulkanShaderImageSampler> ImageSamplers;
		std::unordered_map<uint32, VulkanShaderImageSampler> StorageImages;
		std::unordered_map<uint32, VulkanShaderImageSampler> SeparateTextures;
		std::unordered_map<uint32, VulkanShaderImageSampler> SeparateSamplers;

		std::unordered_map<HLString, VkWriteDescriptorSet> WriteDescriptorSets;

		operator bool() const
		{ 
			return !(StorageBuffers.empty() 
					 && UniformBuffers.empty() 
					 && ImageSamplers.empty() 
					 && StorageImages.empty());
		}
	};
}

#endif // HIGHLO_API_VULKAN

