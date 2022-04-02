#pragma once

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace highlo
{
	struct ShaderMaterialDescriptorSet
	{
		VkDescriptorPool Pool = nullptr;
		std::vector<VkDescriptorSet> Descriptors;
	};

	struct ShaderUniformBuffer
	{
		VkDescriptorBufferInfo Descriptor;
		uint32 Size = 0;
		uint32 BindingPoint = 0;
		HLString Name;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct ShaderStorageBuffer
	{
		VkDescriptorBufferInfo Descriptor;
		uint32 Size = 0;
		uint32 BindingPoint = 0;
		HLString Name;
		VmaAllocation Allocation = nullptr;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct ShaderImageSampler
	{
		uint32 BindingPoint = 0;
		uint32 DescriptorSet = 0;
		uint32 ArraySize = 0;
		HLString Name;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct ShaderPushConstantRange
	{
		uint32 Offset = 0;
		uint32 Size = 0;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct ShaderDescriptorSet
	{
		std::unordered_map<uint32, ShaderUniformBuffer*> UniformBuffers;
		std::unordered_map<uint32, ShaderStorageBuffer*> StorageBuffers;
		std::unordered_map<uint32, ShaderImageSampler> ImageSamplers;
		std::unordered_map<uint32, ShaderImageSampler> StorageImages;
		std::unordered_map<uint32, ShaderImageSampler> SeparateTextures;
		std::unordered_map<uint32, ShaderImageSampler> SeparateSamplers;

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

