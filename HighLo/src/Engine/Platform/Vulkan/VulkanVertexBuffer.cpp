// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexBuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

namespace highlo
{
    VulkanVertexBuffer::VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
        : m_Usage(usage), m_Size(size)
    {
        m_LocalData = Allocator::Copy(data, size);
        auto device = VulkanContext::GetCurrentDevice();

        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        // Create staging buffer
        VkBuffer stagingBuffer;
        VmaAllocation stagingBufferAllocation = utils::AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

        // Copy data to staging buffer
        uint8 *dstData = utils::MapMemory<uint8>(stagingBufferAllocation);
        memcpy(dstData, m_LocalData.Data, m_LocalData.Size);
        utils::UnmapMemory(stagingBufferAllocation);

        VkBufferCreateInfo vertexBufferCreateInfo = {};
        vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vertexBufferCreateInfo.size = size;
        vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        m_MemoryAllocation = utils::AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_VulkanBuffer);

        VkCommandBuffer copyCommand = device->CreateCommandBuffer(true);
        VkBufferCopy copyRegion = {};
        copyRegion.size = m_LocalData.Size;
        vkCmdCopyBuffer(copyCommand, stagingBuffer, m_VulkanBuffer, 1, &copyRegion);
        device->FlushCommandBuffer(copyCommand);

        utils::DestroyBuffer(stagingBuffer, stagingBufferAllocation);
    }

    VulkanVertexBuffer::VulkanVertexBuffer(uint32 size, VertexBufferUsage usage)
        : m_Usage(usage), m_Size(size)
    {
        m_LocalData.Allocate(size);

        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.size = size;
        bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        m_MemoryAllocation = utils::AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_VulkanBuffer);
    }
    
    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        utils::DestroyBuffer(m_VulkanBuffer, m_MemoryAllocation);
        m_LocalData.Release();
    }
    
    void VulkanVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
    {
        HL_ASSERT(size <= m_LocalData.Size);
        memcpy(m_LocalData.Data, (uint8*)data + offset, size);

        uint8 *mappedMemory = utils::MapMemory<uint8>(m_MemoryAllocation);
        memcpy(mappedMemory, (uint8*)data + offset, size);
        utils::UnmapMemory(m_MemoryAllocation);
    }
}

#endif // HIGHLO_API_VULKAN

