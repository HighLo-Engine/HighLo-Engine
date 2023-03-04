// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/IndexBuffer.h"

#include "Vulkan.h"
#include <vk_mem_alloc.h>

namespace highlo
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:

		VulkanIndexBuffer(std::vector<int32> &indices);
		VulkanIndexBuffer(void *data, uint32 size);
		VulkanIndexBuffer(uint32 size);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual uint32 GetCount() override { return m_Count; }
		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;
		virtual void UpdateContents(std::vector<int32> &indices, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }

		// Vulkan-specific
		VkBuffer GetVulkanBuffer() { return m_VulkanBuffer; }

	private:

		HLRendererID m_RendererID = 0;
		Allocator m_Buffer;
		uint32 m_Size = 0;
		uint32 m_Count = 0;

		VkBuffer m_VulkanBuffer = nullptr;
		VmaAllocation m_MemoryAllocation;
	};
}

#endif // HIGHLO_API_VULKAN

