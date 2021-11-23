// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<Vertex> &vertices, VertexBufferUsage usage)
	{
		m_Usage = usage;
		m_Size = (uint32)(vertices.size() * sizeof(Vertex));
		m_LocalData = Allocator::Copy(&vertices[0], m_Size);

		Ref<VulkanVertexBuffer> instance = this;
		Renderer::Submit([instance]()
		{

		});
	}
	
	VulkanVertexBuffer::VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
	{
		m_Usage = usage;
		m_LocalData = Allocator::Copy(data, size);
		m_Size = size;

		Ref<VulkanVertexBuffer> instance = this;
		Renderer::Submit([instance]()
		{

		});
	}
	
	VulkanVertexBuffer::VulkanVertexBuffer(uint32 size, VertexBufferUsage usage)
	{
		m_Usage = usage;
		m_Size = size;
		m_LocalData.Allocate(size);

		Ref<VulkanVertexBuffer> instance = this;
		Renderer::Submit([instance]()
		{

		});
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}
	
	void VulkanVertexBuffer::Bind() const
	{
	}
	
	void VulkanVertexBuffer::Unbind() const
	{
	}
	
	const BufferLayout &VulkanVertexBuffer::GetLayout() const
	{
	}
	
	void VulkanVertexBuffer::SetLayout(const BufferLayout &layout)
	{
	}
	
	void VulkanVertexBuffer::UpdateContents(std::vector<Vertex> &vertices, uint32 offset)
	{
	}
	
	void VulkanVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
	}
}

#endif // HIGHLO_API_VULKAN

