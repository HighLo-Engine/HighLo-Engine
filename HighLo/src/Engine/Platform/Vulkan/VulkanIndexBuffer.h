// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "VulkanAllocator.h"

#include "Engine/Core/Allocator.h"

namespace highlo
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:

		VulkanIndexBuffer(std::vector<int32> &indices);
		VulkanIndexBuffer(void *data, uint32 size);
		VulkanIndexBuffer(uint32 size);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32 GetCount() override { return m_Size / sizeof(uint32); }
		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;
		virtual void UpdateContents(std::vector<int32> &indices, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }

	private:

		HLRendererID m_RendererID = 0;
		uint32 m_Size = 0;

		VkBuffer m_VulkanBuffer = nullptr;
		VmaAllocation m_MemoryAllocation;
		
		Allocator m_LocalData;
	};
}

#endif // HIGHLO_API_VULKAN

