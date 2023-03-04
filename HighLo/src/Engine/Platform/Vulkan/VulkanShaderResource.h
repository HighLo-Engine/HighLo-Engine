// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#include "Vulkan.h"
#include <vk_mem_alloc.h>

namespace highlo::VkShaderResource
{
	struct UniformBuffer
	{
		VkDescriptorBufferInfo Descriptor;
		uint32 Size = 0;
		uint32 BindingPoint = 0;
		HLString Name;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct StorageBuffer
	{
		VmaAllocation MemoryAlloc = nullptr;
		VkDescriptorBufferInfo Descriptor;
		uint32 Size = 0;
		uint32 BindingPoint = 0;
		HLString Name;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct ImageSampler
	{
		uint32 BindingPoint = 0;
		uint32 DescriptorSet = 0;
		uint32 ArraySize = 0;
		HLString Name;
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};

	struct PushConstantRange
	{
		VkShaderStageFlagBits Stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		uint32 Offset = 0;
		uint32 Size = 0;
	};

	struct ShaderDescriptorSet
	{
		std::unordered_map<uint32, UniformBuffer> UniformBuffers;
		std::unordered_map<uint32, StorageBuffer> StorageBuffers;
		std::unordered_map<uint32, ImageSampler> ImageSamplers;
		std::unordered_map<uint32, ImageSampler> StorageImages;
		std::unordered_map<uint32, ImageSampler> SeparateTextures; // Not really an image sampler.
		std::unordered_map<uint32, ImageSampler> SeparateSamplers;

		std::unordered_map<HLString, VkWriteDescriptorSet> WriteDescriptorSets;

		operator bool() const { return !(StorageBuffers.empty() && UniformBuffers.empty() && ImageSamplers.empty() && StorageImages.empty()); }
	};
}

