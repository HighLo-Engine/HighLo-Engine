// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"
#include "VulkanContext.h"
#include "VulkanShaderDefs.h"
#include "VulkanShader.h"
#include "VulkanUniformBuffer.h"
#include "VulkanStorageBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"

namespace highlo
{
	struct VulkanRendererData
	{
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		VulkanShaderMaterialDescriptorSet QuadDescriptorSet;

		VkCommandBuffer ActiveCommandBuffer = nullptr;
		VkDescriptorSet ActiveRendererDescriptorSet = nullptr;
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32> DescriptorPoolAllocationCount;

		// Default samplers
		VkSampler SamplerClamp = nullptr;
		VkSampler SamplerPoint = nullptr;

		// UniformBufferSet -> Shader Hash -> Frame -> WriteDescriptor
		std::unordered_map<UniformBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> UniformBufferWriteDescriptorCache;
		std::unordered_map<StorageBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> StorageBufferWriteDescriptorCache;

		int32 SelectedDrawCall = -1;
		int32 DrawCallCount = 0;
	};

	static VulkanRendererData *s_VulkanRendererData = nullptr;

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

			return "unknown";
		}

		static const std::vector<std::vector<VkWriteDescriptorSet>> &RetrieveOrCreateUniformBufferWriteDescriptors(Ref<UniformBufferSet> &uniformBufferSet, const Ref<VulkanMaterial> &material)
		{
			uint64 shaderHash = material->GetShader()->GetHash();
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
			Ref<VulkanShader> vulkanShader = material->GetShader().As<VulkanShader>();
			if (vulkanShader->HasDescriptorSet(0))
			{
				const auto &shaderDescriptorSets = vulkanShader->GetShaderDescriptorSets();
				if (!shaderDescriptorSets.empty())
				{
					for (auto &&[binding, shaderUB] : shaderDescriptorSets[0].UniformBuffers)
					{
						auto &writeDescriptors = s_VulkanRendererData->UniformBufferWriteDescriptorCache[uniformBufferSet.Get()][shaderHash];
						writeDescriptors.resize(framesInFlight);
						for (uint32 frame = 0; frame < framesInFlight; ++frame)
						{
							Ref<VulkanUniformBuffer> uniformBuffer = uniformBufferSet->GetUniform(binding, 0, frame).As<VulkanUniformBuffer>();

							VkWriteDescriptorSet writeDescriptorSet = {};
							writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
							writeDescriptorSet.pNext = nullptr;
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

		static const std::vector<std::vector<VkWriteDescriptorSet>> &RetrieveOrCreateStorageBufferWriteDescriptors(Ref<StorageBufferSet> &storageBufferSet, const Ref<VulkanMaterial> &material)
		{
			uint64 shaderHash = material->GetShader()->GetHash();
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
			Ref<VulkanShader> vulkanShader = material->GetShader().As<VulkanShader>();
			if (vulkanShader->HasDescriptorSet(0))
			{
				const auto &shaderDescriptorSets = vulkanShader->GetShaderDescriptorSets();
				if (!shaderDescriptorSets.empty())
				{
					for (auto &&[binding, shaderSB] : shaderDescriptorSets[0].StorageBuffers)
					{
						auto &writeDescriptors = s_VulkanRendererData->StorageBufferWriteDescriptorCache[storageBufferSet.Get()][shaderHash];
						writeDescriptors.resize(framesInFlight);
						for (uint32 frame = 0; frame < framesInFlight; ++frame)
						{
							Ref<VulkanStorageBuffer> storageBuffer = storageBufferSet->GetStorage(binding, 0, frame).As<VulkanStorageBuffer>();

							VkWriteDescriptorSet writeDescriptorSet = {};
							writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
							writeDescriptorSet.pNext = nullptr;
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
	}

	void VulkanRenderingAPI::Init()
	{
		utils::InitAllocator(VulkanContext::GetCurrentDevice());
		s_VulkanRendererData = new VulkanRendererData();
		auto &config = Renderer::GetConfig();
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		s_VulkanRendererData->DescriptorPools.resize(config.FramesInFlight);
		s_VulkanRendererData->DescriptorPoolAllocationCount.resize(config.FramesInFlight);

		auto &caps = Renderer::GetCapabilities();
		auto &properties = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetProperties();
		caps.Vendor = utils::VulkanVendorIDToString(properties.vendorID);
		caps.Device = properties.deviceName;
		caps.Version = HLString::ToString(properties.driverVersion);
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
		poolInfo.pNext = nullptr;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 100000;
		poolInfo.poolSizeCount = (uint32)IM_ARRAYSIZE(poolSizes);
		poolInfo.pPoolSizes = poolSizes;
		for (uint32 i = 0; i < config.FramesInFlight; i++)
		{
			VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &s_VulkanRendererData->DescriptorPools[i]));
			s_VulkanRendererData->DescriptorPoolAllocationCount[i] = 0;
		}

		// Create fullscreen quad
		float x = -1;
		float y = -1;
		float width = 2, height = 2;
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;
		};

		QuadVertex *data = new QuadVertex[4];

		data[0].Position = glm::vec3(x, y, 0.0f);
		data[0].TexCoord = glm::vec2(0, 0);

		data[1].Position = glm::vec3(x + width, y, 0.0f);
		data[1].TexCoord = glm::vec2(1, 0);

		data[2].Position = glm::vec3(x + width, y + height, 0.0f);
		data[2].TexCoord = glm::vec2(1, 1);

		data[3].Position = glm::vec3(x, y + height, 0.0f);
		data[3].TexCoord = glm::vec2(0, 1);

		s_VulkanRendererData->QuadVertexBuffer = VertexBuffer::Create(data, 4 * sizeof(QuadVertex));
		uint32 indices[6] = { 0, 1, 2, 2, 3, 0, };
		s_VulkanRendererData->QuadIndexBuffer = IndexBuffer::Create(indices, 6 * sizeof(uint32));
	}
	
	void VulkanRenderingAPI::Shutdown()
	{
		utils::ShutdownAllocator();
		VulkanShader::ClearUniformBuffers();

		delete s_VulkanRendererData;
		s_VulkanRendererData = nullptr;
	}
	
	void VulkanRenderingAPI::BeginFrame()
	{
		Ref<VulkanSwapChain> swapchain = HLApplication::Get().GetWindow().GetSwapChain();
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		uint32 bufferIndex = swapchain->GetCurrentBufferIndex();

		vkResetDescriptorPool(device, s_VulkanRendererData->DescriptorPools[bufferIndex], 0);
		memset(s_VulkanRendererData->DescriptorPoolAllocationCount.data(), 0, s_VulkanRendererData->DescriptorPoolAllocationCount.size() * sizeof(uint32));
		
		s_VulkanRendererData->DrawCallCount = 0;
	}
	
	void VulkanRenderingAPI::EndFrame()
	{
	}
	
	void VulkanRenderingAPI::BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
	{
		HL_ASSERT(!s_VulkanRendererData->ActiveCommandBuffer);

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);
		s_VulkanRendererData->ActiveCommandBuffer = commandBuffer;

		VkDebugUtilsLabelEXT debugLabel = {};
		debugLabel.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
		debugLabel.pNext = nullptr;
		debugLabel.pLabelName = renderPass->GetSpecification().DebugName.IsEmpty() ? "" : *renderPass->GetSpecification().DebugName;
		memcpy(&debugLabel.color, glm::value_ptr(renderPass->GetSpecification().DebugMarkerColor), sizeof(float) * 4);
		fpCmdBeginDebugUtilsLabelEXT(commandBuffer, &debugLabel);

		Ref<VulkanFramebuffer> fb = renderPass->GetSpecification().Framebuffer.As<VulkanFramebuffer>();
		const auto &fbSpec = fb->GetSpecification();

		uint32 width = fb->GetWidth();
		uint32 height = fb->GetHeight();

		VkViewport viewport = {};
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = fb->GetRenderPass();
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		
		if (fb->GetSpecification().SwapChainTarget)
		{
			Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain();
			width = swapChain->GetWidth();
			height = swapChain->GetHeight();

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
			width = fb->GetWidth();
			height = fb->GetHeight();
			renderPassBeginInfo.renderArea.extent.width = width;
			renderPassBeginInfo.renderArea.extent.height = height;
			renderPassBeginInfo.framebuffer = fb->GetFramebuffer();

			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)width;
			viewport.height = (float)height;
		}

		const auto &clearValues = fb->GetClearValues();
		renderPassBeginInfo.clearValueCount = (uint32)clearValues.size();
		renderPassBeginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		if (shouldClear)
		{
			const uint32 colorAttachmentCount = (uint32)fb->GetColorAttachmentCount();
			const uint32 totalAttachmentCount = colorAttachmentCount + (fb->HasDepthAttachment() ? 1 : 0);
			HL_ASSERT(clearValues.size() == totalAttachmentCount);

			std::vector<VkClearAttachment> attachments(totalAttachmentCount);
			std::vector<VkClearRect> clearRects(totalAttachmentCount);
			for (uint32 i = 0; i < colorAttachmentCount; ++i)
			{
				attachments[i].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				attachments[i].colorAttachment = i;
				attachments[i].clearValue = clearValues[i];

				clearRects[i].rect.offset = { 0, 0 };
				clearRects[i].rect.extent = { width, height };
				clearRects[i].baseArrayLayer = 0;
				clearRects[i].layerCount = 1;
			}

			if (fb->HasDepthAttachment())
			{
				attachments[colorAttachmentCount].aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
				attachments[colorAttachmentCount].clearValue = clearValues[colorAttachmentCount];

				clearRects[colorAttachmentCount].rect.offset = { 0, 0 };
				clearRects[colorAttachmentCount].rect.extent = { width, height };
				clearRects[colorAttachmentCount].baseArrayLayer = 0;
				clearRects[colorAttachmentCount].layerCount = 1;
			}

			vkCmdClearAttachments(commandBuffer, totalAttachmentCount, attachments.data(), totalAttachmentCount, clearRects.data());
		}

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.extent.width = width;
		scissor.extent.height = height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}
	
	void VulkanRenderingAPI::EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer)
	{
		HL_ASSERT(s_VulkanRendererData->ActiveCommandBuffer);

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		vkCmdEndRenderPass(commandBuffer);
		fpCmdEndDebugUtilsLabelEXT(commandBuffer);

		s_VulkanRendererData->ActiveCommandBuffer = nullptr;
	}
	
	void VulkanRenderingAPI::ClearScreenColor(const glm::vec4 &color)
	{
	//	HL_ASSERT(s_VulkanRendererData->ActiveCommandBuffer);
	//
	//	VkClearColorValue clearColor = { color.r, color.g, color.b, color.a };
	//	VkClearValue clearValue = {};
	//	clearValue.color = clearColor;
	//
	//	VkImageSubresourceRange imageRange = {};
	//	imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//	imageRange.levelCount = 1;
	//	imageRange.layerCount = 1;
	//
	//	// TODO: replace the nullptr with the actual image
	//	vkCmdClearColorImage(s_VulkanRendererData->ActiveCommandBuffer, nullptr, VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &imageRange);
	}
	
	void VulkanRenderingAPI::ClearScreenBuffers()
	{
	}

	void VulkanRenderingAPI::DrawIndexed(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type, bool depthTest, const glm::mat4 &localTransform)
	{
	}
	
	void VulkanRenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawFullscreenQuad(Ref<CommandBuffer> &renderCommandBuffer, Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<Material> &material, const glm::mat4 &transform)
	{
	}
	
	void VulkanRenderingAPI::DrawStaticMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset)
	{
	}
	
	void VulkanRenderingAPI::DrawDynamicMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedDynamicMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMeshWithMaterial(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount, Ref<Material> overrideMaterial)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedDynamicMeshWithMaterial(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount, Ref<Material> overrideMaterial)
	{
	}
	
	void VulkanRenderingAPI::SetWireframe(bool wf)
	{
	}

	void VulkanRenderingAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
	}
	
	void VulkanRenderingAPI::SetBlendMode(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetMultiSample(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetDepthTest(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetLineThickness(float thickness)
	{
	}
	
	Ref<Environment> VulkanRenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		return nullptr;
	}
	
	Ref<Texture3D> VulkanRenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		return nullptr;
	}
	
	VkDescriptorSet VulkanRenderingAPI::AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo)
	{
		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		allocInfo.descriptorPool = s_VulkanRendererData->DescriptorPools[bufferIndex];

		VkDescriptorSet result;
		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &result));
		s_VulkanRendererData->DescriptorPoolAllocationCount[bufferIndex] += allocInfo.descriptorSetCount;
		return result;
	}

	void VulkanRenderingAPI::UpdateMaterialForRendering(Ref<VulkanMaterial> &material, Ref<UniformBufferSet> &uniformBufferSet, Ref<StorageBufferSet> &storageBufferSet)
	{
		if (uniformBufferSet)
		{
			auto writeDescriptors = utils::RetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, material);
			if (storageBufferSet)
			{
				const auto &storageBufferWriteDescriptors = utils::RetrieveOrCreateStorageBufferWriteDescriptors(storageBufferSet, material);
				const uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
				for (uint32 frame = 0; frame < framesInFlight; ++frame)
				{
					writeDescriptors[frame].reserve(writeDescriptors[frame].size() + storageBufferWriteDescriptors[frame].size());
					writeDescriptors[frame].insert(writeDescriptors[frame].end(), storageBufferWriteDescriptors[frame].begin(), storageBufferWriteDescriptors[frame].end());
				}
			}

			material->SetUniformBufferWriteDescriptors(writeDescriptors);
			material->UpdateForRendering();
		}
		else
		{
			material->UpdateForRendering();
		}
	}
	
	VkSampler VulkanRenderingAPI::GetClampSampler()
	{
		if (s_VulkanRendererData->SamplerClamp)
			return s_VulkanRendererData->SamplerClamp;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		VkSamplerCreateInfo sampler = {};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.pNext = nullptr;
		sampler.maxAnisotropy = 1.0f;
		sampler.magFilter = VK_FILTER_LINEAR;
		sampler.minFilter = VK_FILTER_LINEAR;
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.mipLodBias = 0.0f;
		sampler.minLod = 0.0f;
		sampler.maxLod = 100.0f;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		VK_CHECK_RESULT(vkCreateSampler(device, &sampler, nullptr, &s_VulkanRendererData->SamplerClamp));
		return s_VulkanRendererData->SamplerClamp;
	}
	
	VkSampler VulkanRenderingAPI::GetPointSampler()
	{
		if (s_VulkanRendererData->SamplerPoint)
			return s_VulkanRendererData->SamplerPoint;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		VkSamplerCreateInfo sampler = {};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.pNext = nullptr;
		sampler.maxAnisotropy = 1.0f;
		sampler.magFilter = VK_FILTER_NEAREST;
		sampler.minFilter = VK_FILTER_NEAREST;
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.mipLodBias = 0.0f;
		sampler.minLod = 0.0f;
		sampler.maxLod = 100.0f;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		VK_CHECK_RESULT(vkCreateSampler(device, &sampler, nullptr, &s_VulkanRendererData->SamplerPoint));
		return s_VulkanRendererData->SamplerPoint;
	}
	
	uint32 VulkanRenderingAPI::GetDescriptorAllocationCount(uint32 frameIndex)
	{
		HL_ASSERT(frameIndex < s_VulkanRendererData->DescriptorPoolAllocationCount.size());
		return s_VulkanRendererData->DescriptorPoolAllocationCount[frameIndex];
	}
	
	int32 &VulkanRenderingAPI::GetSelectedDrawCall()
	{
		return s_VulkanRendererData->SelectedDrawCall;
	}
}

#endif // HIGHLO_API_VULKAN

