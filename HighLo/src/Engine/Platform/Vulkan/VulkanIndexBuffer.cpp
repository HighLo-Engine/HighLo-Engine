#include "HighLoPch.h"
#include "VulkanIndexBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"
#include "VulkanContext.h"

namespace highlo
{
	VulkanIndexBuffer::VulkanIndexBuffer(std::vector<int32> &indices)
	{
		m_Size = ((uint32)indices.size()) * sizeof(uint32);
		m_LocalData = Allocator::Copy(&indices[0], m_Size);
		auto device = VulkanContext::GetCurrentDevice();

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_Size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation = utils::AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		// Copy data to staging buffer
		uint8 *destData = utils::MapMemory<uint8>(stagingBufferAllocation);
		memcpy(destData, m_LocalData.Data, m_LocalData.Size);
		utils::UnmapMemory(stagingBufferAllocation);

		VkBufferCreateInfo indexBufferCreateInfo = {};
		indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferCreateInfo.size = m_Size;
		indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		m_MemoryAllocation = utils::AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_VulkanBuffer);

		VkCommandBuffer copyCmd = device->CreateCommandBuffer(true);

		VkBufferCopy copyRegion = {};
		copyRegion.size = m_LocalData.Size;
		vkCmdCopyBuffer(
			copyCmd,
			stagingBuffer,
			m_VulkanBuffer,
			1,
			&copyRegion);

		device->FlushCommandBuffer(copyCmd);

		utils::DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	}

	VulkanIndexBuffer::VulkanIndexBuffer(void *data, uint32 size)
		: m_Size(size)
	{
		m_LocalData = Allocator::Copy(data, size);
		auto device = VulkanContext::GetCurrentDevice();

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_Size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation = utils::AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		// Copy data to staging buffer
		uint8 *destData = utils::MapMemory<uint8>(stagingBufferAllocation);
		memcpy(destData, m_LocalData.Data, m_LocalData.Size);
		utils::UnmapMemory(stagingBufferAllocation);

		VkBufferCreateInfo indexBufferCreateInfo = {};
		indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferCreateInfo.size = m_Size;
		indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		m_MemoryAllocation = utils::AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_VulkanBuffer);

		VkCommandBuffer copyCmd = device->CreateCommandBuffer(true);

		VkBufferCopy copyRegion = {};
		copyRegion.size = m_LocalData.Size;
		vkCmdCopyBuffer(
			copyCmd,
			stagingBuffer,
			m_VulkanBuffer,
			1,
			&copyRegion);

		device->FlushCommandBuffer(copyCmd);

		utils::DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	}
	
	VulkanIndexBuffer::VulkanIndexBuffer(uint32 size)
		: m_Size(size)
	{
		m_LocalData.Allocate(size);

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		utils::AllocateBuffer(createInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_VulkanBuffer);
	}
	
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		utils::DestroyBuffer(m_VulkanBuffer, m_MemoryAllocation);
		m_LocalData.Release();
	}
	
	void VulkanIndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		HL_ASSERT(size <= m_LocalData.Size);
		memcpy(m_LocalData.Data, (uint8*)data + offset, size);

		uint8 *mappedMemory = utils::MapMemory<uint8>(m_MemoryAllocation);
		memcpy(mappedMemory, (uint8*)data + offset, size);
		utils::UnmapMemory(m_MemoryAllocation);
	}
	
	void VulkanIndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
		UpdateContents((void*)indices.data(), (uint32)indices.size(), offset);
	}
}

#endif // HIGHLO_API_VULKAN

