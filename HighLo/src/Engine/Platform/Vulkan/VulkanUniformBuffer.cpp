// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanUniformBuffer.h"

#include "VulkanContext.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32 size, uint32 binding)
		: m_Size(size), m_Binding(binding)
	{
		m_LocalStorage = new uint8[size];
		Invalidate();
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		Release();
	}
	
	void VulkanUniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
		memcpy(m_LocalStorage, data, size);

		VulkanAllocator allocator("VulkanUniformBuffer");
		uint8 *pData = allocator.MapMemory<uint8>(m_MemoryAlloc);
		memcpy(pData, (const uint8*)m_LocalStorage + offset, size);
		allocator.UnmapMemory(m_MemoryAlloc);
	}
	
	void VulkanUniformBuffer::Invalidate()
	{
		Release();

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = 0;
		allocInfo.memoryTypeIndex = 0;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.size = m_Size;

		VulkanAllocator allocator("UniformBuffer");
		m_MemoryAlloc = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, m_Buffer);

		m_DescriptorInfo.buffer = m_Buffer;
		m_DescriptorInfo.offset = 0;
		m_DescriptorInfo.range = m_Size;
	}

	void VulkanUniformBuffer::Release()
	{
		if (!m_MemoryAlloc)
			return;

		VulkanAllocator allocator("UniformBuffer");
		allocator.DestroyBuffer(m_Buffer, m_MemoryAlloc);

		m_Buffer = nullptr;
		m_MemoryAlloc = nullptr;

		delete[] m_LocalStorage;
		m_LocalStorage = nullptr;
	}
}

#endif // HIGHLO_API_VULKAN

