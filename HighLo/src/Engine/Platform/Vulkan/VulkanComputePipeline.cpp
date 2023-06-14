// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanComputePipeline.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCheckpoint.h"
#include "VulkanShader.h"

namespace highlo
{
    static VkFence s_ComputeFence = nullptr;

    VulkanComputePipeline::VulkanComputePipeline(const Ref<Shader> &computeShader)
        : m_Shader(computeShader)
    {
        Invalidate();
        Renderer::RegisterShaderDependency(computeShader, this);
    }

    VulkanComputePipeline::~VulkanComputePipeline()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyPipeline(device, m_ComputePipeline, nullptr);
        vkDestroyPipelineLayout(device, m_ComputePipelineLayout, nullptr);
        vkDestroyPipelineCache(device, m_PipelineCache, nullptr);
    }
    
    void VulkanComputePipeline::Begin(const Ref<CommandBuffer> &renderCommandBuffer)
    {
        HL_ASSERT(!m_ActiveComputeCommandbuffer);

        if (renderCommandBuffer)
        {
            uint32 frameIndex = Renderer::GetCurrentFrameIndex();
            m_ActiveComputeCommandbuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);
            m_UsingGraphicsQueue = true;
        }
        else
        {
            m_ActiveComputeCommandbuffer = VulkanContext::GetCurrentDevice()->CreateCommandBuffer(true, true);
            m_UsingGraphicsQueue = false;
        }

        vkCmdBindPipeline(m_ActiveComputeCommandbuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipeline);
    }
    
    void VulkanComputePipeline::End()
    {
        HL_ASSERT(m_ActiveComputeCommandbuffer);

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        if (!m_UsingGraphicsQueue)
        {
            VkQueue computeQueue = VulkanContext::GetCurrentDevice()->GetComputeQueue();
            vkEndCommandBuffer(m_ActiveComputeCommandbuffer);

            if (!s_ComputeFence)
            {
                VkFenceCreateInfo fenceCreate = {};
                fenceCreate.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
                fenceCreate.flags = VK_FENCE_CREATE_SIGNALED_BIT;
                VK_CHECK_RESULT(vkCreateFence(device, &fenceCreate, nullptr, &s_ComputeFence));
                utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FENCE, "Compute pipeline fence", s_ComputeFence);
            }

            vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
            vkResetFences(device, 1, &s_ComputeFence);

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &m_ActiveComputeCommandbuffer;
            VK_CHECK_RESULT(vkQueueSubmit(computeQueue, 1, &submitInfo, s_ComputeFence));

            // Safety instruction: this is here to wait until the execution of the compute shader is finished
            vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
        }

        m_ActiveComputeCommandbuffer = nullptr;
    }
    
    void VulkanComputePipeline::Invalidate()
    {
        CreatePipeline();
    }

    void VulkanComputePipeline::Execute(VkDescriptorSet *descriptorSets, uint32 descriptorSetCount, uint32 x, uint32 y, uint32 z)
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        VkQueue computeQueue = VulkanContext::GetCurrentDevice()->GetComputeQueue();

        VkCommandBuffer computeCommandBuffer = VulkanContext::GetCurrentDevice()->CreateCommandBuffer(true, true);
        SetVulkanCheckpoint(computeCommandBuffer, "VulkanComputePipeline::Execute");

        vkCmdBindPipeline(computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipeline);
        for (uint32 i = 0; i < descriptorSetCount; ++i)
        {
            vkCmdBindDescriptorSets(computeCommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipelineLayout, 0, 1, &descriptorSets[i], 0, 0);
            vkCmdDispatch(computeCommandBuffer, x, y, z);
        }
        vkEndCommandBuffer(computeCommandBuffer);

        if (!s_ComputeFence)
        {
            VkFenceCreateInfo fenceCreate = {};
            fenceCreate.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceCreate.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            VK_CHECK_RESULT(vkCreateFence(device, &fenceCreate, nullptr, &s_ComputeFence));
            utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FENCE, "Compute pipeline fence", s_ComputeFence);
        }

        // Make sure previous compute shader in pipeline has completed (TODO: this shouldn't be needed for all cases)
        vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &s_ComputeFence);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &computeCommandBuffer;
        VK_CHECK_RESULT(vkQueueSubmit(computeQueue, 1, &submitInfo, s_ComputeFence));

        // Safety instruction: this is here to wait until the execution of the compute shader is finished
        vkWaitForFences(device, 1, &s_ComputeFence, VK_TRUE, UINT64_MAX);
    }

    void VulkanComputePipeline::Dispatch(VkDescriptorSet descriptorSet, uint32 x, uint32 y, uint32 z) const
    {
        HL_ASSERT(m_ActiveComputeCommandbuffer);
        vkCmdBindDescriptorSets(m_ActiveComputeCommandbuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipelineLayout, 0, 1, &descriptorSet, 0, 0);
        vkCmdDispatch(m_ActiveComputeCommandbuffer, x, y, z);
    }
    
    void VulkanComputePipeline::SetPushConstants(const void *data, uint32 size, uint32 offset) const
    {
        HL_ASSERT(m_ActiveComputeCommandbuffer);
        vkCmdPushConstants(m_ActiveComputeCommandbuffer, m_ComputePipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, offset, size, data);
    }
    
    void VulkanComputePipeline::CreatePipeline()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        Ref<VulkanShader> vulkanShader = m_Shader.As<VulkanShader>();

        auto descriptorSetLayouts = vulkanShader->GetAllDescriptorSetLayouts();

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = (uint32)descriptorSetLayouts.size();
        pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();

        const auto &pushConstantRanges = vulkanShader->GetPushConstantRanges();
        std::vector<VkPushConstantRange> vulkanPushConstantRanges = utils::PushConstantRangeToVulkanPushConstantRange(pushConstantRanges);

        if (pushConstantRanges.size())
        {
            pipelineLayoutCreateInfo.pushConstantRangeCount = (uint32)vulkanPushConstantRanges.size();
            pipelineLayoutCreateInfo.pPushConstantRanges = vulkanPushConstantRanges.data();
        }

        VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &m_ComputePipelineLayout));

        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
        pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &m_PipelineCache));

        const auto &shaderStages = vulkanShader->GetPipelineShaderStageCreateInfos();
        HL_ASSERT(shaderStages.size() > 0);

        VkComputePipelineCreateInfo computePipelineCreateInfo = {};
        computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        computePipelineCreateInfo.layout = m_ComputePipelineLayout;
        computePipelineCreateInfo.flags = 0;
        computePipelineCreateInfo.stage = shaderStages[0];
        VK_CHECK_RESULT(vkCreateComputePipelines(device, m_PipelineCache, 1, &computePipelineCreateInfo, nullptr, &m_ComputePipeline));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE, m_Shader->GetName(), m_ComputePipeline);
    }
}

#endif // HIGHLO_API_VULKAN

