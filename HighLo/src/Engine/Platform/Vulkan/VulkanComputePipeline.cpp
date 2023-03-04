// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanComputePipeline.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Renderer/Renderer.h"
#include "VulkanContext.h"
#include "VulkanCommandBuffer.h"
#include "VulkanShader.h"

namespace highlo
{
	static VkFence s_ComputeFence = nullptr;

	VulkanComputePipeline::VulkanComputePipeline(const Ref<Shader> &computeShader)
		: m_Shader(computeShader)
	{
		Ref<VulkanComputePipeline> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->RT_CreatePipeline();
		});

		Renderer::RegisterShaderDependency(computeShader, this);
	}

	VulkanComputePipeline::~VulkanComputePipeline()
	{
		m_Shader = nullptr;
	}

	void VulkanComputePipeline::Begin(const Ref<CommandBuffer> &renderCommandBuffer)
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
			m_ActiveComputeCommandBuffer = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true, true);
			m_UsingGraphicsQueue = false;
		}

		vkCmdBindPipeline(m_ActiveComputeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipeline);
	}
	
	void VulkanComputePipeline::End()
	{
		HL_ASSERT(m_ActiveComputeCommandBuffer);

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		if (!m_UsingGraphicsQueue)
		{
			VkQueue computeQueue = VulkanContext::GetCurrentDevice()->GetComputeQueue();

			vkEndCommandBuffer(m_ActiveComputeCommandBuffer);

			if (!s_ComputeFence)
			{
				VkFenceCreateInfo fenceCreateInfo{};
				fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
				VK_CHECK_RESULT(vkCreateFence(device, &fenceCreateInfo, nullptr, &s_ComputeFence));
				utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FENCE, "Compute pipeline fence", s_ComputeFence);
			}

			vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
			vkResetFences(device, 1, &s_ComputeFence);

			VkSubmitInfo computeSubmitInfo{};
			computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			computeSubmitInfo.commandBufferCount = 1;
			computeSubmitInfo.pCommandBuffers = &m_ActiveComputeCommandBuffer;
			VK_CHECK_RESULT(vkQueueSubmit(computeQueue, 1, &computeSubmitInfo, s_ComputeFence));

			// Wait for execution of compute shader to complete
			// Currently this is here for "safety"
			{
				vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
			}
		}

		m_ActiveComputeCommandBuffer = nullptr;
	}
	
	void VulkanComputePipeline::Invalidate()
	{
	}

	void VulkanComputePipeline::Dispatch(VkDescriptorSet descriptorSet, uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) const
	{
		HL_ASSERT(m_ActiveComputeCommandBuffer);

		vkCmdBindDescriptorSets(m_ActiveComputeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipelineLayout, 0, 1, &descriptorSet, 0, 0);
		vkCmdDispatch(m_ActiveComputeCommandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	void VulkanComputePipeline::Execute(VkDescriptorSet *descriptorSets, uint32 descriptorSetCount, uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ)
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

		VkQueue computeQueue = VulkanContext::GetCurrentDevice()->GetComputeQueue();

		VkCommandBuffer computeCommandBuffer = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true, true);

		// TODO
	//	utils::SetVulkanCheckpoint(computeCommandBuffer, "VulkanComputePipeline::Execute");

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

			utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FENCE, fmt::format("Compute pipeline fence"), s_ComputeFence);
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
		// Currently this is here for "safety"
		{
			vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
		}
	}
	
	void VulkanComputePipeline::SetPushConstants(const void *data, uint32 size) const
	{
		vkCmdPushConstants(m_ActiveComputeCommandBuffer, m_ComputePipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, size, data);
	}
	
	void VulkanComputePipeline::CreatePipeline()
	{
		Ref<VulkanComputePipeline> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->RT_CreatePipeline();
		});
	}
	
	void VulkanComputePipeline::RT_CreatePipeline()
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		Ref<VulkanShader> shader = m_Shader.As<VulkanShader>();
		auto &descriptorSetLayouts = shader->GetAllDescriptorSetLayouts();

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = (uint32)descriptorSetLayouts.size();
		pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();

		const auto &pushConstantRanges = shader->GetPushConstantRanges();
		std::vector<VkPushConstantRange> vulkanPushConstantRanges(pushConstantRanges.size());
		if (pushConstantRanges.size())
		{
			// TODO: should come from shader
			for (uint32 i = 0; i < pushConstantRanges.size(); i++)
			{
				const auto &pushConstantRange = pushConstantRanges[i];
				auto &vulkanPushConstantRange = vulkanPushConstantRanges[i];

				vulkanPushConstantRange.stageFlags = pushConstantRange.Stage;
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
		const auto &shaderStages = shader->GetPipelineShaderStageCreateInfos();
		computePipelineCreateInfo.stage = shaderStages[0];

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &m_PipelineCache));
		VK_CHECK_RESULT(vkCreateComputePipelines(device, m_PipelineCache, 1, &computePipelineCreateInfo, nullptr, &m_ComputePipeline));

		utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE, m_Shader->GetName(), m_ComputePipeline);
	}
}

#endif // HIGHLO_API_VULKAN

