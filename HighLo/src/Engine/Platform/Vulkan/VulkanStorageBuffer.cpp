#include "HighLoPch.h"
#include "VulkanStorageBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanStorageBuffer::VulkanStorageBuffer(uint32 size, uint32 binding)
		: m_Size(size), m_Binding(binding)
	{
		m_LocalStorage = new uint8[size];
		Invalidate();
	}

	VulkanStorageBuffer::~VulkanStorageBuffer()
	{
		Release();
	}

	void VulkanStorageBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
		memcpy(m_LocalStorage, data, size);

		VulkanAllocator allocator("VulkanStorageBuffer");
		uint8 *pData = allocator.MapMemory<uint8>(m_MemoryAlloc);
		memcpy(pData, (uint8*)m_LocalStorage + offset, size);
		allocator.UnmapMemory(m_MemoryAlloc);
	}

	void VulkanStorageBuffer::Resize(uint32 size)
	{
		m_Size = size;
		Invalidate();
	}

	void VulkanStorageBuffer::Invalidate()
	{
		Release();

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
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

		VulkanAllocator allocator("StorageBuffer");
		allocator.DestroyBuffer(m_Buffer, m_MemoryAlloc);

		m_Buffer = nullptr;
		m_MemoryAlloc = nullptr;

		delete[] m_LocalStorage;
		m_LocalStorage = nullptr;
	}
}

#endif // HIGHLO_API_VULKAN

