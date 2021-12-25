// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexArray.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanUniformBuffer.h"
#include "VulkanFramebuffer.h"

namespace highlo
{
	namespace utils
	{
		static VkPrimitiveTopology GetVulkanTopology(PrimitiveType type)
		{
			switch (type)
			{
				case PrimitiveType::Triangles:
					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

				case PrimitiveType::TriangleStrip:
					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

				case PrimitiveType::Lines:
					return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

				case PrimitiveType::LineStrip:
					return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

				case PrimitiveType::Patch:
					return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

				case PrimitiveType::None:
					return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
			}

			HL_ASSERT(false);
			return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
		}

		static VkFormat ShaderDataTypeToVulkanFormat(ShaderDataType type)
		{
			switch (type)
			{
				case ShaderDataType::Float:
					return VK_FORMAT_R32_SFLOAT;

				case ShaderDataType::Float2:
					return VK_FORMAT_R32G32_SFLOAT;

				case ShaderDataType::Float3:
					return VK_FORMAT_R32G32B32_SFLOAT;

				case ShaderDataType::Float4:
					return VK_FORMAT_R32G32B32A32_SFLOAT;

				case ShaderDataType::Int:
					return VK_FORMAT_R32_SINT;

				case ShaderDataType::Int2:
					return VK_FORMAT_R32G32_SINT;

				case ShaderDataType::Int3:
					return VK_FORMAT_R32G32B32_SINT;

				case ShaderDataType::Int4:
					return VK_FORMAT_R32G32B32A32_SINT;
			}

			HL_ASSERT(false);
			return VK_FORMAT_UNDEFINED;
		}
	}

	VulkanVertexArray::VulkanVertexArray(const VertexArraySpecification& spec)
		: m_Specification(spec)
	{
		HL_ASSERT(spec.Shader);
		HL_ASSERT(spec.RenderPass);
		Invalidate();
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
	}

	void VulkanVertexArray::Bind() const
	{
	}

	void VulkanVertexArray::Unbind() const
	{
	}

	void VulkanVertexArray::Invalidate()
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		HL_ASSERT(m_Specification.Shader);

		Ref<VulkanShader> vulkanShader = m_Specification.Shader.As<VulkanShader>();
		Ref<VulkanFramebuffer> vulkanFramebuffer = m_Specification.RenderPass->GetSpecification().Framebuffer.As<VulkanFramebuffer>();

		auto descriptorSetLayouts = vulkanShader->GetAllDescriptorSetLayouts();
		const auto &pushConstantRanges = vulkanShader->GetPushConstantRanges();

		std::vector<VkPushConstantRange> vulkanPushConstantRanges(pushConstantRanges.size());
		for (uint32 i = 0; i < pushConstantRanges.size(); ++i)
		{
			const auto &pushConstantRange = pushConstantRanges[i];
			auto &vulkanPushConstantRange = vulkanPushConstantRanges[i];

			vulkanPushConstantRange.stageFlags = pushConstantRange.ShaderStage;
			vulkanPushConstantRange.offset = pushConstantRange.Offset;
			vulkanPushConstantRange.size = pushConstantRange.Size;
		}

