// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexArray.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

#include "VulkanContext.h"
#include "VulkanShader.h"
#include "VulkanFramebuffer.h"

namespace highlo
{
    namespace utils
    {
        static VkPrimitiveTopology PrimitiveTypeToVulkanType(PrimitiveType type)
        {
            switch (type)
            {
                case PrimitiveType::Lines:          return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                case PrimitiveType::LineStrip:      return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
                case PrimitiveType::Triangles:      return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                case PrimitiveType::TriangleStrip:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            }

            HL_ASSERT(false);
            return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
        }

        static VkFormat ShaderDataTypeToVulkanFormat(ShaderDataType type)
        {
            switch (type)
            {
                case ShaderDataType::Float:     return VK_FORMAT_R32_SFLOAT;
                case ShaderDataType::Float2:    return VK_FORMAT_R32G32_SFLOAT;
                case ShaderDataType::Float3:    return VK_FORMAT_R32G32B32_SFLOAT;
                case ShaderDataType::Float4:    return VK_FORMAT_R32G32B32A32_SFLOAT;

                case ShaderDataType::Int:       return VK_FORMAT_R32_SINT;
                case ShaderDataType::Int2:      return VK_FORMAT_R32G32_SINT;
                case ShaderDataType::Int3:      return VK_FORMAT_R32G32B32_SINT;
                case ShaderDataType::Int4:      return VK_FORMAT_R32G32B32A32_SINT;
            }

            HL_ASSERT(false);
            return VK_FORMAT_UNDEFINED;
        }

        static VkCompareOp DepthCompareOpToVulkan(const DepthCompareOperator op)
        {
            switch (op)
            {
                case DepthCompareOperator::Never:           return VK_COMPARE_OP_NEVER;
                case DepthCompareOperator::NotEqual:        return VK_COMPARE_OP_NOT_EQUAL;
                case DepthCompareOperator::Less:            return VK_COMPARE_OP_LESS;
                case DepthCompareOperator::LessOrEqual:     return VK_COMPARE_OP_LESS_OR_EQUAL;
                case DepthCompareOperator::Greater:         return VK_COMPARE_OP_GREATER;
                case DepthCompareOperator::GreaterOrEqual:  return VK_COMPARE_OP_GREATER_OR_EQUAL;
                case DepthCompareOperator::Equal:           return VK_COMPARE_OP_EQUAL;
                case DepthCompareOperator::Always:          return VK_COMPARE_OP_ALWAYS;
            }

            HL_ASSERT(false);
            return VK_COMPARE_OP_MAX_ENUM;
        }
    }

    VulkanVertexArray::VulkanVertexArray(const VertexArraySpecification &spec)
        : m_Specification(spec)
    {
        HL_ASSERT(spec.Shader);
        HL_ASSERT(spec.RenderPass);

        Invalidate();

        Renderer::RegisterShaderDependency(spec.Shader, this);
    }
    
