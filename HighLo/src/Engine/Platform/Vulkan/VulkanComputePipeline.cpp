#include "HighLoPch.h"
#include "VulkanComputePipeline.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanCommandBuffer.h"

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	static VkFence s_ComputeFence = nullptr;

	VulkanComputePipeline::VulkanComputePipeline(Ref<Shader> computeShader)
		: m_Shader(computeShader.As<VulkanShader>())
	{
		Invalidate();

		Renderer::RegisterShaderDependency(computeShader, this);
	}
	
	VulkanComputePipeline::~VulkanComputePipeline()
	{
	}
	
	void VulkanComputePipeline::Begin(Ref<CommandBuffer> renderCommandBuffer)
	{
		HL_ASSERT(!m_ActiveComputeCommandBuffer);

		if (renderCommandBuffer)
		{
			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			m_ActiveComputeCommandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);
			m_UsingGraphicsQueue = true;
		}
		else
		{
			m_ActiveComputeCommandBuffer = VulkanContext::GetCurrentDevice()->CreateCommandBuffer(true);
			m_UsingGraphicsQueue = false;
		}

		vkCmdBindPipeline(m_ActiveComputeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipeline);
	}

	void VulkanComputePipeline::End()
	{
		HL_ASSERT(m_ActiveComputeCommandBuffer);

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		if (!m_UsingGraphicsQueue)
		{
			VkQueue computeQueue = VulkanContext::GetCurrentDevice()->GetComputeQueue();

			if (!s_ComputeFence)
			{
				VkFenceCreateInfo fenceCreateInfo{};
				fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
				VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &s_ComputeFence));
			}

			vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
			vkResetFences(device, 1, &s_ComputeFence);

			VkSubmitInfo computeSubmitInfo{};
			computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			computeSubmitInfo.commandBufferCount = 1;
			computeSubmitInfo.pCommandBuffers = &m_ActiveComputeCommandBuffer;
			VK_CHECK_RESULT(vkQueueSubmit(computeQueue, 1, &computeSubmitInfo, s_ComputeFence));

			// Wait for execution of compute shader to complete
			Timer timer("ComputePipelineExecution");
			vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
			timer.Stop();
			HL_CORE_TRACE("Compute Shader execution VulkanComputePipeline::End() ({0})", *timer.GetOutputString());
		}

		m_ActiveComputeCommandBuffer = nullptr;
	}

	void VulkanComputePipeline::Invalidate()
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		auto descriptorSetLayouts = m_Shader->GetAllDescriptorSetLayouts();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = (uint32)descriptorSetLayouts.size();
		pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();

		const auto &pushConstantRanges = m_Shader->GetPushConstantRanges();
		std::vector<VkPushConstantRange> vulkanPushConstantRanges(pushConstantRanges.size());
		if (pushConstantRanges.size())
		{
			// TODO: should come from shader
			for (uint32 i = 0; i < pushConstantRanges.size(); i++)
			{
				const auto &pushConstantRange = pushConstantRanges[i];
				auto &vulkanPushConstantRange = vulkanPushConstantRanges[i];

				vulkanPushConstantRange.stageFlags = pushConstantRange.ShaderStage;
				vulkanPushConstantRange.offset = pushConstantRange.Offset;
				vulkanPushConstantRange.size = pushConstantRange.Size;
			}

			pipelineLayoutCreateInfo.pushConstantRangeCount = (uint32)vulkanPushConstantRanges.size();
			pipelineLayoutCreateInfo.pPushConstantRanges = vulkanPushConstantRanges.data();
		}

		VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &m_ComputePipelineLayout));

		VkComputePipelineCreateInfo computePipelineCreateInfo{};
		computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		computePipelineCreateInfo.layout = m_ComputePipelineLayout;
		computePipelineCreateInfo.flags = 0;
		const auto &shaderStages = m_Shader->GetPipelineShaderStageCreateInfos();
		computePipelineCreateInfo.stage = shaderStages[0];

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &m_PipelineCache));
		VK_CHECK_RESULT(vkCreateComputePipelines(device, m_PipelineCache, 1, &computePipelineCreateInfo, nullptr, &m_ComputePipeline));
	}
	
	void VulkanComputePipeline::Dispatch(VkDescriptorSet descriptorSet, uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ)
	{
		HL_ASSERT(m_ActiveComputeCommandBuffer);

		vkCmdBindDescriptorSets(m_ActiveComputeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipelineLayout, 0, 1, &descriptorSet, 0, 0);
		vkCmdDispatch(m_ActiveComputeCommandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	void VulkanComputePipeline::Execute(VkDescriptorSet *descriptorSets, uint32 descriptorSetCount, uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ)
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		VkQueue computeQueue = VulkanContext::GetCurrentDevice()->GetComputeQueue();
		VkCommandBuffer computeCommandBuffer = VulkanContext::GetCurrentDevice()->CreateCommandBuffer(true, true);

		utils::SetVulkanCheckpoint(computeCommandBuffer, "VulkanComputePipeline::Execute");

		vkCmdBindPipeline(computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipeline);
		for (uint32 i = 0; i < descriptorSetCount; i++)
		{
			vkCmdBindDescriptorSets(computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipelineLayout, 0, 1, &descriptorSets[i], 0, 0);
			vkCmdDispatch(computeCommandBuffer, groupCountX, groupCountY, groupCountZ);
		}

		vkEndCommandBuffer(computeCommandBuffer);
		if (!s_ComputeFence)
		{

			VkFenceCreateInfo fenceCreateInfo{};
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &s_ComputeFence));
		}

		// Make sure previous compute shader in pipeline has completed (TODO: this shouldn't be needed for all cases)
		vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
		vkResetFences(device, 1, &s_ComputeFence);

		VkSubmitInfo computeSubmitInfo{};
		computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		computeSubmitInfo.commandBufferCount = 1;
		computeSubmitInfo.pCommandBuffers = &computeCommandBuffer;
		VK_CHECK_RESULT(vkQueueSubmit(computeQueue, 1, &computeSubmitInfo, s_ComputeFence));

		// Wait for execution of compute shader to complete
		Timer timer("");
		vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
		timer.Stop();
		HL_CORE_TRACE("Compute shader execution VulkanComputePipeline::Execute() ({0})", *timer.GetOutputString());
	}

	void VulkanComputePipeline::SetPushConstants(const void *data, uint32 size)
	{
		vkCmdPushConstants(m_ActiveComputeCommandBuffer, m_ComputePipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, size, data);
	}
}

#endif // HIGHLO_API_VULKAN

