// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanIndexBuffer.h"

#ifdef HIGHLO_API_VULKAN

#define USE_STAGING 1

#include "VulkanContext.h"

namespace highlo
{
	VulkanIndexBuffer::VulkanIndexBuffer(std::vector<int32> &indices)
	{
		m_Size = indices.size() * sizeof(int32);
		m_LocalData = Allocator::Copy(indices.data(), m_Size);

		Invalidate();
	}
	
	VulkanIndexBuffer::VulkanIndexBuffer(void *data, uint32 size)
	{
		m_LocalData = Allocator::Copy(data, size);
		m_Size = size;

		Invalidate();
	}
	
	VulkanIndexBuffer::VulkanIndexBuffer(uint32 size)
		: m_Size(size)
	{
	}
	
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		VkBuffer buffer = m_VulkanBuffer;
		VmaAllocation allocation = m_MemoryAllocation;

		VulkanAllocator allocator("IndexBuffer");
		allocator.DestroyBuffer(buffer, allocation);

		m_LocalData.Release();
	}
	
	void VulkanIndexBuffer::Bind() const
	{
	}
	
	void VulkanIndexBuffer::Unbind() const
	{
	}
	
	void VulkanIndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
	}
	
	void VulkanIndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
	}
	
	void VulkanIndexBuffer::Invalidate()
	{
		auto device = VulkanContext::GetCurrentDevice();
		VulkanAllocator allocator("IndexBuffer");

	#ifdef USE_STAGING

		// Create staging buffer
		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_Size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		// Create index buffer
		VkBufferCreateInfo indexBufferCreateInfo = {};
		indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferCreateInfo.size = m_Size;
		indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		m_MemoryAllocation = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_VulkanBuffer);

		// Copy staging buffer into the index data
		VkCommandBuffer copyCmd = device->CreateCommandBuffer(true);

		VkBufferCopy copyRegion = {};
		copyRegion.size = m_LocalData.m_Size;

		vkCmdCopyBuffer(copyCmd, stagingBuffer, m_VulkanBuffer, 1, &copyRegion);

		device->FlushCommandBuffer(copyCmd);
		allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);

	#else

		// Create index buffer
		VkBufferCreateInfo indexBufferCreateInfo = {};
		indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferCreateInfo.size = m_Size;
		indexBufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		auto bufferAlloc = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_VulkanBuffer);

		void *dstBuffer = allocator.MapMemory<void>(bufferAlloc);
		memcpy(dstBuffer, m_LocalData.m_Data, m_Size);
		allocator.UnmapMemory(bufferAlloc);

	#endif // USE_STAGING
	}
}

#endif // HIGHLO_API_VULKAN