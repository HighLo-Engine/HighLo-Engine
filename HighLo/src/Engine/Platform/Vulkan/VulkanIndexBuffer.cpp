// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanIndexBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Renderer/Renderer.h"

#include "VulkanContext.h"
#include "VulkanAllocator.h"

namespace highlo
{
	VulkanIndexBuffer::VulkanIndexBuffer(std::vector<int32> &indices)
	{
		m_Count = (uint32)indices.size();
		m_Size = m_Count * sizeof(int32);
		m_Buffer = Allocator::Copy(&indices[0], m_Size);

		Ref<VulkanIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			auto device = VulkanContext::GetCurrentDevice();
			VulkanAllocator allocator("IndexBuffer");

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

			VkBufferCreateInfo indexBufferCreateInfo = {};
			indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferCreateInfo.size = instance->m_Size;
			indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			instance->m_MemoryAllocation = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_VulkanBuffer);

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
			VkBufferCreateInfo indexbufferCreateInfo = {};
			indexbufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexbufferCreateInfo.size = instance->m_Size;
			indexbufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			auto bufferAlloc = allocator.AllocateBuffer(indexbufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);

			void *dstBuffer = allocator.MapMemory<void>(bufferAlloc);
			memcpy(dstBuffer, instance->m_Buffer.Data, instance->m_Size);
			allocator.UnmapMemory(bufferAlloc);
#endif
		});
	}

	VulkanIndexBuffer::VulkanIndexBuffer(void *data, uint32 size)
	{
		m_Buffer = Allocator::Copy(data, size);
		m_Size = size;
		m_Count = size / sizeof(int32);

		Ref<VulkanIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			auto device = VulkanContext::GetCurrentDevice();
			VulkanAllocator allocator("IndexBuffer");

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

			VkBufferCreateInfo indexBufferCreateInfo = {};
			indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferCreateInfo.size = instance->m_Size;
			indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			instance->m_MemoryAllocation = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_VulkanBuffer);

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
			VkBufferCreateInfo indexbufferCreateInfo = {};
			indexbufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexbufferCreateInfo.size = instance->m_Size;
			indexbufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			auto bufferAlloc = allocator.AllocateBuffer(indexbufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);

			void *dstBuffer = allocator.MapMemory<void>(bufferAlloc);
			memcpy(dstBuffer, instance->m_Buffer.Data, instance->m_Size);
			allocator.UnmapMemory(bufferAlloc);
#endif
		});
	}
	
	VulkanIndexBuffer::VulkanIndexBuffer(uint32 size)
	{
		m_Size = size;
		m_Count = m_Size / sizeof(int32);
		m_Buffer.Allocate(size);

		Ref<VulkanIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			auto device = VulkanContext::GetCurrentDevice();
			VulkanAllocator allocator("IndexBuffer");

			VkBufferCreateInfo indexBufferCreateInfo = {};
			indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferCreateInfo.size = instance->m_Size;
			indexBufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			instance->m_MemoryAllocation = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VulkanBuffer);
		});
	}
	
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		VkBuffer buffer = m_VulkanBuffer;
		VmaAllocation allocation = m_MemoryAllocation;
		Renderer::SubmitWithoutResources([buffer, allocation]()
		{
			VulkanAllocator allocator("IndexBuffer");
			allocator.DestroyBuffer(buffer, allocation);
		});

		m_Buffer.Release();
	}
	
	void VulkanIndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		HL_ASSERT(size < m_Buffer.Size);
		m_Size = size;
		m_Count = size / sizeof(int32);
		memcpy(m_Buffer.Data, (uint8*)data + offset, size);

		VulkanAllocator allocator("VulkanIndexBuffer");
		uint8 *pData = allocator.MapMemory<uint8>(m_MemoryAllocation);
		memcpy(pData, (uint8*)data + offset, size);
		allocator.UnmapMemory(m_MemoryAllocation);
	}
	
	void VulkanIndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
		uint32 size = (uint32)(indices.size() * sizeof(int32));
		HL_ASSERT(size < m_Buffer.Size);
		m_Size = size;
		m_Count = (uint32)indices.size();
		memcpy(m_Buffer.Data, (uint8*)((&indices[0]) + offset), size);

		VulkanAllocator allocator("VulkanIndexBuffer");
		uint8 *pData = allocator.MapMemory<uint8>(m_MemoryAllocation);
		memcpy(pData, (uint8*)((&indices[0]) + offset), size);
		allocator.UnmapMemory(m_MemoryAllocation);
	}
}

#endif // HIGHLO_API_VULKAN

