// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Vulkan.h"
#include "Engine/Renderer/Shaders/UniformBuffer.h"

#include "Vulkan.h"
#include "VulkanAllocator.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:

		VulkanUniformBuffer(uint32 size, uint32 binding);
		virtual ~VulkanUniformBuffer();

		virtual void SetData(const void *data, uint32 size, uint32 offset = 0) override;
		virtual uint32 GetBinding() const override { return m_Binding; }

		// Vulkan-specific
		const VkDescriptorBufferInfo &GetDescriptorBufferInfo() const { return m_DescriptorInfo; }

	private:

		void Invalidate();
		void Release();

		uint32 m_Size = 0, m_Binding = 0;

		HLString m_Name;
		uint8 *m_LocalStorage = nullptr;

		VmaAllocation m_MemoryAlloc = nullptr;
		VkBuffer m_Buffer;
		VkDescriptorBufferInfo m_DescriptorInfo{};
		VkShaderStageFlagBits m_ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};
}

#endif // HIGHLO_API_VULKAN

