// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanAllocator.h"

namespace highlo
{
	VulkanVertexBuffer::VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		m_Buffer = Allocator::Copy(data, size);

		Ref<VulkanVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			auto device = VulkanContext::GetCurrentDevice();
			VulkanAllocator allocator("VertexBuffer");

#define USE_STAGING 1
#if USE_STAGING
			VkBufferCreateInfo bufferCreateInfo{};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.size = instance->m_Size;
			bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			VkBuffer stagingBuffer;
			VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			// Copy data to staging buffer
			uint8 *destData = allocator.MapMemory<uint8>(stagingBufferAllocation);
			memcpy(destData, instance->m_Buffer.Data, instance->m_Buffer.Size);
			allocator.UnmapMemory(stagingBufferAllocation);

			VkBufferCreateInfo vertexBufferCreateInfo = {};
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferCreateInfo.size = instance->m_Size;
			vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			instance->m_MemoryAllocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_VulkanBuffer);

			VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

			VkBufferCopy copyRegion = {};
			copyRegion.size = instance->m_Buffer.Size;
			vkCmdCopyBuffer(
				copyCmd,
				stagingBuffer,
				instance->m_VulkanBuffer,
				1,
				&copyRegion);

			device->FlushCommandBuffer(copyCmd);

			allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
#else
			VkBufferCreateInfo vertexBufferCreateInfo = {};
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferCreateInfo.size = instance->m_Size;
			vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			auto bufferAlloc = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);

			void *dstBuffer = allocator.MapMemory<void>(bufferAlloc);
			memcpy(dstBuffer, instance->m_Buffer.Data, instance->m_Size);
			allocator.UnmapMemory(bufferAlloc);
#endif
		});
	}
	
	VulkanVertexBuffer::VulkanVertexBuffer(uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		m_Buffer.Allocate(size);

		Ref<VulkanVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			auto device = VulkanContext::GetCurrentDevice();
			VulkanAllocator allocator("VertexBuffer");

			VkBufferCreateInfo vertexBufferCreateInfo = {};
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferCreateInfo.size = instance->m_Size;
			vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			instance->m_MemoryAllocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);
		});
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		VkBuffer buffer = m_VulkanBuffer;
		VmaAllocation allocation = m_MemoryAllocation;
		Renderer::SubmitWithoutResources([buffer, allocation]()
		{
			VulkanAllocator allocator("VertexBuffer");
			allocator.DestroyBuffer(buffer, allocation);
		});

		m_Buffer.Release();
	}
	
	void VulkanVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		HL_ASSERT(size < m_Buffer.Size);
		m_Size = size;
		memcpy(m_Buffer.Data, (uint8*)data + offset, size);

		VulkanAllocator allocator("VulkanVertexBuffer");
		uint8 *pData = allocator.MapMemory<uint8>(m_MemoryAllocation);
		memcpy(pData, (uint8*)data + offset, size);
		allocator.UnmapMemory(m_MemoryAllocation);
	}
	
	void VulkanVertexBuffer::SetData(void *data, uint32 size)
	{
		HL_ASSERT(size < m_Buffer.Size);

		m_Size = size;
		memcpy(m_Buffer.Data, (uint8*)data, size);

		VulkanAllocator allocator("VulkanVertexBuffer");
		uint8 *pData = allocator.MapMemory<uint8>(m_MemoryAllocation);
		memcpy(pData, (uint8*)data, size);
		allocator.UnmapMemory(m_MemoryAllocation);
	}
}

#endif // HIGHLO_API_VULKAN

