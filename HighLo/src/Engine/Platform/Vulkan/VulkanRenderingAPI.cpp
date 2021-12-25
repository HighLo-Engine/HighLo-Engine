// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#include "Engine/Renderer/SceneRenderer.h"
#include "Engine/Renderer/Shaders/UniformBufferSet.h"
#include "Engine/Renderer/Shaders/StorageBufferSet.h"
#include "Engine/Utils/ImageUtils.h"
#include "Engine/Assets/AssetMaterial.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanShader.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"
#include "VulkanComputePipeline.h"
#include "VulkanTexture2D.h"
#include "VulkanTexture3D.h"
#include "VulkanVertexArray.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"
#include "VulkanStorageBuffer.h"

namespace highlo
{
	namespace utils
	{
		static const char *VulkanVendorIDToString(uint32 vendorID)
		{
			switch (vendorID)
			{
				case 0x10DE: return "NVIDIA";
				case 0x1002: return "AMD";
				case 0x8086: return "INTEL";
				case 0x13B5: return "ARM";
			}
			return "Unknown";
		}
	}

	struct VulkanRendererData
	{
		int32 SelectedDrawCall = -1;
		int32 DrawCallCount = 0;

		std::unordered_map<SceneRenderer*, std::vector<VulkanShader::ShaderMaterialDescriptorSet>> RenderDescriptorSets;
		VkDescriptorSet ActiveRenderDescriptorSet = nullptr;
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32> DescriptorPoolAllocationCounts;

		// Default Sampler
		VkSampler SamplerClamp = nullptr;

		// UniformBufferSet -> Shader Hash -> Frame -> WriteDescriptor
		std::unordered_map<UniformBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> UniformBufferWriteDescriptorCache;
		std::unordered_map<StorageBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> StorageBufferWriteDescriptorCache;

		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		VulkanShader::ShaderMaterialDescriptorSet QuadDescriptorSet;
	};

	struct VulkanQuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	static VulkanRendererData *s_VulkanRendererData = nullptr;

