// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexBuffer.h"

#include "VulkanContext.h"

#define USE_STAGING 1

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<Vertex> &vertices, VertexBufferUsage usage)
	{
		m_Usage = usage;
		m_Size = (uint32)(vertices.size() * sizeof(Vertex));
		m_LocalData = Allocator::Copy(&vertices[0], m_Size);

		Invalidate();
	}
	
	VulkanVertexBuffer::VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
	{
		m_Usage = usage;
		m_Size = size;
		m_LocalData = Allocator::Copy(data, m_Size);

		Invalidate();
	}
	
	VulkanVertexBuffer::VulkanVertexBuffer(uint32 size, VertexBufferUsage usage)
	{
		m_Usage = usage;
		m_Size = size;
		m_LocalData.Allocate(size);

		auto device = VulkanContext::GetCurrentDevice();
		VulkanAllocator allocator("VertexBuffer");

		VkBufferCreateInfo vertexBufferCreateInfo = {};
		vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferCreateInfo.size = m_Size;
		vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		m_MemoryAllocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_VulkanBuffer);
		m_LocalData.Allocate(m_Size);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		VkBuffer buffer = m_VulkanBuffer;
		VmaAllocation allocation = m_MemoryAllocation;

		VulkanAllocator allocator("VertexBuffer");
		allocator.DestroyBuffer(buffer, allocation);

		m_LocalData.Release();
	}
	
	void VulkanVertexBuffer::Bind() const
	{
	}
	
	void VulkanVertexBuffer::Unbind() const
	{
	}
	
	void VulkanVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
	}
	
	void VulkanVertexBuffer::Invalidate()
	{
		auto device = VulkanContext::GetCurrentDevice();
		VulkanAllocator allocator("VertexBuffer");

	#if USE_STAGING
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_Size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		// Copy data to staging buffer
		uint8 *destData = allocator.MapMemory<uint8>(stagingBufferAllocation);
		memcpy(destData, m_LocalData.m_Data, m_LocalData.m_Size);
		allocator.UnmapMemory(stagingBufferAllocation);

		VkBufferCreateInfo vertexBufferCreateInfo = {};
		vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferCreateInfo.size = m_Size;
		vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		m_MemoryAllocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_VulkanBuffer);

		VkCommandBuffer copyCmd = device->CreateCommandBuffer(true);

		VkBufferCopy copyRegion = {};
		copyRegion.size = m_LocalData.m_Size;
		vkCmdCopyBuffer(copyCmd, stagingBuffer, m_VulkanBuffer, 1, &copyRegion);

		device->FlushCommandBuffer(copyCmd);
		allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	#else
		VkBufferCreateInfo vertexBufferCreateInfo = {};
		vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferCreateInfo.size = m_Size;
		vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		auto bufferAlloc = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_VulkanBuffer);

		void *dstBuffer = allocator.MapMemory<void>(bufferAlloc);
		memcpy(dstBuffer, m_LocalData.m_Data, m_Size);
		allocator.UnmapMemory(bufferAlloc);
	#endif
	}
}

#endif // HIGHLO_API_VULKAN

