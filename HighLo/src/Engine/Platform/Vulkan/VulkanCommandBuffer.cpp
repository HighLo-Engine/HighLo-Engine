// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanCommandBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

namespace highlo
{
    // TODO: Refactor constructors so that equal parts get extracted into own functions to avoid code duplication

    VulkanCommandBuffer::VulkanCommandBuffer(uint32 count, const HLString &debugName)
        : m_DebugName(debugName)
    {
        auto &device = VulkanContext::GetCurrentDevice();
        uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;

        if (count == 0)
            count = framesInFlight;

        // Create Command pool
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.queueFamilyIndex = device->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
        commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        VK_CHECK_RESULT(vkCreateCommandPool(device->GetNativeDevice(), &commandPoolCreateInfo, nullptr, &m_CommandPool));
        utils::SetDebugUtilsObjectName(device->GetNativeDevice(), VK_OBJECT_TYPE_COMMAND_POOL, m_DebugName, m_CommandPool);

        VkCommandBufferAllocateInfo commandBufferAllocInfo = {};
        commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.commandPool = m_CommandPool;
        commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocInfo.commandBufferCount = count;

        m_CommandBuffers.resize(count);
        VK_CHECK_RESULT(vkAllocateCommandBuffers(device->GetNativeDevice(), &commandBufferAllocInfo, m_CommandBuffers.data()));
        for (uint32 i = 0; i < count; ++i)
            utils::SetDebugUtilsObjectName(device->GetNativeDevice(), VK_OBJECT_TYPE_COMMAND_BUFFER, fmt::format("{} (frame in flight: {})", m_DebugName, i).c_str(), m_CommandBuffers[i]);

        VkFenceCreateInfo fenceCreate = {};
        fenceCreate.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreate.pNext = nullptr;
        fenceCreate.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        m_WaitFences.resize(framesInFlight);
        for (uint32 i = 0; i < m_WaitFences.size(); ++i)
        {
            VK_CHECK_RESULT(vkCreateFence(device->GetNativeDevice(), &fenceCreate, nullptr, &m_WaitFences[i]));
            utils::SetDebugUtilsObjectName(device->GetNativeDevice(), VK_OBJECT_TYPE_FENCE, fmt::format("{} (frame in flight: {}) fence", m_DebugName, i).c_str(), m_WaitFences[i]);
        }

        // Timestamp queries
        const uint32 maxUserQueries = 16;
        m_TimestampQueryCount = 2 + 2 * maxUserQueries;

        VkQueryPoolCreateInfo queryPoolCreateInfo = {};
        queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
        queryPoolCreateInfo.pNext = nullptr;
        queryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
        queryPoolCreateInfo.queryCount = m_TimestampQueryCount;

        m_TimestampQueryPools.resize(framesInFlight);
        for (auto &timestamp : m_TimestampQueryPools)
            VK_CHECK_RESULT(vkCreateQueryPool(device->GetNativeDevice(), &queryPoolCreateInfo, nullptr, &timestamp));

        m_TimestampQueryResults.resize(framesInFlight);
        for (auto &result : m_TimestampQueryResults)
            result.resize(m_TimestampQueryCount);

        m_ExecutionGPUTimes.resize(framesInFlight);
        for (auto &execution : m_ExecutionGPUTimes)
            execution.resize(m_TimestampQueryCount / 2);

        // Pipeline statistics queries
        m_PipelineQueryCount = 7;
        queryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
        queryPoolCreateInfo.queryCount = m_PipelineQueryCount;
        queryPoolCreateInfo.pipelineStatistics = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

        m_PipelineStatisticsQueryPools.resize(framesInFlight);
        m_PipelineStatisticsQueryResults.resize(framesInFlight);

        for (auto &statistic : m_PipelineStatisticsQueryPools)
            VK_CHECK_RESULT(vkCreateQueryPool(device->GetNativeDevice(), &queryPoolCreateInfo, nullptr, &statistic));
    }

