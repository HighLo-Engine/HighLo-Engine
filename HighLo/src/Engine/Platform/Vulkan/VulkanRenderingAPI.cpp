// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Renderer/Renderer.h"
#include "Engine/Utils/ImageUtils.h"

#include "VulkanContext.h"
#include "VulkanAllocator.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"
#include "VulkanComputePipeline.h"
#include "VulkanShader.h"
#include "VulkanTexture2D.h"
#include "VulkanTexture3D.h"
#include "VulkanMaterial.h"
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
		Ref<VertexBuffer> FullscreenQuadVertexBuffer = nullptr;
		Ref<IndexBuffer> FullscreenQuadIndexBuffer = nullptr;
		VkShaderMaterialDescriptorSet FullscreenQuadDescriptorSet;
		
		std::unordered_map<const SceneRenderer*, std::vector<VkShaderMaterialDescriptorSet>> RendererDescriptorSet;
		
		VkDescriptorSet ActiveRendererDescriptorSet = nullptr;
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32> DescriptorPoolAllocationCount;

		// UniformBufferSet -> Shader Hash -> Frame -> WriteDescriptor
		std::unordered_map<const UniformBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> UniformBufferWriteDescriptorCache;
		std::unordered_map<const StorageBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> StorageBufferWriteDescriptorCache;

		// Default samplers
		VkSampler SamplerClamp = nullptr;
		VkSampler SamplerPoint = nullptr;

		int32 SelectedDrawCall = -1;
		int32 DrawCallCount = 0;
	};

	static VulkanRendererData *s_VKRendererData = nullptr;

	// TODO: Move these to a general location, because it is used in every rendering api implementation
	struct VKQuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	struct VKAnimatedBoneTransformUniformBuffer
	{
		glm::mat4 BoneTransform[100];
	};

	static const std::vector<std::vector<VkWriteDescriptorSet>> &RT_RetrieveOrCreateUniformBufferWriteDescriptors(const Ref<UniformBufferSet> &uniformBufferSet, const Ref<VulkanMaterial> &material)
	{
		uint64 shaderHash = material->GetShader()->GetHash();

		if (s_VKRendererData->UniformBufferWriteDescriptorCache.find(uniformBufferSet.Get()) != s_VKRendererData->UniformBufferWriteDescriptorCache.end())
		{
			const auto &shaderMap = s_VKRendererData->UniformBufferWriteDescriptorCache.at(uniformBufferSet.Get());
			if (shaderMap.find(shaderHash) != shaderMap.end())
			{
				const auto &writeDescriptors = shaderMap.at(shaderHash);
				return writeDescriptors;
			}
		}

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		Ref<VulkanShader> vkShader = material->GetShader().As<VulkanShader>();

		// TODO: Might need to iterate over all sets
		if (vkShader->HasDescriptorSet(0))
		{
			const auto &shaderDescriptorSets = vkShader->GetShaderDescriptorSets();
			if (!shaderDescriptorSets.empty())
			{
				for (auto &&[binding, shaderUB] : shaderDescriptorSets[0].UniformBuffers)
				{
					auto &writeDescriptors = s_VKRendererData->UniformBufferWriteDescriptorCache[uniformBufferSet.Get()][shaderHash];
					writeDescriptors.resize(framesInFlight);

					for (uint32 frame = 0; frame < framesInFlight; ++frame)
					{
						Ref<VulkanUniformBuffer> uniformBuffer = uniformBufferSet->GetUniform(binding, 0, frame);

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

		return s_VKRendererData->UniformBufferWriteDescriptorCache[uniformBufferSet.Get()][shaderHash];
	}

	static const std::vector<std::vector<VkWriteDescriptorSet>> &RT_RetrieveOrCreateStorageBufferWriteDescriptors(const Ref<StorageBufferSet> &storageBufferSet, const Ref<VulkanMaterial> &material)
	{
		uint64 shaderHash = material->GetShader()->GetHash();

		if (s_VKRendererData->StorageBufferWriteDescriptorCache.find(storageBufferSet.Get()) != s_VKRendererData->StorageBufferWriteDescriptorCache.end())
		{
			const auto &shaderMap = s_VKRendererData->StorageBufferWriteDescriptorCache.at(storageBufferSet.Get());
			if (shaderMap.find(shaderHash) != shaderMap.end())
			{
				const auto &writeDescriptors = shaderMap.at(shaderHash);
				return writeDescriptors;
			}
		}

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		Ref<VulkanShader> vkShader = material->GetShader().As<VulkanShader>();

		// TODO: Might need to iterate over all sets
		if (vkShader->HasDescriptorSet(0))
		{
			const auto &shaderDescriptorSets = vkShader->GetShaderDescriptorSets();
			if (!shaderDescriptorSets.empty())
			{
				for (auto &&[binding, shaderSB] : shaderDescriptorSets[0].StorageBuffers)
				{
					auto &writeDescriptors = s_VKRendererData->StorageBufferWriteDescriptorCache[storageBufferSet.Get()][shaderHash];
					writeDescriptors.resize(framesInFlight);
					for (uint32 frame = 0; frame < framesInFlight; ++frame)
					{
						Ref<VulkanStorageBuffer> storageBuffer = storageBufferSet->GetStorage(binding, 0, frame);

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

		return s_VKRendererData->StorageBufferWriteDescriptorCache[storageBufferSet.Get()][shaderHash];
	}

	static VkBuffer TransformBufferToVulkanBuffer(const TransformVertexData *vertex_data, uint32 size)
	{
		VulkanAllocator allocator("TransformBuffer");
		Ref<VulkanDevice> device = VulkanContext::GetCurrentDevice().As<VulkanDevice>();
		VkBuffer result = {0};

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		// Copy data to staging buffer
		uint8 *destData = allocator.MapMemory<uint8>(stagingBufferAllocation);
		memcpy(destData, vertex_data, size);
		allocator.UnmapMemory(stagingBufferAllocation);

		VkBufferCreateInfo vertexBufferCreateInfo = {};
		vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferCreateInfo.size = size;
		vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		VmaAllocation allocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, result);

		VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(copyCmd, stagingBuffer, result, 1, &copyRegion);

		device->FlushCommandBuffer(copyCmd);

		allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);

		return result;
	}

	void VulkanRenderingAPI::Init()
	{
		s_VKRendererData = new VulkanRendererData();

		const auto &config = Renderer::GetConfig();
		s_VKRendererData->DescriptorPools.resize(config.FramesInFlight);
		s_VKRendererData->DescriptorPoolAllocationCount.resize(config.FramesInFlight);

		auto &caps = Renderer::GetCapabilities();
		auto &properties = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetProperties();
		caps.Vendor = utils::VulkanVendorIDToString(properties.vendorID);
		caps.Device = properties.deviceName;
		caps.Version = std::to_string(properties.driverVersion);
		caps.MaxTextures = properties.limits.maxDescriptorSetSampledImages;
		caps.MaxTextureUnits = properties.limits.maxDescriptorSetSampledImages;
		caps.MaxAnisotropy = properties.limits.maxSamplerAnisotropy;
		caps.MaxSamples = properties.limits.maxDescriptorSetSamplers;

		utils::DumpGPUInfos();

		// Create Descriptor Pool
		Renderer::Submit([]() mutable
		{
			VkDescriptorPoolSize pool_sizes[] =
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

			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 100000;
			pool_info.poolSizeCount = (uint32)IM_ARRAYSIZE(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;

			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
			for (uint32 i = 0; i < framesInFlight; i++)
			{
				VK_CHECK_RESULT(vkCreateDescriptorPool(device, &pool_info, nullptr, &s_VKRendererData->DescriptorPools[i]));
				s_VKRendererData->DescriptorPoolAllocationCount[i] = 0;
			}
		});

		float x = -1;
		float y = -1;
		float width = 2, height = 2;

		VKQuadVertex *data = new VKQuadVertex[4];

		data[0].Position = glm::vec3(x, y, 0.1f);
		data[0].TexCoord = glm::vec2(0, 0);

		data[1].Position = glm::vec3(x + width, y, 0.1f);
		data[1].TexCoord = glm::vec2(1, 0);

		data[2].Position = glm::vec3(x + width, y + height, 0.1f);
		data[2].TexCoord = glm::vec2(1, 1);

		data[3].Position = glm::vec3(x, y + height, 0.1f);
		data[3].TexCoord = glm::vec2(0, 1);

		std::vector<int32> indices = { 0, 1, 2, 2, 3, 0 };

		s_VKRendererData->FullscreenQuadVertexBuffer = VertexBuffer::Create(data, 4 * sizeof(VKQuadVertex));
		s_VKRendererData->FullscreenQuadIndexBuffer = IndexBuffer::Create(indices);

		delete[] data;
		indices.clear();
		indices.shrink_to_fit();
	}

	void VulkanRenderingAPI::Shutdown()
	{
		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		vkDeviceWaitIdle(device);

		delete s_VKRendererData;
		s_VKRendererData = nullptr;
	}
	
	void VulkanRenderingAPI::BeginFrame()
	{
		Renderer::Submit([]()
		{
			Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain();

			// Reset descriptor pools here
			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			uint32 bufferIndex = swapChain->GetCurrentBufferIndex();
			vkResetDescriptorPool(device, s_VKRendererData->DescriptorPools[bufferIndex], 0);
			memset(s_VKRendererData->DescriptorPoolAllocationCount.data(), 0, s_VKRendererData->DescriptorPoolAllocationCount.size() * sizeof(uint32));

			s_VKRendererData->DrawCallCount = 0;
		});
	}
	
	void VulkanRenderingAPI::EndFrame()
	{
	}
	
	void VulkanRenderingAPI::BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
	{
		Renderer::Submit([renderCommandBuffer, renderPass, shouldClear]()
		{
			uint32 frameIndex = Renderer::RT_GetCurrentFrameIndex();
			VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetActiveCommandBuffer();

			VkDebugUtilsLabelEXT debugLabel{};
			debugLabel.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
			memcpy(&debugLabel.color, glm::value_ptr(renderPass->GetSpecification().DebugMarkerColor), sizeof(float) * 4);
			debugLabel.pLabelName = *renderPass->GetSpecification().DebugName;
			fpCmdBeginDebugUtilsLabelEXT(commandBuffer, &debugLabel);

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

					clearRects[i].rect.offset = { (int32)0, (int32)0 };
					clearRects[i].rect.extent = { width, height };
					clearRects[i].baseArrayLayer = 0;
					clearRects[i].layerCount = 1;
				}

				if (framebuffer->HasDepthAttachment())
				{
					attachments[colorAttachmentCount].aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
					attachments[colorAttachmentCount].clearValue = clearValues[colorAttachmentCount];
					clearRects[colorAttachmentCount].rect.offset = { (int32)0, (int32)0 };
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
		});
	}
	
	void VulkanRenderingAPI::EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer)
	{
		Renderer::Submit([renderCommandBuffer]()
		{
			uint32 frameIndex = Renderer::RT_GetCurrentFrameIndex();
			VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetActiveCommandBuffer();

			vkCmdEndRenderPass(commandBuffer);
			fpCmdEndDebugUtilsLabelEXT(commandBuffer);
		});
	}
	
	void VulkanRenderingAPI::ClearScreenColor(const glm::vec4 &color)
	{
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
	
	void VulkanRenderingAPI::DrawFullscreenQuad(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<Material> &material, const glm::mat4 &transform)
	{
		Ref<VulkanMaterial> vkMaterial = material.As<VulkanMaterial>();
		Renderer::Submit([renderCommandBuffer, va, uniformBufferSet, storageBufferSet, material, vkMaterial]() mutable
		{
			uint32 frameIndex = Renderer::RT_GetCurrentFrameIndex();
			VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetActiveCommandBuffer();

			Ref<VulkanVertexArray> vkVertexArray = va.As<VulkanVertexArray>();
			VkPipelineLayout layout = vkVertexArray->GetVulkanPipelineLayout();

			Ref<VulkanVertexBuffer> vkMeshVB = s_VKRendererData->FullscreenQuadVertexBuffer.As<VulkanVertexBuffer>();
			VkBuffer vbMeshBuffer = vkMeshVB->GetVulkanBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

			Ref<VulkanIndexBuffer> vkMeshIB = s_VKRendererData->FullscreenQuadIndexBuffer.As<VulkanIndexBuffer>();
			VkBuffer ibMeshBuffer = vkMeshIB->GetVulkanBuffer();
			vkCmdBindIndexBuffer(commandBuffer, ibMeshBuffer, 0, VK_INDEX_TYPE_UINT32);

			VkPipeline pipeline = vkVertexArray->GetVulkanPipeline();
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

			// Update materials
			if (uniformBufferSet)
			{
				const auto &ubWriteDescriptors = RT_RetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, vkMaterial);
				if (storageBufferSet)
				{
					const auto &sbWriteDescriptors = RT_RetrieveOrCreateStorageBufferWriteDescriptors(storageBufferSet, vkMaterial);
					vkMaterial->RT_UpdateForRendering(ubWriteDescriptors, sbWriteDescriptors);
				}
				else
				{
					vkMaterial->RT_UpdateForRendering(ubWriteDescriptors, std::vector<std::vector<VkWriteDescriptorSet>>());
				}
			}
			else
			{
				vkMaterial->RT_UpdateForRendering();
			}

			VkDescriptorSet descriptorSet = vkMaterial->GetDescriptorSet(frameIndex);
			if (descriptorSet)
			{
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);
			}

			Allocator uniformStorageBuffer = vkMaterial->GetUniformStorageBuffer();
			if (uniformStorageBuffer.Size)
			{
				vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, uniformStorageBuffer.Size, uniformStorageBuffer.Data);
			}

			vkCmdDrawIndexed(commandBuffer, s_VKRendererData->FullscreenQuadIndexBuffer->GetCount(), 1, 0, 0, 0);
		});
	}
	
	void VulkanRenderingAPI::DrawStaticMesh(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<StaticModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials, const TransformVertexData *transformBuffer, uint32 transformBufferOffset)
	{
		Renderer::Submit([renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materials, transformBuffer, transformBufferOffset]() mutable
		{
			if (s_VKRendererData->SelectedDrawCall != -1 && s_VKRendererData->DrawCallCount > s_VKRendererData->SelectedDrawCall)
				return;

			uint32 frameIndex = Renderer::RT_GetCurrentFrameIndex();
			VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetActiveCommandBuffer();
			Ref<MeshFile> &meshFile = model->Get();
			
			Ref<VulkanVertexBuffer> vkMeshVB = meshFile->GetVertexBuffer().As<VulkanVertexBuffer>();
			VkBuffer vbMeshBuffer = vkMeshVB->GetVulkanBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

			VkDeviceSize instanceOffsets[1] = { transformBufferOffset };
			VkBuffer vbTransformBuffer = TransformBufferToVulkanBuffer(transformBuffer, sizeof(transformBuffer));
			HL_ASSERT(vbTransformBuffer);
			vkCmdBindVertexBuffers(commandBuffer, 1, 1, &vbTransformBuffer, instanceOffsets);

			Ref<VulkanIndexBuffer> vkMeshIB = meshFile->GetIndexBuffer().As<VulkanIndexBuffer>();
			VkBuffer ibMeshBuffer = vkMeshIB->GetVulkanBuffer();
			vkCmdBindIndexBuffer(commandBuffer, ibMeshBuffer, 0, VK_INDEX_TYPE_UINT32);

			Ref<VulkanVertexArray> vkVertexArray = va.As<VulkanVertexArray>();
			VkPipeline pipeline = vkVertexArray->GetVulkanPipeline();
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

			const auto &submeshes = meshFile->GetSubmeshes();
			const Mesh &submesh = submeshes[submeshIndex];
			auto &meshMaterialTable = model->GetMaterials();
			uint32 materialCount = meshMaterialTable->GetMaterialCount();

			Ref<MaterialAsset> materialAsset = 
				materials->HasMaterial(submesh.MaterialIndex) ? 
				materials->GetMaterial(submesh.MaterialIndex) : 
				meshMaterialTable->GetMaterial(submesh.MaterialIndex);

			Ref<VulkanMaterial> vkMaterial = materialAsset->GetMaterial().As<VulkanMaterial>();

			// Update material
			if (uniformBufferSet)
			{
				const auto &ubWriteDescriptors = RT_RetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, vkMaterial);
				if (storageBufferSet)
				{
					const auto &sbWriteDescriptors = RT_RetrieveOrCreateStorageBufferWriteDescriptors(storageBufferSet, vkMaterial);
					vkMaterial->RT_UpdateForRendering(ubWriteDescriptors, sbWriteDescriptors);
				}
				else
				{
					vkMaterial->RT_UpdateForRendering(ubWriteDescriptors, std::vector<std::vector<VkWriteDescriptorSet>>());
				}
			}
			else
			{
				vkMaterial->RT_UpdateForRendering();
			}

			std::array<VkDescriptorSet, 2> descriptorSets = { 
				vkMaterial->GetDescriptorSet(frameIndex),
				s_VKRendererData->ActiveRendererDescriptorSet
			};

			VkPipelineLayout layout = vkVertexArray->GetVulkanPipelineLayout();
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, (uint32)descriptorSets.size(), descriptorSets.data(), 0, nullptr);

			Allocator uniformStorageBuffer = vkMaterial->GetUniformStorageBuffer();
			vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, uniformStorageBuffer.Size, uniformStorageBuffer.Data);

			vkCmdDrawIndexed(commandBuffer, submesh.IndexCount, 1, submesh.BaseIndex, submesh.BaseVertex, 0);
			s_VKRendererData->DrawCallCount++;
		});
	}
	
	void VulkanRenderingAPI::DrawDynamicMesh(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<DynamicModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials, const TransformVertexData *transformBuffer, uint32 transformBufferOffset)
	{
		// TODO
		Renderer::Submit([]() mutable
		{
			if (s_VKRendererData->SelectedDrawCall != -1 && s_VKRendererData->DrawCallCount > s_VKRendererData->SelectedDrawCall)
				return;


		});
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMesh(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<StaticModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials, const TransformVertexData *transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
		Renderer::Submit([renderCommandBuffer, va, model, uniformBufferSet, storageBufferSet, submeshIndex, materials, transformBuffer, transformBufferOffset, instanceCount]() mutable
		{
			if (s_VKRendererData->SelectedDrawCall != -1 && s_VKRendererData->DrawCallCount > s_VKRendererData->SelectedDrawCall)
				return;

			uint32 frameIndex = Renderer::RT_GetCurrentFrameIndex();
			VkCommandBuffer commandBuffer = renderCommandBuffer.As<VulkanCommandBuffer>()->GetActiveCommandBuffer();
			Ref<MeshFile> &meshFile = model->Get();

			Ref<VulkanVertexBuffer> vkMeshVB = meshFile->GetVertexBuffer().As<VulkanVertexBuffer>();
			VkBuffer vbMeshBuffer = vkMeshVB->GetVulkanBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vbMeshBuffer, offsets);

			VkDeviceSize instanceOffsets[1] = { transformBufferOffset };
			VkBuffer vbTransformBuffer = TransformBufferToVulkanBuffer(transformBuffer, sizeof(transformBuffer));
			HL_ASSERT(vbTransformBuffer);
			vkCmdBindVertexBuffers(commandBuffer, 1, 1, &vbTransformBuffer, instanceOffsets);

			Ref<VulkanIndexBuffer> vkMeshIB = meshFile->GetIndexBuffer().As<VulkanIndexBuffer>();
			VkBuffer ibMeshBuffer = vkMeshIB->GetVulkanBuffer();
			vkCmdBindIndexBuffer(commandBuffer, ibMeshBuffer, 0, VK_INDEX_TYPE_UINT32);

			Ref<VulkanVertexArray> vkVertexArray = va.As<VulkanVertexArray>();
			VkPipeline pipeline = vkVertexArray->GetVulkanPipeline();
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

			const auto &submeshes = meshFile->GetSubmeshes();
			const Mesh &submesh = submeshes[submeshIndex];
			auto &meshMaterialTable = model->GetMaterials();
			uint32 materialCount = meshMaterialTable->GetMaterialCount();

			Ref<MaterialAsset> &assetMaterial = 
				materials->HasMaterial(submesh.MaterialIndex) ? 
				materials->GetMaterial(submesh.MaterialIndex) : 
				meshMaterialTable->GetMaterial(submesh.MaterialIndex);
			Ref<VulkanMaterial> vkMaterial = assetMaterial->GetMaterial().As<VulkanMaterial>();

			// Update Material
			if (uniformBufferSet)
			{
				const auto &ubWriteDescriptors = RT_RetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, vkMaterial);
				if (storageBufferSet)
				{
					const auto &sbWriteDescriptors = RT_RetrieveOrCreateStorageBufferWriteDescriptors(storageBufferSet, vkMaterial);
					vkMaterial->RT_UpdateForRendering(ubWriteDescriptors, sbWriteDescriptors);
				}
				else
				{
					vkMaterial->RT_UpdateForRendering(ubWriteDescriptors, std::vector<std::vector<VkWriteDescriptorSet>>());
				}
			}
			else
			{
				vkMaterial->RT_UpdateForRendering();
			}

			if (s_VKRendererData->SelectedDrawCall != -1 && s_VKRendererData->DrawCallCount > s_VKRendererData->SelectedDrawCall)
				return;

			VkDescriptorSet descriptorSet = vkMaterial->GetDescriptorSet(frameIndex);
			std::vector<VkDescriptorSet> descriptorSets = {
				descriptorSet,
				s_VKRendererData->ActiveRendererDescriptorSet,
			};

			Allocator uniformStorageBuffer = vkMaterial->GetUniformStorageBuffer();
			uint32 pushConstantOffset = 0;

			// TODO: If we support animations again, we will need another descriptor set with the bone data...

			VkPipelineLayout layout = vkVertexArray->GetVulkanPipelineLayout();
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, (uint32)descriptorSets.size(), descriptorSets.data(), 0, nullptr);

			

			if (uniformStorageBuffer)
			{
				vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, pushConstantOffset, uniformStorageBuffer.Size, uniformStorageBuffer.Data);
			}

			vkCmdDrawIndexed(commandBuffer, submesh.IndexCount, instanceCount, submesh.BaseIndex, submesh.BaseVertex, 0);
			s_VKRendererData->DrawCallCount++;
		});
	}
	
	void VulkanRenderingAPI::DrawInstancedDynamicMesh(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<DynamicModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials, const TransformVertexData *transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMeshWithMaterial(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<StaticModel> &model, uint32 submeshIndex, const TransformVertexData *transformBuffer, uint32 transformBufferOffset, uint32 instanceCount, Ref<Material> &overrideMaterial)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedDynamicMeshWithMaterial(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<DynamicModel> &model, uint32 submeshIndex, const TransformVertexData *transformBuffer, uint32 transformBufferOffset, uint32 instanceCount, Ref<Material> &overrideMaterial)
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
	
	void VulkanRenderingAPI::SetSceneEnvironment(const Ref<SceneRenderer> &sceneRenderer, Ref<Environment> &environment, const Ref<Texture2D> &shadow)
	{
		if (!environment)
			environment = Renderer::GetEmptyEnvironment();

		Renderer::Submit([sceneRenderer, environment, shadow]() mutable
		{
			const auto shader = Renderer::GetShaderLibrary()->Get("HighLoPBR");
			Ref<VulkanShader> pbrShader = shader.As<VulkanShader>();
			const uint32 bufferIndex = Renderer::RT_GetCurrentFrameIndex();

			if (s_VKRendererData->RendererDescriptorSet.find(sceneRenderer.Get()) == s_VKRendererData->RendererDescriptorSet.end())
			{
				const uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
				s_VKRendererData->RendererDescriptorSet[sceneRenderer.Get()].resize(framesInFlight);
				for (uint32 i = 0; i < framesInFlight; i++)
					s_VKRendererData->RendererDescriptorSet.at(sceneRenderer.Get())[i] = pbrShader->CreateDescriptorSets(1);

			}

			VkDescriptorSet descriptorSet = s_VKRendererData->RendererDescriptorSet.at(sceneRenderer.Get())[bufferIndex].DescriptorSets[0];
			s_VKRendererData->ActiveRendererDescriptorSet = descriptorSet;

			std::array<VkWriteDescriptorSet, 4> writeDescriptors;

			Ref<VulkanTexture3D> radianceMap = environment->GetRadianceMap().As<VulkanTexture3D>();
			Ref<VulkanTexture3D> irradianceMap = environment->GetIrradianceMap().As<VulkanTexture3D>();

		//	const auto &radianceMapImageInfo = radianceMap->GetDescriptorInfo();
		//	writeDescriptors[0] = *pbrShader->GetDescriptorSet("u_EnvRadianceTex", 1);
		//	writeDescriptors[0].dstSet = descriptorSet;
		//	writeDescriptors[0].pImageInfo = &radianceMapImageInfo;

		//	const auto &irradianceMapImageInfo = irradianceMap->GetDescriptorInfo();
		//	writeDescriptors[1] = *pbrShader->GetDescriptorSet("u_EnvIrradianceTex", 1);
		//	writeDescriptors[1].dstSet = descriptorSet;
		//	writeDescriptors[1].pImageInfo = &irradianceMapImageInfo;

		//	const auto &brdfLutImageInfo = Renderer::GetBRDFLutTexture().As<VulkanTexture2D>()->GetDescriptorInfo();
		//	writeDescriptors[2] = *pbrShader->GetDescriptorSet("u_BRDFLUTTexture", 1);
		//	writeDescriptors[2].dstSet = descriptorSet;
		//	writeDescriptors[2].pImageInfo = &brdfLutImageInfo;

		//	const auto &shadowImageInfo = shadow.As<VulkanTexture2D>()->GetDescriptorInfo();
		//	writeDescriptors[3] = *pbrShader->GetDescriptorSet("u_ShadowMapTexture", 1);
		//	writeDescriptors[3].dstSet = descriptorSet;
		//	writeDescriptors[3].pImageInfo = &shadowImageInfo;

		//	const auto vulkanDevice = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		//	vkUpdateDescriptorSets(vulkanDevice, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);
		});
	}
	
	Ref<Environment> VulkanRenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		if (!Renderer::GetConfig().ComputeEnvironmentMaps)
			return Renderer::GetEmptyEnvironment();

		Ref<Texture2D> envEquirect = Texture2D::LoadFromFile(filePath);
		HL_ASSERT(envEquirect->GetFormat() == TextureFormat::RGBA32F, "Texture is not HDR!");

		TextureSpecification cubemapSpec;
		cubemapSpec.Format = TextureFormat::RGBA32F;
		cubemapSpec.Width = cubemapSize;
		cubemapSpec.Height = cubemapSize;

		Ref<Texture3D> envUnfiltered = Texture3D::CreateFromSpecification(cubemapSpec);
		Ref<Texture3D> envFiltered = Texture3D::CreateFromSpecification(cubemapSpec);

		// Convert equirectangular to cubemap
		Ref<Shader> equirectangularConversionShader = Renderer::GetShaderLibrary()->Get("EquirectangularToCubeMap");
		Ref<VulkanComputePipeline> equirectangularConversionPipeline = Ref<VulkanComputePipeline>::Create(equirectangularConversionShader);

		Renderer::Submit([equirectangularConversionPipeline, envEquirect, envUnfiltered, cubemapSize]() mutable
		{
			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			Ref<VulkanShader> shader = equirectangularConversionPipeline->GetShader();

			std::array<VkWriteDescriptorSet, 2> writeDescriptors;
			auto descriptorSet = shader->CreateDescriptorSets();
			Ref<VulkanTexture3D> envUnfilteredCubemap = envUnfiltered.As<VulkanTexture3D>();
			writeDescriptors[0] = *shader->GetDescriptorSet("o_CubeMap");
			writeDescriptors[0].dstSet = descriptorSet.DescriptorSets[0]; // Should this be set inside the shader?
			writeDescriptors[0].pImageInfo = &envUnfilteredCubemap->GetDescriptorInfo();

			Ref<VulkanTexture2D> envEquirectVK = envEquirect.As<VulkanTexture2D>();
			writeDescriptors[1] = *shader->GetDescriptorSet("u_EquirectangularTex");
			writeDescriptors[1].dstSet = descriptorSet.DescriptorSets[0]; // Should this be set inside the shader?
			writeDescriptors[1].pImageInfo = &envEquirectVK->GetDescriptorInfo();

			vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, NULL);
			equirectangularConversionPipeline->Execute(descriptorSet.DescriptorSets.data(), (uint32)descriptorSet.DescriptorSets.size(), cubemapSize / 32, cubemapSize / 32, 6);

			envUnfilteredCubemap->GenerateMips(true);
		});

		Ref<Shader> environmentMipFilterShader = Renderer::GetShaderLibrary()->Get("EnvironmentMipFilter");
		Ref<VulkanComputePipeline> environmentMipFilterPipeline = Ref<VulkanComputePipeline>::Create(environmentMipFilterShader);

		Renderer::Submit([environmentMipFilterPipeline, envUnfiltered, envFiltered, cubemapSize]() mutable
		{
			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			Ref<VulkanShader> shader = environmentMipFilterPipeline->GetShader();

			Ref<VulkanTexture3D> envFilteredCubemap = envFiltered.As<VulkanTexture3D>();
			VkDescriptorImageInfo imageInfo = envFilteredCubemap->GetDescriptorInfo();

			uint32 mipCount = utils::CalculateMipCount(cubemapSize, cubemapSize);

			std::vector<VkWriteDescriptorSet> writeDescriptors(mipCount * 2);
			std::vector<VkDescriptorImageInfo> mipImageInfos(mipCount);
			auto descriptorSet = shader->CreateDescriptorSets(0, 12);
			for (uint32 i = 0; i < mipCount; i++)
			{
				VkDescriptorImageInfo &mipImageInfo = mipImageInfos[i];
				mipImageInfo = imageInfo;
				mipImageInfo.imageView = envFilteredCubemap->CreateImageViewSingleMip(i);

				writeDescriptors[i * 2 + 0] = *shader->GetDescriptorSet("outputTexture");
				writeDescriptors[i * 2 + 0].dstSet = descriptorSet.DescriptorSets[i]; // Should this be set inside the shader?
				writeDescriptors[i * 2 + 0].pImageInfo = &mipImageInfo;

				Ref<VulkanTexture3D> envUnfilteredCubemap = envUnfiltered.As<VulkanTexture3D>();
				writeDescriptors[i * 2 + 1] = *shader->GetDescriptorSet("inputTexture");
				writeDescriptors[i * 2 + 1].dstSet = descriptorSet.DescriptorSets[i]; // Should this be set inside the shader?
				writeDescriptors[i * 2 + 1].pImageInfo = &envUnfilteredCubemap->GetDescriptorInfo();
			}

			vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, NULL);

			environmentMipFilterPipeline->Begin(); // begin compute pass
			const float deltaRoughness = 1.0f / glm::max((float)envFiltered->GetMipLevelCount() - 1.0f, 1.0f);
			for (uint32 i = 0, size = cubemapSize; i < mipCount; i++, size /= 2)
			{
				uint32 numGroups = glm::max(1u, size / 32);
				float roughness = i * deltaRoughness;
				roughness = glm::max(roughness, 0.05f);
				environmentMipFilterPipeline->SetPushConstants(&roughness, sizeof(float));
				environmentMipFilterPipeline->Dispatch(descriptorSet.DescriptorSets[i], numGroups, numGroups, 6);
			}
			environmentMipFilterPipeline->End();
		});

		Ref<Shader> environmentIrradianceShader = Renderer::GetShaderLibrary()->Get("EnvironmentIrradiance");
		Ref<VulkanComputePipeline> environmentIrradiancePipeline = Ref<VulkanComputePipeline>::Create(environmentIrradianceShader);

		cubemapSpec.Width = irradianceMapSize;
		cubemapSpec.Height = irradianceMapSize;
		Ref<Texture3D> irradianceMap = Texture3D::CreateFromSpecification(cubemapSpec);

		Renderer::Submit([environmentIrradiancePipeline, irradianceMap, envFiltered]() mutable
		{
			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			Ref<VulkanShader> shader = environmentIrradiancePipeline->GetShader();

			Ref<VulkanTexture3D> envFilteredCubemap = envFiltered.As<VulkanTexture3D>();
			Ref<VulkanTexture3D> irradianceCubemap = irradianceMap.As<VulkanTexture3D>();
			auto descriptorSet = shader->CreateDescriptorSets();

			std::array<VkWriteDescriptorSet, 2> writeDescriptors;
			writeDescriptors[0] = *shader->GetDescriptorSet("o_IrradianceMap");
			writeDescriptors[0].dstSet = descriptorSet.DescriptorSets[0];
			writeDescriptors[0].pImageInfo = &irradianceCubemap->GetDescriptorInfo();

			writeDescriptors[1] = *shader->GetDescriptorSet("u_RadianceMap");
			writeDescriptors[1].dstSet = descriptorSet.DescriptorSets[0];
			writeDescriptors[1].pImageInfo = &envFilteredCubemap->GetDescriptorInfo();

			vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, NULL);
			environmentIrradiancePipeline->Begin();
			environmentIrradiancePipeline->SetPushConstants(&Renderer::GetConfig().IrradianceMapComputeSamples, sizeof(uint32));
			environmentIrradiancePipeline->Dispatch(descriptorSet.DescriptorSets[0], irradianceMap->GetWidth() / 32, irradianceMap->GetHeight() / 32, 6);
			environmentIrradiancePipeline->End();

			irradianceCubemap->GenerateMips();
		});

		return Ref<Environment>::Create(filePath, envUnfiltered, envFiltered, irradianceMap);
	}
	
	Ref<Texture3D> VulkanRenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		const uint32 cubemapSize = Renderer::GetConfig().EnvironmentMapResolution;
		const uint32 irradianceMapSize = 32;

		TextureSpecification cubemapSpec;
		cubemapSpec.Format = TextureFormat::RGBA32F;
		cubemapSpec.Width = cubemapSize;
		cubemapSpec.Height = cubemapSize;

		Ref<Texture3D> environmentMap = Texture3D::CreateFromSpecification(cubemapSpec);

		Ref<Shader> preethamSkyShader = Renderer::GetShaderLibrary()->Get("PreethamSky");
		Ref<VulkanComputePipeline> preethamSkyComputePipeline = Ref<VulkanComputePipeline>::Create(preethamSkyShader);

		glm::vec3 params = { turbidity, azimuth, inclination };
		Renderer::Submit([preethamSkyComputePipeline, environmentMap, cubemapSize, params]() mutable
		{
			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			Ref<VulkanShader> shader = preethamSkyComputePipeline->GetShader();

			std::array<VkWriteDescriptorSet, 1> writeDescriptors;
			auto descriptorSet = shader->CreateDescriptorSets();
			Ref<VulkanTexture3D> envUnfilteredCubemap = environmentMap.As<VulkanTexture3D>();
			writeDescriptors[0] = *shader->GetDescriptorSet("o_CubeMap");
			writeDescriptors[0].dstSet = descriptorSet.DescriptorSets[0]; // Should this be set inside the shader?
			writeDescriptors[0].pImageInfo = &envUnfilteredCubemap->GetDescriptorInfo();

			vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, NULL);

			preethamSkyComputePipeline->Begin();
			preethamSkyComputePipeline->SetPushConstants(&params, sizeof(glm::vec3));
			preethamSkyComputePipeline->Dispatch(descriptorSet.DescriptorSets[0], cubemapSize / 32, cubemapSize / 32, 6);
			preethamSkyComputePipeline->End();

			envUnfilteredCubemap->GenerateMips(true);
		});

		return environmentMap;
	}

	VkDescriptorSet VulkanRenderingAPI::AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo)
	{
		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

		allocInfo.descriptorPool = s_VKRendererData->DescriptorPools[bufferIndex];

		VkDescriptorSet result = { 0 };
		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &result));
		s_VKRendererData->DescriptorPoolAllocationCount[bufferIndex] += allocInfo.descriptorSetCount;
		return result;
	}
	
	VkSampler VulkanRenderingAPI::GetClampSampler()
	{
		if (s_VKRendererData->SamplerClamp)
			return s_VKRendererData->SamplerClamp;

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
		samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 100.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &s_VKRendererData->SamplerClamp));
		return s_VKRendererData->SamplerClamp;
	}
	
	VkSampler VulkanRenderingAPI::GetPointSampler()
	{
		if (s_VKRendererData->SamplerPoint)
			return s_VKRendererData->SamplerPoint;

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
		samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
		samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 100.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

		VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
		VK_CHECK_RESULT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &s_VKRendererData->SamplerPoint));
		return s_VKRendererData->SamplerPoint;
	}
	
	uint32 VulkanRenderingAPI::GetDescriptorAllocationCount(uint32 frameIndex)
	{
		return s_VKRendererData->DescriptorPoolAllocationCount[frameIndex];
	}
	
	int32 &VulkanRenderingAPI::GetSelectedDrawCall()
	{
		return s_VKRendererData->SelectedDrawCall;
	}
}

#endif // HIGHLO_API_VULKAN

