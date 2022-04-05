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

		VulkanStorageBuffer(uint32 size, uint32 binding);
		virtual ~VulkanStorageBuffer();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void SetData(const void *data, uint32 size, uint32 offset = 0) override;
		virtual void Resize(uint32 size) override;

		virtual uint32 GetBinding() const override { return m_Binding; }

		// Vulkan-specific
		const VkDescriptorBufferInfo &GetDescriptorBufferInfo() const { return m_DescriptorBufferInfo; }

	private:

		// TODO: Make this part of the api
		void Release();
		void Invalidate();

	private:

		uint32 m_Size;
		uint32 m_Binding;

		VmaAllocation m_MemoryAllocation = nullptr;
		VkBuffer m_Buffer = {};
		VkDescriptorBufferInfo m_DescriptorBufferInfo = {};
	};
}

#endif // HIGHLO_API_VULKAN

