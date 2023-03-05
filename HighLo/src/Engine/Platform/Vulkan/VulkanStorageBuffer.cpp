// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanStorageBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Renderer/Renderer.h"

#include "VulkanContext.h"
#include "VulkanAllocator.h"

namespace highlo
{
	VulkanStorageBuffer::VulkanStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: StorageBuffer(binding, layout), m_Size(size)
	{
		Ref<VulkanStorageBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->RT_Invalidate();
		});
	}

	VulkanStorageBuffer::~VulkanStorageBuffer()
	{
		Release();
	}
	
	void VulkanStorageBuffer::UploadToShader()
	{
		VulkanAllocator allocator("Staging");

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.size = m_Size;

		VkBuffer stagingBuffer;
		auto stagingAlloc = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		uint8 *pData = allocator.MapMemory<uint8>(stagingAlloc);
		memcpy(pData, m_Data, m_Size);
		allocator.UnmapMemory(stagingAlloc);

		{
			VkCommandBuffer commandBuffer = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true);

			VkBufferCopy copyRegion = {};
			copyRegion.dstOffset = 0;
			copyRegion.srcOffset = 0;
			copyRegion.size = m_Size;

			vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_Buffer, 1, &copyRegion);

			VulkanContext::GetCurrentDevice()->FlushCommandBuffer(commandBuffer);
		}

		allocator.DestroyBuffer(stagingBuffer, stagingAlloc);
	}
	
	void VulkanStorageBuffer::Resize(uint32 size)
	{
		m_Size = size;

		Ref<VulkanStorageBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->RT_Invalidate();
		});
	}

	void VulkanStorageBuffer::RT_Invalidate()
	{
		Release();

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		bufferInfo.size = m_Size;

		VulkanAllocator allocator("StorageBuffer");
		m_MemoryAlloc = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Buffer);

		m_DescriptorInfo.buffer = m_Buffer;
		m_DescriptorInfo.offset = 0;
		m_DescriptorInfo.range = m_Size;
	}
	
	void VulkanStorageBuffer::Release()
	{
		if (!m_MemoryAlloc)
			return;

		Renderer::SubmitWithoutResources([buffer = m_Buffer, memoryAlloc = m_MemoryAlloc]()
		{
			VulkanAllocator allocator("StorageBuffer");
			allocator.DestroyBuffer(buffer, memoryAlloc);
		});

		m_Buffer = nullptr;
		m_MemoryAlloc = nullptr;
	}
}

#endif // HIGHLO_API_VULKAN