	void VulkanRenderingAPI::Init()
	{
		s_VulkanRendererData = new VulkanRendererData();

		const auto &config = Renderer::GetConfig();
		auto &properties = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetProperties();
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		s_VulkanRendererData->DescriptorPools.resize(config.FramesInFlight);
		s_VulkanRendererData->DescriptorPoolAllocationCounts.resize(config.FramesInFlight);

		auto &caps = Renderer::GetCapabilities();
		caps.Vendor = utils::VulkanVendorIDToString(properties.vendorID);
		caps.Device = properties.deviceName;
		caps.Version = HLString::ToString(properties.driverVersion);
		caps.MaxSamples = properties.limits.maxSamplerAllocationCount;
		caps.MaxAnisotropy = properties.limits.maxSamplerAnisotropy;
		
		// TODO
		//caps.MaxTextureUnits = properties.limits.;

		utils::DumpGPUInfos();

		// Create Descriptor Pool
		VkDescriptorPoolSize poolSizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 100000;
		poolInfo.poolSizeCount = (uint32)IM_ARRAYSIZE(poolSizes);
		poolInfo.pPoolSizes = poolSizes;
		for (uint32 i = 0; i < config.FramesInFlight; i++)
		{
			VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &s_VulkanRendererData->DescriptorPools[i]));
			s_VulkanRendererData->DescriptorPoolAllocationCounts[i] = 0;
		}

		// Create fullscreen quad
		float x = -1;
		float y = -1;
		float width = 2, height = 2;

		VulkanQuadVertex *data = new VulkanQuadVertex[4];

		data[0].Position = glm::vec3(x, y, 0.0f);
		data[0].TexCoord = glm::vec2(0, 0);

		data[1].Position = glm::vec3(x + width, y, 0.0f);
		data[1].TexCoord = glm::vec2(1, 0);

		data[2].Position = glm::vec3(x + width, y + height, 0.0f);
		data[2].TexCoord = glm::vec2(1, 1);

		data[3].Position = glm::vec3(x, y + height, 0.0f);
		data[3].TexCoord = glm::vec2(0, 1);

		s_VulkanRendererData->QuadVertexBuffer = VertexBuffer::Create(data, 4 * sizeof(VulkanQuadVertex));
		
		uint32 indices[6] = { 0, 1, 2, 2, 3, 0, };
		s_VulkanRendererData->QuadIndexBuffer = IndexBuffer::Create(indices, 6 * sizeof(uint32));
	}
	
	void VulkanRenderingAPI::Shutdown()
	{
		VulkanShader::ClearUniformBuffers();
		delete s_VulkanRendererData;
	}
	
	void VulkanRenderingAPI::BeginFrame()
	{
		Ref<VulkanSwapChain> &swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();

		// Reset descriptor pools here
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		uint32 bufferIndex = swapChain->GetCurrentBufferIndex();
		vkResetDescriptorPool(device, s_VulkanRendererData->DescriptorPools[bufferIndex], 0);
		memset(s_VulkanRendererData->DescriptorPoolAllocationCounts.data(), 0, s_VulkanRendererData->DescriptorPoolAllocationCounts.size() * sizeof(uint32));

		s_VulkanRendererData->DrawCallCount = 0;
	}
	
	void VulkanRenderingAPI::EndFrame()
	{
	}

	void VulkanRenderingAPI::BeginRenderPass(Ref<CommandBuffer> renderCommandBuffer, Ref<RenderPass> renderPass, bool shouldClear)
	{
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		auto fb = renderPass->GetSpecification().Framebuffer;
		Ref<VulkanFramebuffer> framebuffer = fb.As<VulkanFramebuffer>();
		const auto &fbSpec = framebuffer->GetSpecification();

		uint32 width = framebuffer->GetWidth();
		uint32 height = framebuffer->GetHeight();

		VkViewport viewport = {};
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = framebuffer->GetRenderPass();
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;

		if (framebuffer->GetSpecification().SwapChainTarget)
		{
			Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain();
			width = swapChain->GetWidth();
			height = swapChain->GetHeight();
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.pNext = nullptr;
			renderPassBeginInfo.renderPass = framebuffer->GetRenderPass();
			renderPassBeginInfo.renderArea.offset.x = 0;
			renderPassBeginInfo.renderArea.offset.y = 0;
			renderPassBeginInfo.renderArea.extent.width = width;
			renderPassBeginInfo.renderArea.extent.height = height;
			renderPassBeginInfo.framebuffer = swapChain->GetCurrentFramebuffer();
			
			viewport.x = 0.0f;
			viewport.y = (float)height;
			viewport.width = (float)width;
			viewport.height = -(float)height;
		}
		else
		{
			width = framebuffer->GetWidth();
			height = framebuffer->GetHeight();
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.pNext = nullptr;
			renderPassBeginInfo.renderPass = framebuffer->GetRenderPass();
			renderPassBeginInfo.renderArea.offset.x = 0;
			renderPassBeginInfo.renderArea.offset.y = 0;
			renderPassBeginInfo.renderArea.extent.width = width;
			renderPassBeginInfo.renderArea.extent.height = height;
			renderPassBeginInfo.framebuffer = framebuffer->GetVulkanFramebuffer();

			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)width;
			viewport.height = (float)height;
		}

		// TODO: Does our framebuffer have a depth attachment?
		const auto &clearValues = framebuffer->GetVulkanClearValues();
		renderPassBeginInfo.clearValueCount = (uint32)clearValues.size();
		renderPassBeginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		if (shouldClear)
		{
			const uint32 colorAttachmentCount = (uint32)framebuffer->GetColorAttachmentCount();
			const uint32 totalAttachmentCount = colorAttachmentCount + (framebuffer->HasDepthAttachment() ? 1 : 0);
			HL_ASSERT(clearValues.size() == totalAttachmentCount);

			std::vector<VkClearAttachment> attachments(totalAttachmentCount);
			std::vector<VkClearRect> clearRects(totalAttachmentCount);
			for (uint32 i = 0; i < colorAttachmentCount; i++)
			{
				attachments[i].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				attachments[i].colorAttachment = i;
				attachments[i].clearValue = clearValues[i];

				clearRects[i].rect.offset = { (int32_t)0, (int32_t)0 };
				clearRects[i].rect.extent = { width, height };
				clearRects[i].baseArrayLayer = 0;
				clearRects[i].layerCount = 1;
			}

			if (framebuffer->HasDepthAttachment())
			{
				attachments[colorAttachmentCount].aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
				attachments[colorAttachmentCount].clearValue = clearValues[colorAttachmentCount];
				clearRects[colorAttachmentCount].rect.offset = { (int32_t)0, (int32_t)0 };
				clearRects[colorAttachmentCount].rect.extent = { width, height };
				clearRects[colorAttachmentCount].baseArrayLayer = 0;
				clearRects[colorAttachmentCount].layerCount = 1;
			}

			vkCmdClearAttachments(commandBuffer, totalAttachmentCount, attachments.data(), totalAttachmentCount, clearRects.data());
		}

		// Update dynamic viewport state
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		// Update dynamic scissor state
		VkRect2D scissor = {};
		scissor.extent.width = width;
		scissor.extent.height = height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void VulkanRenderingAPI::EndRenderPass(Ref<CommandBuffer> renderCommandBuffer)
	{
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		vkCmdEndRenderPass(commandBuffer);
	}

	void VulkanRenderingAPI::RenderDynamicModel(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va,
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<DynamicModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materialTable, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformOffset, 
		uint32 instanceCount)
	{
		HL_ASSERT(model);
		HL_ASSERT(materialTable);

		if (s_VulkanRendererData->SelectedDrawCall != -1 && s_VulkanRendererData->DrawCallCount > s_VulkanRendererData->SelectedDrawCall)
			return;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		Ref<MeshFile> meshAsset = model->Get();
		Ref<VulkanVertexBuffer> vulkanMeshVB = meshAsset->GetVertexBuffer().As<VulkanVertexBuffer>();
		VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

		Ref<VulkanVertexBuffer> vulkanTransformBuffer = transformBuffer.As<VulkanVertexBuffer>();
		VkBuffer vbTransformBuffer = vulkanTransformBuffer->GetVulkanBuffer();
		VkDeviceSize instanceOffsets[1] = { transformOffset };
		vkCmdBindVertexBuffers(commandBuffer, 1, 1, &vbTransformBuffer, instanceOffsets);

		auto vulkanMeshIB = Ref<VulkanIndexBuffer>(meshAsset->GetIndexBuffer());
		VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

		Ref<VulkanVertexArray> vulkanPipeline = va.As<VulkanVertexArray>();
		VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		std::vector<std::vector<VkWriteDescriptorSet>> writeDescriptors;

		const auto &meshAssetSubmeshes = meshAsset->GetSubmeshes();
		const Mesh &submesh = meshAssetSubmeshes[submeshIndex];
		auto &meshMaterialTable = model->GetMaterials();
		uint32 materialCount = meshMaterialTable->GetMaterialCount();
		Ref<MaterialAsset> material = materialTable->HasMaterial(submesh.MaterialIndex) ? materialTable->GetMaterial(submesh.MaterialIndex) : meshMaterialTable->GetMaterial(submesh.MaterialIndex);
		Ref<VulkanMaterial> vulkanMaterial = material->GetMaterial().As<VulkanMaterial>();
		UpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);

		if (s_VulkanRendererData->SelectedDrawCall != -1 && s_VulkanRendererData->DrawCallCount > s_VulkanRendererData->SelectedDrawCall)
			return;

		VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();
		VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(frameIndex);

		// NOTE: Descriptor Set 1 is owned by the renderer
		std::array<VkDescriptorSet, 2> descriptorSets =
		{
			descriptorSet,
			s_VulkanRendererData->ActiveRenderDescriptorSet
		};

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, (uint32)descriptorSets.size(), descriptorSets.data(), 0, nullptr);

		Allocator uniformStorageBuffer = vulkanMaterial->GetUniformStorageBuffer();
		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, uniformStorageBuffer.m_Size, uniformStorageBuffer.m_Data);
		vkCmdDrawIndexed(commandBuffer, submesh.IndexCount, instanceCount, submesh.BaseIndex, submesh.BaseVertex, 0);
		s_VulkanRendererData->DrawCallCount++;
	}

	void VulkanRenderingAPI::RenderStaticModel(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<StaticModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materialTable, 
		const Transform &transform)
	{
		HL_ASSERT(model);
		HL_ASSERT(materialTable);

		if (s_VulkanRendererData->SelectedDrawCall != -1 && s_VulkanRendererData->DrawCallCount > s_VulkanRendererData->SelectedDrawCall)
			return;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		Ref<MeshFile> meshAsset = model->Get();
		Ref<VulkanVertexBuffer> vulkanMeshVB = meshAsset->GetVertexBuffer().As<VulkanVertexBuffer>();
		VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

		auto vulkanMeshIB = Ref<VulkanIndexBuffer>(meshAsset->GetIndexBuffer());
		VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

		Ref<VulkanVertexArray> vulkanPipeline = va.As<VulkanVertexArray>();
		VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		std::vector<std::vector<VkWriteDescriptorSet>> writeDescriptors;

		const auto &meshAssetSubmeshes = meshAsset->GetSubmeshes();
		const Mesh &submesh = meshAssetSubmeshes[submeshIndex];
		auto &meshMaterialTable = model->GetMaterials();
		uint32 materialCount = meshMaterialTable->GetMaterialCount();
		Ref<MaterialAsset> material = materialTable->HasMaterial(submesh.MaterialIndex) ? materialTable->GetMaterial(submesh.MaterialIndex) : meshMaterialTable->GetMaterial(submesh.MaterialIndex);
		Ref<VulkanMaterial> vulkanMaterial = material->GetMaterial().As<VulkanMaterial>();
		UpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);

		if (s_VulkanRendererData->SelectedDrawCall != -1 && s_VulkanRendererData->DrawCallCount > s_VulkanRendererData->SelectedDrawCall)
			return;

		VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();
		VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(frameIndex);

		// NOTE: Descriptor Set 1 is owned by the renderer
		std::array<VkDescriptorSet, 2> descriptorSets = {
			descriptorSet,
			s_VulkanRendererData->ActiveRenderDescriptorSet
		};

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, (uint32)descriptorSets.size(), descriptorSets.data(), 0, nullptr);

		Allocator uniformStorageBuffer = vulkanMaterial->GetUniformStorageBuffer();
		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform.GetTransform());
		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(glm::mat4), uniformStorageBuffer.m_Size, uniformStorageBuffer.m_Data);
		vkCmdDrawIndexed(commandBuffer, submesh.IndexCount, 1, submesh.BaseIndex, submesh.BaseVertex, 0);
		s_VulkanRendererData->DrawCallCount++;
	}

	void VulkanRenderingAPI::RenderDynamicModelWithMaterial(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<DynamicModel> model, 
		uint32 submeshIndex, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformOffset, 
		uint32 instanceCount, 
		Ref<Material> material, 
		Allocator additionalUniforms)
	{
		HL_ASSERT(model);
		HL_ASSERT(model->Get());

		Allocator pushConstantBuffer;
		if (additionalUniforms.m_Size)
		{
			pushConstantBuffer.Allocate(additionalUniforms.m_Size);
			if (additionalUniforms.m_Size)
				pushConstantBuffer.Write(additionalUniforms.m_Data, additionalUniforms.m_Size);
		}

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);
		Ref<VulkanMaterial> vulkanMaterial = material.As<VulkanMaterial>();

		Ref<MeshFile> meshAsset = model->Get();
		auto vulkanMeshVB = meshAsset->GetVertexBuffer().As<VulkanVertexBuffer>();
		VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
		VkDeviceSize vertexOffsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, vertexOffsets);

		Ref<VulkanVertexBuffer> vulkanTransformBuffer = transformBuffer.As<VulkanVertexBuffer>();
		VkBuffer vbTransformBuffer = vulkanTransformBuffer->GetVulkanBuffer();
		VkDeviceSize instanceOffsets[1] = { transformOffset };
		vkCmdBindVertexBuffers(commandBuffer, 1, 1, &vbTransformBuffer, instanceOffsets);

		auto vulkanMeshIB = Ref<VulkanIndexBuffer>(meshAsset->GetIndexBuffer());
		VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

		UpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);

		Ref<VulkanVertexArray> vulkanPipeline = va.As<VulkanVertexArray>();
		VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
		VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		float lineWidth = vulkanPipeline->GetSpecification().LineWidth;
		if (lineWidth != 1.0f)
			vkCmdSetLineWidth(commandBuffer, lineWidth);

		// Bind descriptor sets describing shader binding points
		VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(frameIndex);
		if (descriptorSet)
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);

		Allocator uniformStorageBuffer = vulkanMaterial->GetUniformStorageBuffer();
		if (uniformStorageBuffer)
			vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, uniformStorageBuffer.m_Size, uniformStorageBuffer.m_Data);

		const auto &meshAssetSubmeshes = meshAsset->GetSubmeshes();
		const Mesh &submesh = meshAssetSubmeshes[submeshIndex];

		if (pushConstantBuffer.m_Size)
			vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, pushConstantBuffer.m_Size, pushConstantBuffer.m_Data);

		vkCmdDrawIndexed(commandBuffer, submesh.IndexCount, instanceCount, submesh.BaseIndex, submesh.BaseVertex, 0);
		pushConstantBuffer.Release();
	}

	void VulkanRenderingAPI::RenderStaticModelWithMaterial(
		Ref<CommandBuffer> renderCommandBuffer,
		Ref<VertexArray> va,
		Ref<UniformBufferSet> uniformBufferSet,
		Ref<StorageBufferSet> storageBufferSet,
		Ref<StaticModel> model,
		uint32 submeshIndex,
		Ref<MaterialTable> materialTable,
		const Transform &transform)
	{
		HL_ASSERT(model);
		HL_ASSERT(materialTable);

		if (s_VulkanRendererData->SelectedDrawCall != -1 && s_VulkanRendererData->DrawCallCount > s_VulkanRendererData->SelectedDrawCall)
			return;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		Ref<MeshFile> meshAsset = model->Get();
		Ref<VulkanVertexBuffer> vulkanMeshVB = meshAsset->GetVertexBuffer().As<VulkanVertexBuffer>();
		VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

		auto vulkanMeshIB = Ref<VulkanIndexBuffer>(meshAsset->GetIndexBuffer());
		VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

		Ref<VulkanVertexArray> vulkanPipeline = va.As<VulkanVertexArray>();
		VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		std::vector<std::vector<VkWriteDescriptorSet>> writeDescriptors;

		const auto &meshAssetSubmeshes = meshAsset->GetSubmeshes();
		const Mesh &submesh = meshAssetSubmeshes[submeshIndex];
		auto &meshMaterialTable = model->GetMaterials();
		uint32 materialCount = meshMaterialTable->GetMaterialCount();
		Ref<MaterialAsset> material = materialTable->HasMaterial(submesh.MaterialIndex) ? materialTable->GetMaterial(submesh.MaterialIndex) : meshMaterialTable->GetMaterial(submesh.MaterialIndex);
		Ref<VulkanMaterial> vulkanMaterial = material->GetMaterial().As<VulkanMaterial>();
		
		UpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);

		if (s_VulkanRendererData->SelectedDrawCall != -1 && s_VulkanRendererData->DrawCallCount > s_VulkanRendererData->SelectedDrawCall)
			return;

		VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();
		VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(frameIndex);

		// NOTE: Descriptor Set 1 is owned by the renderer
		std::array<VkDescriptorSet, 2> descriptorSets = {
			descriptorSet,
			s_VulkanRendererData->ActiveRenderDescriptorSet
		};

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, (uint32)descriptorSets.size(), descriptorSets.data(), 0, nullptr);

		Allocator uniformStorageBuffer = vulkanMaterial->GetUniformStorageBuffer();
		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform.GetTransform());
		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(glm::mat4), uniformStorageBuffer.m_Size, uniformStorageBuffer.m_Data);
		vkCmdDrawIndexed(commandBuffer, submesh.IndexCount, 1, submesh.BaseIndex, submesh.BaseVertex, 0);
		s_VulkanRendererData->DrawCallCount++;
	}

	void VulkanRenderingAPI::RenderQuad(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<Material> material, 
		const Transform &transform)
	{
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		Ref<VulkanVertexArray> vulkanPipeline = va.As<VulkanVertexArray>();
		Ref<VulkanMaterial> vulkanMaterial = material.As<VulkanMaterial>();

		VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();

		auto vulkanMeshVB = s_VulkanRendererData->QuadVertexBuffer.As<VulkanVertexBuffer>();
		VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

		auto vulkanMeshIB = s_VulkanRendererData->QuadIndexBuffer.As<VulkanIndexBuffer>();
		VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

		VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		UpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);

		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(bufferIndex);
		if (descriptorSet)
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);

		Allocator uniformStorageBuffer = vulkanMaterial->GetUniformStorageBuffer();

		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform.GetTransform());
		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(glm::mat4), uniformStorageBuffer.m_Size, uniformStorageBuffer.m_Data);
		vkCmdDrawIndexed(commandBuffer, s_VulkanRendererData->QuadIndexBuffer->GetCount(), 1, 0, 0, 0);
	}

	void VulkanRenderingAPI::RenderGeometry(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<Material> material, 
		Ref<VertexBuffer> vertexBuffer, 
		Ref<IndexBuffer> indexBuffer, 
		const Transform &transform,
		uint32 indexCount)
	{
		Ref<VulkanMaterial> vulkanMaterial = material.As<VulkanMaterial>();
		if (indexCount == 0)
			indexCount = indexBuffer->GetCount();

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		Ref<VulkanVertexArray> vulkanPipeline = va.As<VulkanVertexArray>();

		VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();

		auto vulkanMeshVB = vertexBuffer.As<VulkanVertexBuffer>();
		VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

		auto vulkanMeshIB = indexBuffer.As<VulkanIndexBuffer>();
		VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

		VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		const auto &writeDescriptors = RetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, vulkanMaterial);
		vulkanMaterial->UpdateForRendering(writeDescriptors);

		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(bufferIndex);
		if (descriptorSet)
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);

		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform.GetTransform());
		Allocator uniformStorageBuffer = vulkanMaterial->GetUniformStorageBuffer();
		if (uniformStorageBuffer)
			vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(glm::mat4), uniformStorageBuffer.m_Size, uniformStorageBuffer.m_Data);

		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}

	void VulkanRenderingAPI::SubmitFullscreenQuad(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<Material> material)
	{
		SubmitFullscreenQuad(renderCommandBuffer, va, uniformBufferSet, nullptr, material);
	}

	void VulkanRenderingAPI::SubmitFullscreenQuad(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<Material> material)
	{
		Ref<VulkanMaterial> vulkanMaterial = material.As<VulkanMaterial>();

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		Ref<VulkanVertexArray> vulkanPipeline = va.As<VulkanVertexArray>();
		VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();

		auto vulkanMeshVB = s_VulkanRendererData->QuadVertexBuffer.As<VulkanVertexBuffer>();
		VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

		auto vulkanMeshIB = s_VulkanRendererData->QuadIndexBuffer.As<VulkanIndexBuffer>();
		VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

		VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		UpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);

		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(bufferIndex);
		if (descriptorSet)
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);

		Allocator uniformStorageBuffer = vulkanMaterial->GetUniformStorageBuffer();
		if (uniformStorageBuffer.m_Size)
			vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, uniformStorageBuffer.m_Size, uniformStorageBuffer.m_Data);

		vkCmdDrawIndexed(commandBuffer, s_VulkanRendererData->QuadIndexBuffer->GetCount(), 1, 0, 0, 0);
	}

	void VulkanRenderingAPI::SubmitFullscreenQuadWithOverrides(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<Material> material, 
		Allocator vertexShaderOverrides, 
		Allocator fragmentShaderOverrides)
	{
		Allocator vertexPushConstantBuffer;
		if (vertexShaderOverrides)
		{
			vertexPushConstantBuffer.Allocate(vertexShaderOverrides.m_Size);
			vertexPushConstantBuffer.Write(vertexShaderOverrides.m_Data, vertexShaderOverrides.m_Size);
		}

		Allocator fragmentPushConstantBuffer;
		if (fragmentPushConstantBuffer)
		{
			fragmentPushConstantBuffer.Allocate(fragmentShaderOverrides.m_Size);
			fragmentPushConstantBuffer.Write(fragmentShaderOverrides.m_Data, fragmentShaderOverrides.m_Size);
		}

		Ref<VulkanMaterial> vulkanMaterial = material.As<VulkanMaterial>();

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		Ref<VulkanVertexArray> vulkanPipeline = va.As<VulkanVertexArray>();

		VkPipelineLayout layout = vulkanPipeline->GetVulkanPipelineLayout();

		auto vulkanMeshVB = s_VulkanRendererData->QuadVertexBuffer.As<VulkanVertexBuffer>();
		VkBuffer vbMeshBuffer = vulkanMeshVB->GetVulkanBuffer();
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

		auto vulkanMeshIB = s_VulkanRendererData->QuadIndexBuffer.As<VulkanIndexBuffer>();
		VkBuffer ibBuffer = vulkanMeshIB->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

		VkPipeline pipeline = vulkanPipeline->GetVulkanPipeline();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		if (uniformBufferSet)
		{
			const auto &writeDescriptors = RetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, vulkanMaterial);
			vulkanMaterial->UpdateForRendering(writeDescriptors);
		}
		else
		{
			vulkanMaterial->UpdateForRendering();
		}

		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(bufferIndex);
		if (descriptorSet)
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);

		if (vertexPushConstantBuffer)
			vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, vertexPushConstantBuffer.m_Size, vertexPushConstantBuffer.m_Data);
		if (fragmentPushConstantBuffer)
			vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, vertexPushConstantBuffer.m_Size, fragmentPushConstantBuffer.m_Size, fragmentPushConstantBuffer.m_Data);

		vkCmdDrawIndexed(commandBuffer, s_VulkanRendererData->QuadIndexBuffer->GetCount(), 1, 0, 0, 0);

		vertexPushConstantBuffer.Release();
		fragmentPushConstantBuffer.Release();
	}

	void VulkanRenderingAPI::DispatchComputeShader(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<ComputePipeline> computePipeline, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<Material> material, 
		const glm::ivec3 &groups)
	{
		auto vulkanMaterial = material.As<VulkanMaterial>();
		auto vulkanPipeline = computePipeline.As<VulkanComputePipeline>();
		const uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		UpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);

		const VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(frameIndex);

		vulkanPipeline->Begin(renderCommandBuffer);
		vulkanPipeline->Dispatch(descriptorSet, groups.x, groups.y, groups.z);
		vulkanPipeline->End();
	}

	void VulkanRenderingAPI::ClearTexture(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<Texture2D> texture)
	{
		Ref<VulkanTexture2D> vulkanTexture = texture.As<VulkanTexture2D>();

		const auto vulkanCommandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrameIndex());
		VkImageSubresourceRange subresourceRange{};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = texture->GetSpecification().Mips;
		subresourceRange.layerCount = texture->GetSpecification().Layers;

		 VkClearColorValue clearColor{ 0.f, 0.f, 0.f, 0.f };
		 vkCmdClearColorImage(vulkanCommandBuffer, vulkanTexture->GetTextureInfo().Image, vulkanTexture->GetSpecification().Usage == TextureUsage::Storage ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &clearColor, 1, &subresourceRange);
	}

	Ref<Environment> VulkanRenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		if (!Renderer::GetConfig().ComputeEnvironmentMaps)
			return Renderer::GetEmptyEnvironment();

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		Ref<Texture2D> envEquirect = Texture2D::LoadFromFile(filePath);
		HL_ASSERT(envEquirect->GetFormat() == TextureFormat::RGBA32F, "Texture is not HDR!");

		Ref<Texture3D> envUnfiltered = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);
		Ref<Texture3D> envFiltered = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);

		// ========================================================================================================================================
		// ============================================ Convert equirectangular to cubemap ========================================================
		// ========================================================================================================================================
		Ref<VulkanComputePipeline> equirectangularConversionPipeline = Ref<VulkanComputePipeline>::Create(Renderer::GetShaderLibrary()->Get("EquirectangularToCubeMap"));
		Ref<VulkanShader> equirectangularConversionShader = equirectangularConversionPipeline->GetShader();


		std::array<VkWriteDescriptorSet, 2> equirectangularConversionWriteDescriptors;
		auto equirectangularConversionDescriptorSet = equirectangularConversionShader->CreateDescriptorSets();

		Ref<VulkanTexture3D> envUnfilteredCubemap = envUnfiltered.As<VulkanTexture3D>();
		equirectangularConversionWriteDescriptors[0] = *equirectangularConversionShader->GetDescriptorSet("o_CubeMap");
		equirectangularConversionWriteDescriptors[0].dstSet = equirectangularConversionDescriptorSet.DescriptorSets[0];
		equirectangularConversionWriteDescriptors[0].pImageInfo = &envUnfilteredCubemap->GetVulkanDescriptorInfo();

		Ref<VulkanTexture2D> envEquirectVK = envEquirect.As<VulkanTexture2D>();
		equirectangularConversionWriteDescriptors[1] = *equirectangularConversionShader->GetDescriptorSet("o_EquirectangularTex");
		equirectangularConversionWriteDescriptors[1].dstSet = equirectangularConversionDescriptorSet.DescriptorSets[0];
		equirectangularConversionWriteDescriptors[1].pImageInfo = &envEquirectVK->GetVulkanDescriptorInfo();

		vkUpdateDescriptorSets(device, (uint32)equirectangularConversionWriteDescriptors.size(), equirectangularConversionWriteDescriptors.data(), 0, NULL);
		equirectangularConversionPipeline->Execute(equirectangularConversionDescriptorSet.DescriptorSets.data(),
												   (uint32)equirectangularConversionDescriptorSet.DescriptorSets.size(),
												   cubemapSize / 32,
												   cubemapSize / 32,
												   6);

		envUnfilteredCubemap->GenerateMips(true);

		// ========================================================================================================================================
		// ===================================== Create Environment Mips and calculate compute passes =============================================
		// ========================================================================================================================================

		Ref<VulkanComputePipeline> environmentMipFilterPipeline = Ref<VulkanComputePipeline>::Create(Renderer::GetShaderLibrary()->Get("EnvironmentMipFilter"));
		Ref<VulkanShader> environmentMipFilterShader = environmentMipFilterPipeline->GetShader();

		Ref<VulkanTexture3D> envFilteredCubemap = envFiltered.As<VulkanTexture3D>();
		VkDescriptorImageInfo imageInfo = envFilteredCubemap->GetVulkanDescriptorInfo();

		uint32 mipCount = utils::CalculateMipCount(cubemapSize, cubemapSize);

		std::vector<VkWriteDescriptorSet> environmentMipWriteDescriptors(mipCount * 2);
		std::vector<VkDescriptorImageInfo> mipImageInfos(mipCount);
		auto environmentMipFilterDescriptorSet = environmentMipFilterShader->CreateDescriptorSets(0, 12);
		for (uint32 i = 0; i < mipCount; i++)
		{
			VkDescriptorImageInfo &mipImageInfo = mipImageInfos[i];
			mipImageInfo = imageInfo;
			mipImageInfo.imageView = envFilteredCubemap->CreateImageViewSingleMip(i);

			environmentMipWriteDescriptors[i * 2 + 0] = *environmentMipFilterShader->GetDescriptorSet("outputTexture");
			environmentMipWriteDescriptors[i * 2 + 0].dstSet = environmentMipFilterDescriptorSet.DescriptorSets[i]; // Should this be set inside the shader?
			environmentMipWriteDescriptors[i * 2 + 0].pImageInfo = &mipImageInfo;

			Ref<VulkanTexture3D> envUnfilteredCubemap = envUnfiltered.As<VulkanTexture3D>();
			environmentMipWriteDescriptors[i * 2 + 1] = *environmentMipFilterShader->GetDescriptorSet("inputTexture");
			environmentMipWriteDescriptors[i * 2 + 1].dstSet = environmentMipFilterDescriptorSet.DescriptorSets[i]; // Should this be set inside the shader?
			environmentMipWriteDescriptors[i * 2 + 1].pImageInfo = &envUnfilteredCubemap->GetVulkanDescriptorInfo();
		}

		vkUpdateDescriptorSets(device, (uint32)environmentMipWriteDescriptors.size(), environmentMipWriteDescriptors.data(), 0, NULL);

		environmentMipFilterPipeline->Begin(); // begin compute pass
		const float deltaRoughness = 1.0f / glm::max((float)envFiltered->GetMipLevelCount() - 1.0f, 1.0f);
		for (uint32 i = 0, size = cubemapSize; i < mipCount; i++, size /= 2)
		{
			uint32 numGroups = glm::max(1u, size / 32);
			float roughness = i * deltaRoughness;
			roughness = glm::max(roughness, 0.05f);
			environmentMipFilterPipeline->SetPushConstants(&roughness, sizeof(float));
			environmentMipFilterPipeline->Dispatch(environmentMipFilterDescriptorSet.DescriptorSets[i], numGroups, numGroups, 6);
		}
		environmentMipFilterPipeline->End();

		// ========================================================================================================================================
		// =================================================== Create Irradiance map ==============================================================
		// ========================================================================================================================================

		Ref<Texture3D> irradianceMap = Texture3D::Create(TextureFormat::RGBA32F, irradianceMapSize, irradianceMapSize);
		Ref<VulkanComputePipeline> environmentIrradiancePipeline = Ref<VulkanComputePipeline>::Create(Renderer::GetShaderLibrary()->Get("EnvironmentIrradiance"));
		Ref<VulkanShader> environmentIrradianceShader = environmentIrradiancePipeline->GetShader();

		Ref<VulkanTexture3D> irradianceCubemap = irradianceMap.As<VulkanTexture3D>();
		auto environmentIrradianceDescriptorSet = environmentIrradianceShader->CreateDescriptorSets();

		std::array<VkWriteDescriptorSet, 2> irradianceWriteDescriptors;
		irradianceWriteDescriptors[0] = *environmentIrradianceShader->GetDescriptorSet("o_IrradianceMap");
		irradianceWriteDescriptors[0].dstSet = environmentIrradianceDescriptorSet.DescriptorSets[0];
		irradianceWriteDescriptors[0].pImageInfo = &irradianceCubemap->GetVulkanDescriptorInfo();

		irradianceWriteDescriptors[1] = *environmentIrradianceShader->GetDescriptorSet("u_RadianceMap");
		irradianceWriteDescriptors[1].dstSet = environmentIrradianceDescriptorSet.DescriptorSets[0];
		irradianceWriteDescriptors[1].pImageInfo = &envFilteredCubemap->GetVulkanDescriptorInfo();

		vkUpdateDescriptorSets(device, (uint32)irradianceWriteDescriptors.size(), irradianceWriteDescriptors.data(), 0, NULL);
		environmentIrradiancePipeline->Begin();
		environmentIrradiancePipeline->SetPushConstants(&Renderer::GetConfig().IrradianceMapComputeSamples, sizeof(uint32));
		environmentIrradiancePipeline->Dispatch(environmentIrradianceDescriptorSet.DescriptorSets[0], irradianceMap->GetWidth() / 32, irradianceMap->GetHeight() / 32, 6);
		environmentIrradiancePipeline->End();

		irradianceCubemap->GenerateMips();

		return Ref<Environment>::Create(filePath, envUnfiltered, envFiltered, irradianceMap, Renderer::GetBRDFLutTexture());
	}
	
	void VulkanRenderingAPI::UpdateMaterialForRendering(Ref<VulkanMaterial> material, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet)
	{
		if (uniformBufferSet)
		{
			auto writeDescriptors = RetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, material);
			if (storageBufferSet)
			{
				const auto &storageBufferWriteDescriptors = RetrieveOrCreateStorageBufferWriteDescriptors(storageBufferSet, material);

				const uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
				for (uint32 frame = 0; frame < framesInFlight; frame++)
				{
					writeDescriptors[frame].reserve(writeDescriptors[frame].size() + storageBufferWriteDescriptors[frame].size());
					writeDescriptors[frame].insert(writeDescriptors[frame].end(), storageBufferWriteDescriptors[frame].begin(), storageBufferWriteDescriptors[frame].end());
				}
			}
			material->UpdateForRendering(writeDescriptors);
		}
		else
		{
			material->UpdateForRendering();
		}
	}

	const std::vector<std::vector<VkWriteDescriptorSet>> &VulkanRenderingAPI::RetrieveOrCreateUniformBufferWriteDescriptors(Ref<UniformBufferSet> uniformBufferSet, Ref<VulkanMaterial> vulkanMaterial)
	{
		uint64 shaderHash = vulkanMaterial->GetShader()->GetHash();
		if (s_VulkanRendererData->UniformBufferWriteDescriptorCache.find(uniformBufferSet.Get()) != s_VulkanRendererData->UniformBufferWriteDescriptorCache.end())
		{
			const auto &shaderMap = s_VulkanRendererData->UniformBufferWriteDescriptorCache.at(uniformBufferSet.Get());
			if (shaderMap.find(shaderHash) != shaderMap.end())
			{
				const auto &writeDescriptors = shaderMap.at(shaderHash);
				return writeDescriptors;
			}
		}

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		Ref<VulkanShader> vulkanShader = vulkanMaterial->GetShader().As<VulkanShader>();
		if (vulkanShader->HasDescriptorSet(0))
		{
			const auto &shaderDescriptorSets = vulkanShader->GetShaderDescriptorSets();
			if (!shaderDescriptorSets.empty())
			{
				for (auto &&[binding, shaderUB] : shaderDescriptorSets[0].UniformBuffers)
				{
					auto &writeDescriptors = s_VulkanRendererData->UniformBufferWriteDescriptorCache[uniformBufferSet.Get()][shaderHash];
					writeDescriptors.resize(framesInFlight);
					for (uint32 frame = 0; frame < framesInFlight; frame++)
					{
						Ref<VulkanUniformBuffer> uniformBuffer = uniformBufferSet->GetUniform(binding, 0, frame); // set = 0 for now

						VkWriteDescriptorSet writeDescriptorSet = {};
						writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						writeDescriptorSet.descriptorCount = 1;
						writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
						writeDescriptorSet.pBufferInfo = &uniformBuffer->GetDescriptorBufferInfo();
						writeDescriptorSet.dstBinding = uniformBuffer->GetBinding();
						writeDescriptors[frame].push_back(writeDescriptorSet);
					}
				}

			}
		}

		return s_VulkanRendererData->UniformBufferWriteDescriptorCache[uniformBufferSet.Get()][shaderHash];
	}

	const std::vector<std::vector<VkWriteDescriptorSet>> &VulkanRenderingAPI::RetrieveOrCreateStorageBufferWriteDescriptors(Ref<StorageBufferSet> storageBufferSet, Ref<VulkanMaterial> vulkanMaterial)
	{
		uint64 shaderHash = vulkanMaterial->GetShader()->GetHash();
		if (s_VulkanRendererData->StorageBufferWriteDescriptorCache.find(storageBufferSet.Get()) != s_VulkanRendererData->StorageBufferWriteDescriptorCache.end())
		{
			const auto &shaderMap = s_VulkanRendererData->StorageBufferWriteDescriptorCache.at(storageBufferSet.Get());
			if (shaderMap.find(shaderHash) != shaderMap.end())
			{
				const auto &writeDescriptors = shaderMap.at(shaderHash);
				return writeDescriptors;
			}
		}

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		Ref<VulkanShader> vulkanShader = vulkanMaterial->GetShader().As<VulkanShader>();
		if (vulkanShader->HasDescriptorSet(0))
		{
			const auto &shaderDescriptorSets = vulkanShader->GetShaderDescriptorSets();
			if (!shaderDescriptorSets.empty())
			{
				for (auto &&[binding, shaderSB] : shaderDescriptorSets[0].StorageBuffers)
				{
					auto &writeDescriptors = s_VulkanRendererData->StorageBufferWriteDescriptorCache[storageBufferSet.Get()][shaderHash];
					writeDescriptors.resize(framesInFlight);
					for (uint32 frame = 0; frame < framesInFlight; frame++)
					{
						Ref<VulkanStorageBuffer> storageBuffer = storageBufferSet->GetStorage(binding, 0, frame); // set = 0 for now

						VkWriteDescriptorSet writeDescriptorSet = {};
						writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						writeDescriptorSet.descriptorCount = 1;
						writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
						writeDescriptorSet.pBufferInfo = &storageBuffer->GetDescriptorBufferInfo();
						writeDescriptorSet.dstBinding = storageBuffer->GetBinding();
						writeDescriptors[frame].push_back(writeDescriptorSet);
					}
				}
			}
		}

		return s_VulkanRendererData->StorageBufferWriteDescriptorCache[storageBufferSet.Get()][shaderHash];
	}

	VkDescriptorSet VulkanRenderingAPI::AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo)
	{
		VkDescriptorSet result;
		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		allocInfo.descriptorPool = s_VulkanRendererData->DescriptorPools[bufferIndex];
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &result));
		s_VulkanRendererData->DescriptorPoolAllocationCounts[bufferIndex] += allocInfo.descriptorSetCount;
		return result;
	}
}

#endif // HIGHLO_API_VULKAN

