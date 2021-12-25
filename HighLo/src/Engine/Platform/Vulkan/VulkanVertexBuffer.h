// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Core/Allocator.h"
#include "VulkanAllocator.h"

namespace highlo
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:

		VulkanVertexBuffer(const std::vector<Vertex> &vertices, VertexBufferUsage usage = VertexBufferUsage::Static);
		VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		VulkanVertexBuffer(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }
		virtual VertexBufferUsage GetUsage() override { return m_Usage; }

		// Vulkan-specific
		VkBuffer GetVulkanBuffer() const { return m_VulkanBuffer; }

	private:

		void Invalidate();

		uint32 m_Size = 0;
		Allocator m_LocalData;
		VertexBufferUsage m_Usage;
		HLRendererID m_RendererID = 0;

		VkBuffer m_VulkanBuffer = nullptr;
		VmaAllocation m_MemoryAllocation;
	};
}

#endif // HIGHLO_API_VULKAN