#pragma once

#include "Engine/Renderer/Shaders/StorageBuffer.h"

#include "Vulkan.h"
#include "VulkanAllocator.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanStorageBuffer : public StorageBuffer
	{
	public:

		VulkanStorageBuffer(uint32 size, uint32 binding);
		virtual ~VulkanStorageBuffer();

		virtual void SetData(const void *data, uint32 size, uint32 offset = 0) override;
		virtual void Resize(uint32 size) override;

		virtual uint32 GetBinding() const override { return m_Binding; }

	private:

		void Invalidate();
		void Release();

		uint32 m_Size = 0, m_Binding = 0;
		HLString m_Name;
		uint8 *m_LocalStorage = nullptr;

		VmaAllocation m_MemoryAlloc = nullptr;
		VkBuffer m_Buffer{};
		VkDescriptorBufferInfo m_DescriptorInfo{};
		VkShaderStageFlagBits m_ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};
}

#endif // HIGHLO_API_VULKAN