		// Create the pipeline layout that is used to generate the rendering pipelines that are based on this descriptor set layout
		// In a more complex scenario you would have different pipeline layouts for different descriptor set layouts that could be reused
		VkPipelineLayoutCreateInfo pipelineLayoutCreate = {};
		pipelineLayoutCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreate.pNext = nullptr;
		pipelineLayoutCreate.setLayoutCount = (uint32)descriptorSetLayouts.size();
		pipelineLayoutCreate.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutCreate.pushConstantRangeCount = (uint32)vulkanPushConstantRanges.size();
		pipelineLayoutCreate.pPushConstantRanges = vulkanPushConstantRanges.data();
		VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreate, nullptr, &m_PipelineLayout));

		VkGraphicsPipelineCreateInfo pipelineCreate = {};
		pipelineCreate.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreate.layout = m_PipelineLayout;
		pipelineCreate.renderPass = vulkanFramebuffer->GetRenderPass();

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
		inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyState.topology = utils::GetVulkanTopology(m_Specification.RenderType);

		VkPipelineRasterizationStateCreateInfo rasterizationState = {};
		rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationState.polygonMode = m_Specification.Wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
		rasterizationState.cullMode = m_Specification.BackfaceCulling ? VK_CULL_MODE_BACK_BIT : VK_CULL_MODE_NONE;
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
			blendAttachmentStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendAttachmentStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
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
				FramebufferBlendMode blendMode = vulkanFramebuffer->GetSpecification().BlendMode == FramebufferBlendMode::None ? attachmentSpec.BlendMode : vulkanFramebuffer->GetSpecification().BlendMode;

				blendAttachmentStates[i].blendEnable = attachmentSpec.Blend ? VK_TRUE : VK_FALSE;
				if (blendMode == FramebufferBlendMode::SrcAlphaOneMinusSrcAlpha)
				{
					blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
					blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
					blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
					blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				}
				else if (blendMode == FramebufferBlendMode::OneZero)
				{
					blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
					blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
					blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
					blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
				}
				else if (blendMode == FramebufferBlendMode::ZeroSrcColor)
				{
					blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
					blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
					blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
					blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
				}
				else
				{
					HL_ASSERT(false);
				}

				blendAttachmentStates[i].colorBlendOp = VK_BLEND_OP_ADD;
				blendAttachmentStates[i].alphaBlendOp = VK_BLEND_OP_ADD;
			}
		}

		VkPipelineColorBlendStateCreateInfo colorBlendState = {};
		colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendState.attachmentCount = (uint32)blendAttachmentStates.size();
		colorBlendState.pAttachments = blendAttachmentStates.data();

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		std::vector<VkDynamicState> dynamicStateEnables;
		dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
		if (m_Specification.RenderType == PrimitiveType::Lines || m_Specification.RenderType == PrimitiveType::LineStrip || m_Specification.Wireframe)
			dynamicStateEnables.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.pDynamicStates = dynamicStateEnables.data();
		dynamicState.dynamicStateCount = static_cast<uint32>(dynamicStateEnables.size());

		// Depth and stencil state containing depth and stencil compare and test operations
		// We only use depth tests and want depth tests and writes to be enabled and compare with less or equal
		VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
		depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilState.depthTestEnable = m_Specification.DepthTest ? VK_TRUE : VK_FALSE;
		depthStencilState.depthWriteEnable = m_Specification.DepthWrite ? VK_TRUE : VK_FALSE;
		depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilState.depthBoundsTestEnable = VK_FALSE;
		depthStencilState.back.failOp = VK_STENCIL_OP_KEEP;
		depthStencilState.back.passOp = VK_STENCIL_OP_KEEP;
		depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
		depthStencilState.stencilTestEnable = VK_FALSE;
		depthStencilState.front = depthStencilState.back;

		// Multi sampling state
		// This example does not make use fo multi sampling (for anti-aliasing), the state must still be set and passed to the pipeline
		VkPipelineMultisampleStateCreateInfo multisampleState = {};
		multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleState.pSampleMask = nullptr;

		// Vertex input descriptor
		BufferLayout &layout = m_Specification.Layout;
		BufferLayout &instanceLayout = m_Specification.InstanceLayout;

		std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;

		VkVertexInputBindingDescription& vertexInputBinding = vertexInputBindingDescriptions.emplace_back();
		vertexInputBinding.binding = 0;
		vertexInputBinding.stride = layout.GetStride();
		vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		if (instanceLayout.GetElementCount())
		{
			VkVertexInputBindingDescription& instanceInputBinding = vertexInputBindingDescriptions.emplace_back();
			instanceInputBinding.binding = 1;
			instanceInputBinding.stride = instanceLayout.GetStride();
			instanceInputBinding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
		}

		// Inpute attribute bindings describe shader attribute locations and memory layouts
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributes(layout.GetElementCount() + instanceLayout.GetElementCount());

		uint32 location = 0;
		for (auto element : layout)
		{
			vertexInputAttributes[location].binding = 0;
			vertexInputAttributes[location].location = location;
			vertexInputAttributes[location].format = utils::ShaderDataTypeToVulkanFormat(element.Type);
			vertexInputAttributes[location].offset = element.Offset;
			location++;
		}

		for (auto element : instanceLayout)
		{
			vertexInputAttributes[location].binding = 1;
			vertexInputAttributes[location].location = location;
			vertexInputAttributes[location].format = utils::ShaderDataTypeToVulkanFormat(element.Type);
			vertexInputAttributes[location].offset = element.Offset;
			location++;
		}

		// Vertex input state used for pipeline creation
		VkPipelineVertexInputStateCreateInfo vertexInputState = {};
		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexBindingDescriptionCount = (uint32)vertexInputBindingDescriptions.size();
		vertexInputState.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();
		vertexInputState.vertexAttributeDescriptionCount = (uint32)vertexInputAttributes.size();
		vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();

		const auto& shaderStages = vulkanShader->GetPipelineShaderStageCreateInfos();

		// Set pipeline shader stage info
		pipelineCreate.stageCount = static_cast<uint32>(shaderStages.size());
		pipelineCreate.pStages = shaderStages.data();

		// Assign the pipeline states to the pipeline creation info structure
		pipelineCreate.pVertexInputState = &vertexInputState;
		pipelineCreate.pInputAssemblyState = &inputAssemblyState;
		pipelineCreate.pRasterizationState = &rasterizationState;
		pipelineCreate.pColorBlendState = &colorBlendState;
		pipelineCreate.pMultisampleState = &multisampleState;
		pipelineCreate.pViewportState = &viewportState;
		pipelineCreate.pDepthStencilState = &depthStencilState;
		pipelineCreate.renderPass = vulkanFramebuffer->GetRenderPass();
		pipelineCreate.pDynamicState = &dynamicState;

		// What is this pipeline cache?
		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VkPipelineCache pipelineCache;
		VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &pipelineCache));

		// Create rendering pipeline using the specified states
		VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCreate, nullptr, &m_Pipeline));
	}

	void VulkanVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
	{
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VulkanVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}
	
	void VulkanVertexArray::SetUniformBuffer(Ref<UniformBuffer>& uniformBuffer, uint32 binding, uint32 set)
	{
		Ref<VulkanUniformBuffer> vulkanUniformBuffer = uniformBuffer.As<VulkanUniformBuffer>();
		HL_ASSERT(m_DescriptorSets.DescriptorSets.size() > set);

		VkWriteDescriptorSet writeDescriptorSet = {};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.descriptorCount = 1;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSet.pBufferInfo = &vulkanUniformBuffer->GetDescriptorBufferInfo();
		writeDescriptorSet.dstBinding = binding;
		writeDescriptorSet.dstSet = m_DescriptorSets.DescriptorSets[set];

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
	}
}

#endif // HIGHLO_API_VULKAN

