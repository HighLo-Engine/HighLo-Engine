// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanUniformBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanAllocator.h"

namespace highlo
{
	VulkanUniformBuffer::VulkanUniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: UniformBuffer(binding, layout), m_Size(size)
	{
		Ref<VulkanUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->RT_Invalidate();
		});
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		Release();
	}
	
	void VulkanUniformBuffer::UploadToShader()
	{
		VulkanAllocator allocator("VulkanUniformBuffer");
		uint8 *pData = allocator.MapMemory<uint8>(m_MemoryAlloc);
		memcpy(pData, (const uint8*)m_Data, m_Size);
		allocator.UnmapMemory(m_MemoryAlloc);
	}

	void VulkanUniformBuffer::RT_Invalidate()
	{
		Release();

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

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
		m_MemoryAlloc = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_Buffer);

		m_DescriptorInfo.buffer = m_Buffer;
		m_DescriptorInfo.offset = 0;
		m_DescriptorInfo.range = m_Size;
	}
	
	void VulkanUniformBuffer::Release()
	{
		if (!m_MemoryAlloc)
			return;

		Renderer::SubmitWithoutResources([buffer = m_Buffer, memoryAlloc = m_MemoryAlloc]()
		{
			VulkanAllocator allocator("UniformBuffer");
			allocator.DestroyBuffer(buffer, memoryAlloc);
		});

		m_Buffer = nullptr;
		m_MemoryAlloc = nullptr;
	}
}

#endif // HIGHLO_API_VULKAN