    VulkanVertexArray::~VulkanVertexArray()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyPipeline(device, m_Pipeline, nullptr);
        vkDestroyPipelineCache(device, m_PipelineCache, nullptr);
        vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
    }
    
    void VulkanVertexArray::Invalidate()
    {
        HL_ASSERT(m_Specification.Shader);

        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        Ref<VulkanShader> vulkanShader = Ref<VulkanShader>(m_Specification.Shader);
        Ref<VulkanFramebuffer> vulkanFramebuffer = m_Specification.RenderPass->GetSpecification().Framebuffer.As<VulkanFramebuffer>();
        BufferLayout &layout = m_Specification.Layout;
        BufferLayout &instanceLayout = m_Specification.InstanceLayout;
        std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> vertexInputAttributes(layout.GetElementCount() + instanceLayout.GetElementCount());
        const auto &shaderStages = vulkanShader->GetPipelineShaderStageCreateInfos();

        auto &descriptorSetLayouts = vulkanShader->GetAllDescriptorSetLayouts();
        const auto &pushConstantRanges = vulkanShader->GetPushConstantRanges();
        std::vector<VkPushConstantRange> vulkanPushConstantRanges = utils::PushConstantRangeToVulkanPushConstantRange(pushConstantRanges);

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.pNext = nullptr;
        pipelineLayoutCreateInfo.setLayoutCount = (uint32)descriptorSetLayouts.size();
        pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutCreateInfo.pushConstantRangeCount = (uint32)vulkanPushConstantRanges.size();
        pipelineLayoutCreateInfo.pPushConstantRanges = vulkanPushConstantRanges.data();
        VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout));

        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.layout = m_PipelineLayout;
        pipelineCreateInfo.renderPass = vulkanFramebuffer->GetRenderPass();

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
        inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyState.topology = utils::PrimitiveTypeToVulkanType(m_Specification.RenderType);

        // Rasterization state
        VkPipelineRasterizationStateCreateInfo rasterizationState = {};
        rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.polygonMode = m_Specification.Wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
        rasterizationState.cullMode = m_Specification.Wireframe ? VK_CULL_MODE_BACK_BIT : VK_CULL_MODE_NONE;
        rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationState.depthClampEnable = VK_FALSE;
        rasterizationState.rasterizerDiscardEnable = VK_FALSE;
        rasterizationState.depthBiasEnable = VK_FALSE;
        rasterizationState.lineWidth = m_Specification.LineWidth;

        uint64 colorAttachmentCount = vulkanFramebuffer->GetColorAttachmentCount();
        std::vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates(colorAttachmentCount);
        if (vulkanFramebuffer->GetSpecification().SwapChainTarget)
        {
            blendAttachmentStates[0].colorWriteMask = 0xf;
            blendAttachmentStates[0].blendEnable = VK_TRUE;
            blendAttachmentStates[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            blendAttachmentStates[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            blendAttachmentStates[0].colorBlendOp = VK_BLEND_OP_ADD;
            blendAttachmentStates[0].alphaBlendOp = VK_BLEND_OP_ADD;
            blendAttachmentStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            blendAttachmentStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        }
        else
        {
            for (uint64 i = 0; i < colorAttachmentCount; ++i)
            {
                if (!vulkanFramebuffer->GetSpecification().Blend)
                    break;

                blendAttachmentStates[i].colorWriteMask = 0xf;

                const auto &attachmentSpec = vulkanFramebuffer->GetSpecification().Attachments.Attachments[i];
                FramebufferBlendMode blendMode = 
                    vulkanFramebuffer->GetSpecification().BlendMode == FramebufferBlendMode::None 
                    ? attachmentSpec.BlendMode 
                    : vulkanFramebuffer->GetSpecification().BlendMode;

                blendAttachmentStates[i].blendEnable = attachmentSpec.Blend ? VK_TRUE : VK_FALSE;
                blendAttachmentStates[i].colorBlendOp = VK_BLEND_OP_ADD;
                blendAttachmentStates[i].alphaBlendOp = VK_BLEND_OP_ADD;
                blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

                switch (blendMode)
                {
                    case FramebufferBlendMode::SrcAlphaOneMinusSrcAlpha:
                        blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                        blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                        break;

                    case FramebufferBlendMode::OneZero:
                        blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
                        blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                        break;

                    case FramebufferBlendMode::ZeroSrcColor:
                        blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                        blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
                        break;

                    default:
                        HL_ASSERT(false);
                }
            }
        }

        VkPipelineColorBlendStateCreateInfo colorBlendState = {};
        colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendState.attachmentCount = (uint32)blendAttachmentStates.size();
        colorBlendState.pAttachments = blendAttachmentStates.data();

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pNext = nullptr;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        std::vector<VkDynamicState> dynamicStateEnables;
        dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
        if (m_Specification.RenderType == PrimitiveType::None || m_Specification.RenderType == PrimitiveType::LineStrip || m_Specification.Wireframe)
            dynamicStateEnables.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

        VkPipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pNext = nullptr;
        dynamicState.dynamicStateCount = (uint32)dynamicStateEnables.size();
        dynamicState.pDynamicStates = dynamicStateEnables.data();

        VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
        depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilState.pNext = nullptr;
        depthStencilState.depthTestEnable = m_Specification.DepthTest ? VK_TRUE : VK_FALSE;
        depthStencilState.depthWriteEnable = m_Specification.DepthWrite ? VK_TRUE : VK_FALSE;
        depthStencilState.depthCompareOp = utils::DepthCompareOpToVulkan(m_Specification.DepthCompareOp);
        depthStencilState.depthBoundsTestEnable = VK_FALSE;
        depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
        depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
        depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
        depthStencilState.stencilTestEnable = VK_FALSE;
        depthStencilState.front = depthStencilState.back;

        VkPipelineMultisampleStateCreateInfo multiSampleState = {};
        multiSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multiSampleState.pNext = nullptr;
        multiSampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multiSampleState.pSampleMask = nullptr;

        VkVertexInputBindingDescription &vertexInputBinding = vertexInputBindingDescriptions.emplace_back();
        vertexInputBinding.binding = 0;
        vertexInputBinding.stride = layout.GetStride();
        vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        if (instanceLayout.GetElementCount())
        {
            VkVertexInputBindingDescription &instanceVertexInputBinding = vertexInputBindingDescriptions.emplace_back();
            instanceVertexInputBinding.binding = 1;
            instanceVertexInputBinding.stride = instanceLayout.GetStride();
            instanceVertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
        }

        uint32 location = 0;
        for (auto &element : layout)
        {
            vertexInputAttributes[location].binding = 0;
            vertexInputAttributes[location].location = location;
            vertexInputAttributes[location].format = utils::ShaderDataTypeToVulkanFormat(element.Type);
            vertexInputAttributes[location].offset = element.Offset;
            ++location;
        }

        for (auto &element : instanceLayout)
        {
            vertexInputAttributes[location].binding = 1;
            vertexInputAttributes[location].location = location;
            vertexInputAttributes[location].format = utils::ShaderDataTypeToVulkanFormat(element.Type);
            vertexInputAttributes[location].offset = element.Offset;
            ++location;
        }

        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputState.pNext = nullptr;
        vertexInputState.vertexBindingDescriptionCount = (uint32)vertexInputBindingDescriptions.size();
        vertexInputState.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();
        vertexInputState.vertexAttributeDescriptionCount = (uint32)vertexInputAttributes.size();
        vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();

        pipelineCreateInfo.stageCount = (uint32)shaderStages.size();
        pipelineCreateInfo.pStages = shaderStages.data();

        pipelineCreateInfo.pVertexInputState = &vertexInputState;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineCreateInfo.pRasterizationState = &rasterizationState;
        pipelineCreateInfo.pColorBlendState = &colorBlendState;
        pipelineCreateInfo.pMultisampleState = &multiSampleState;
        pipelineCreateInfo.pViewportState = &viewportState;
        pipelineCreateInfo.pDepthStencilState = &depthStencilState;
        pipelineCreateInfo.renderPass = vulkanFramebuffer->GetRenderPass();
        pipelineCreateInfo.pDynamicState = &dynamicState;

        VkPipelineCacheCreateInfo pipelineCacheCreate = {};
        pipelineCacheCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        pipelineCacheCreate.pNext = nullptr;
        
        VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreate, nullptr, &m_PipelineCache));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE_CACHE, m_Specification.DebugName, m_PipelineCache);

        VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, m_PipelineCache, 1, &pipelineCreateInfo, nullptr, &m_Pipeline));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE, m_Specification.DebugName, m_Pipeline);
    }
    
    void VulkanVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
    {
        m_VertexBuffers.push_back(vertexBuffer);
    }
    
    void VulkanVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
    {
        m_IndexBuffer = indexBuffer;
    }
}

#endif // HIGHLO_API_VULKAN

