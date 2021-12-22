// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanCommandBuffer.h"

#include "Engine/Application/Application.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"

namespace highlo
{
	VulkanCommandBuffer::VulkanCommandBuffer(uint32 count, const HLString &debugName)
		: m_DebugName(debugName)
	{
		auto &device = VulkanContext::GetCurrentDevice();
		auto nativeDevice = device->GetNativeDevice();
		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;

		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = device->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(nativeDevice, &cmdPoolInfo, nullptr, &m_CommandPool));

		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = m_CommandPool;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		if (count == 0)
			count = framesInFlight;
		commandBufferAllocateInfo.commandBufferCount = count;
		m_CommandBuffers.resize(count);
		VK_CHECK_RESULT(vkAllocateCommandBuffers(nativeDevice, &commandBufferAllocateInfo, m_CommandBuffers.data()));

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		m_WaitFences.resize(framesInFlight);
		for (auto &fence : m_WaitFences)
			VK_CHECK_RESULT(vkCreateFence(nativeDevice, &fenceCreateInfo, nullptr, &fence));

		VkQueryPoolCreateInfo queryPoolCreateInfo = {};
		queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		queryPoolCreateInfo.pNext = nullptr;

		// Timestamp queries
		const uint32 maxUserQueries = 10;
		m_TimestampQueryCount = 2 + 2 * maxUserQueries;

		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
		queryPoolCreateInfo.queryCount = m_TimestampQueryCount;
		m_TimestampQueryPools.resize(framesInFlight);
		for (auto &timestampQueryPool : m_TimestampQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(nativeDevice, &queryPoolCreateInfo, nullptr, &timestampQueryPool));

		m_TimestampQueryResults.resize(framesInFlight);
		for (auto &timestampQueryResults : m_TimestampQueryResults)
			timestampQueryResults.resize(m_TimestampQueryCount);

		m_ExecutionGPUTimes.resize(framesInFlight);
		for (auto &executionGPUTimes : m_ExecutionGPUTimes)
			executionGPUTimes.resize(m_TimestampQueryCount / 2);

		// Pipeline statistics queries
		m_PipelineQueryCount = 7;
		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
		queryPoolCreateInfo.queryCount = m_PipelineQueryCount;
		queryPoolCreateInfo.pipelineStatistics =
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

		m_PipelineStatisticsQueryPools.resize(framesInFlight);
		for (auto &pipelineStatisticsQueryPools : m_PipelineStatisticsQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(nativeDevice, &queryPoolCreateInfo, nullptr, &pipelineStatisticsQueryPools));

		m_PipelineStatisticsQueryResults.resize(framesInFlight);
	}

	VulkanCommandBuffer::VulkanCommandBuffer(const HLString &debugName, bool fromSwapChain)
		: m_DebugName(debugName), m_OwnedFromSwapChain(fromSwapChain)
	{
		auto &device = VulkanContext::GetCurrentDevice();
		auto naticeDevice = device->GetNativeDevice();
		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;

		m_CommandBuffers.resize(framesInFlight);
		Ref<VulkanSwapChain> &swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();
		for (uint32 frame = 0; frame < framesInFlight; frame++)
			m_CommandBuffers[frame] = swapChain->GetNativeCommandBuffer(frame);

		VkQueryPoolCreateInfo queryPoolCreateInfo = {};
		queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		queryPoolCreateInfo.pNext = nullptr;

		// Timestamp queries
		const uint32 maxUserQueries = 10;
		m_TimestampQueryCount = 2 + 2 * maxUserQueries;

		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
		queryPoolCreateInfo.queryCount = m_TimestampQueryCount;
		m_TimestampQueryPools.resize(framesInFlight);
		for (auto &timestampQueryPool : m_TimestampQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(naticeDevice, &queryPoolCreateInfo, nullptr, &timestampQueryPool));

		m_TimestampQueryResults.resize(framesInFlight);
		for (auto &timestampQueryResults : m_TimestampQueryResults)
			timestampQueryResults.resize(m_TimestampQueryCount);

		m_ExecutionGPUTimes.resize(framesInFlight);
		for (auto &executionGPUTimes : m_ExecutionGPUTimes)
			executionGPUTimes.resize(m_TimestampQueryCount / 2);

		// Pipeline statistics queries
		m_PipelineQueryCount = 7;
		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
		queryPoolCreateInfo.queryCount = m_PipelineQueryCount;
		queryPoolCreateInfo.pipelineStatistics =
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

		m_PipelineStatisticsQueryPools.resize(framesInFlight);
		for (auto &pipelineStatisticsQueryPools : m_PipelineStatisticsQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(naticeDevice, &queryPoolCreateInfo, nullptr, &pipelineStatisticsQueryPools));

		m_PipelineStatisticsQueryResults.resize(framesInFlight);
	}
	
	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		if (m_OwnedFromSwapChain)
			return;

		auto device = VulkanContext::GetCurrentDevice();
		vkDestroyCommandPool(device->GetNativeDevice(), m_CommandPool, nullptr);
	}
	
	void VulkanCommandBuffer::Begin()
	{
		m_TimestampNextAvailQuery = 2;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		VkCommandBufferBeginInfo cmdBufInfo = {};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		cmdBufInfo.pNext = nullptr;

		VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
		VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));

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
		if (m_OwnedFromSwapChain)
			return;

		auto &device = VulkanContext::GetCurrentDevice();
		auto nativeDevice = device->GetNativeDevice();

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
		submitInfo.pCommandBuffers = &commandBuffer;

		VK_CHECK_RESULT(vkWaitForFences(nativeDevice, 1, &m_WaitFences[frameIndex], VK_TRUE, UINT64_MAX));
		VK_CHECK_RESULT(vkResetFences(nativeDevice, 1, &m_WaitFences[frameIndex]));
		VK_CHECK_RESULT(vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, m_WaitFences[frameIndex]));

		// Retrieve timestamp query results
		vkGetQueryPoolResults(nativeDevice, m_TimestampQueryPools[frameIndex], 0, m_TimestampNextAvailQuery,
							  m_TimestampNextAvailQuery * sizeof(uint64), m_TimestampQueryResults[frameIndex].data(), sizeof(uint64), VK_QUERY_RESULT_64_BIT);

		for (uint32 i = 0; i < m_TimestampNextAvailQuery; i += 2)
		{
			uint64 startTime = m_TimestampQueryResults[frameIndex][i];
			uint64 endTime = m_TimestampQueryResults[frameIndex][i + 1];
			float nsTime = endTime > startTime ? (endTime - startTime) * device->GetPhysicalDevice()->GetLimits().timestampPeriod : 0.0f;
			m_ExecutionGPUTimes[frameIndex][i / 2] = nsTime * 0.000001f; // Time in ms
		}

		// Retrieve pipeline stats results
		vkGetQueryPoolResults(nativeDevice, m_PipelineStatisticsQueryPools[frameIndex], 0, 1,
							  sizeof(PipelineStatistics), &m_PipelineStatisticsQueryResults[frameIndex], sizeof(uint64), VK_QUERY_RESULT_64_BIT);
	}
	
	uint64 VulkanCommandBuffer::BeginTimestampQuery()
	{
		uint64 queryIndex = m_TimestampNextAvailQuery;
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		
		m_TimestampNextAvailQuery += 2;
		
		VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
		vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], (uint32)queryIndex);

		return queryIndex;
	}
	
	void VulkanCommandBuffer::EndTimestampQuery(uint64 queryID)
	{
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
		vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, m_TimestampQueryPools[frameIndex], (uint32)(queryID + 1));
	}
}

#endif // HIGHLO_API_VULKAN