    VulkanCommandBuffer::VulkanCommandBuffer(const HLString &debugName, bool swapChainFlag)
        : m_DebugName(debugName), m_OwnedBySwapchain(true)
    {
        Ref<VulkanDevice> &device = VulkanContext::GetCurrentDevice();
        uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;

        m_CommandBuffers.resize(framesInFlight);
        Ref<VulkanSwapChain> &swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();

        for (uint32 frame = 0; frame < framesInFlight; ++frame)
            m_CommandBuffers[frame] = swapChain->GetDrawCommandbuffer(frame);

        // Timestamp queries
        const uint32 maxUserQueries = 16;
        m_TimestampQueryCount = 2 + 2 * maxUserQueries;

        VkQueryPoolCreateInfo queryPoolCreateInfo = {};
        queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
        queryPoolCreateInfo.pNext = nullptr;
        queryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
        queryPoolCreateInfo.queryCount = m_TimestampQueryCount;

        m_TimestampQueryPools.resize(framesInFlight);
        for (auto &timestamp : m_TimestampQueryPools)
            VK_CHECK_RESULT(vkCreateQueryPool(device->GetNativeDevice(), &queryPoolCreateInfo, nullptr, &timestamp));

        m_TimestampQueryResults.resize(framesInFlight);
        for (auto &result : m_TimestampQueryResults)
            result.resize(m_TimestampQueryCount);

        m_ExecutionGPUTimes.resize(framesInFlight);
        for (auto &execution : m_ExecutionGPUTimes)
            execution.resize(m_TimestampQueryCount / 2);

        // Pipeline statistics queries
        m_PipelineQueryCount = 7;
        queryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
        queryPoolCreateInfo.queryCount = m_PipelineQueryCount;
        queryPoolCreateInfo.pipelineStatistics = VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
            VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
            VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

        m_PipelineStatisticsQueryPools.resize(framesInFlight);
        m_PipelineStatisticsQueryResults.resize(framesInFlight);

        for (auto &pipelineStatistics : m_PipelineStatisticsQueryPools)
            VK_CHECK_RESULT(vkCreateQueryPool(device->GetNativeDevice(), &queryPoolCreateInfo, nullptr, &pipelineStatistics));
    }
    
    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        if (m_OwnedBySwapchain)
            return;

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyCommandPool(device, m_CommandPool, nullptr);
    }
    
    void VulkanCommandBuffer::Begin()
    {
        m_TimestampNextAvailQuery = 2;
        uint32 frameIndex = Renderer::GetCurrentFrameIndex();
        VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
        
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pNext = nullptr;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

        // Timestamp query
        vkCmdResetQueryPool(commandBuffer, m_TimestampQueryPools[frameIndex], 0, m_TimestampQueryCount);
        vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], 0);

        // Pipeline stats query
        vkCmdResetQueryPool(commandBuffer, m_PipelineStatisticsQueryPools[frameIndex], 0, m_PipelineQueryCount);
        vkCmdBeginQuery(commandBuffer, m_PipelineStatisticsQueryPools[frameIndex], 0, 0);
    }
    
    void VulkanCommandBuffer::End()
    {
        uint32 frameIndex = Renderer::GetCurrentFrameIndex();
        VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];

        vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], 1);
        vkCmdEndQuery(commandBuffer, m_PipelineStatisticsQueryPools[frameIndex], 0);
        VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
    }
    
    void VulkanCommandBuffer::Submit()
    {
        if (m_OwnedBySwapchain)
            return;

        auto device = VulkanContext::GetCurrentDevice();
        uint32 frameIndex = Renderer::GetCurrentFrameIndex();
        VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VK_CHECK_RESULT(vkWaitForFences(device->GetNativeDevice(), 1, &m_WaitFences[frameIndex], VK_TRUE, UINT64_MAX));
        VK_CHECK_RESULT(vkResetFences(device->GetNativeDevice(), 1, &m_WaitFences[frameIndex]));
        VK_CHECK_RESULT(vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, m_WaitFences[frameIndex]));

        for (uint32 i = 0; i < m_TimestampNextAvailQuery; i += 2)
        {
            uint64 startTime = m_TimestampQueryResults[frameIndex][i];
            uint64 endTime = m_TimestampQueryResults[frameIndex][i + 1];
            float nsTime = endTime > startTime ? (endTime - startTime) * device->GetPhysicalDevice()->GetLimits().timestampPeriod : 0.0f;
            m_ExecutionGPUTimes[frameIndex][i / 2] = nsTime * 0.000001f;
        }

        vkGetQueryPoolResults(device->GetNativeDevice(), m_PipelineStatisticsQueryPools[frameIndex], 0, 1, sizeof(PipelineStatistics), &m_PipelineStatisticsQueryResults[frameIndex], sizeof(uint64), VK_QUERY_RESULT_64_BIT);
    }
    
    float VulkanCommandBuffer::GetExecutionGPUTime(uint32 frameIndex, uint32 queryIndex) const
    {
        if (queryIndex / 2 >= m_TimestampNextAvailQuery / 2)
            return 0.0f;

        return m_ExecutionGPUTimes[frameIndex][queryIndex / 2];
    }
    
    uint32 VulkanCommandBuffer::BeginTimestampQuery()
    {
        uint32 queryIndex = m_TimestampNextAvailQuery;
        m_TimestampNextAvailQuery += 2;

        uint32 frameIndex = Renderer::GetCurrentFrameIndex();
        VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
        vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], queryIndex);

        return queryIndex;
    }
    
    void VulkanCommandBuffer::EndTimestampQuery(uint32 queryID)
    {
        uint32 frameIndex = Renderer::GetCurrentFrameIndex();
        VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
        vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], queryID + 1);
    }
    
    const PipelineStatistics &VulkanCommandBuffer::GetPipelineStatistics(uint32 frameIndex) const
    {
        HL_ASSERT(frameIndex < m_PipelineStatisticsQueryResults.size());
        return m_PipelineStatisticsQueryResults[frameIndex];
    }
}

#endif // HIGHLO_API_VULKAN

