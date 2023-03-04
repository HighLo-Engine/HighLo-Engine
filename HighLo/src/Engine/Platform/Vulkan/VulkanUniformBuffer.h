// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/Shaders/UniformBuffer.h"
#include "Vulkan.h"
#include <vk_mem_alloc.h>

namespace highlo
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:

		VulkanUniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout);
		virtual ~VulkanUniformBuffer();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void UploadToShader() override;

		// Vulkan-specific
		const VkDescriptorBufferInfo &GetDescriptorBufferInfo() const { return m_DescriptorInfo; }

	private:

		void RT_Invalidate();
		void Release();

	private:

		uint32 m_Size = 0;
		HLString m_Name;

		VkBuffer m_Buffer;
		VmaAllocation m_MemoryAlloc = nullptr;
		VkDescriptorBufferInfo m_DescriptorInfo{};
		VkShaderStageFlagBits m_ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	};
}

#endif // HIGHLO_API_VULKAN

