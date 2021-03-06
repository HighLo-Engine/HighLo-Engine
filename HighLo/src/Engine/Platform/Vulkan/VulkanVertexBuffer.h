// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-22) initial release
//

#pragma once

#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Core/Allocator.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace highlo
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:

		VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		VulkanVertexBuffer(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }
		virtual VertexBufferUsage GetUsage() override { return m_Usage; }

		// Vulkan-specific
		VkBuffer GetVulkanBuffer() const { return m_VulkanBuffer; }

	private:

		HLRendererID m_RendererID = 0;
		uint32 m_Size = 0;
		VertexBufferUsage m_Usage;
		Allocator m_LocalData;

		VkBuffer m_VulkanBuffer = nullptr;
		VmaAllocation m_MemoryAllocation;
	};
}

#endif // HIGHLO_API_VULKAN

