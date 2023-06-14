// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/StorageBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace highlo
{
	class VulkanStorageBuffer : public StorageBuffer
	{
	public:

		VulkanStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());
		virtual ~VulkanStorageBuffer();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void UploadToShader() override;
		virtual void Resize(uint32 size) override;

		// Vulkan-specific
		const VkDescriptorBufferInfo &GetDescriptorBufferInfo() const { return m_DescriptorBufferInfo; }

	private:

		// TODO: Make this part of the api
		void Release();
		void Invalidate();

	private:

		uint32 m_Size;

		VmaAllocation m_MemoryAllocation = nullptr;
		VkBuffer m_Buffer = {};
		VkDescriptorBufferInfo m_DescriptorBufferInfo = {};
	};
}

#endif // HIGHLO_API_VULKAN

