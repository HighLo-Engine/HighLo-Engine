// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Renderer/Renderer.h"

#include "VulkanContext.h"

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
		//VulkanShader::ShaderMaterialDescriptorSet FullscreenQuadDescriptorSet;
		
		//std::unordered_map<SceneRenderer*, std::vector<VulkanShader::ShaderMaterialDescriptorSet>> RendererDescriptorSet;
		
		VkDescriptorSet ActiveRendererDescriptorSet = nullptr;
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32> DescriptorPoolAllocationCount;

		// UniformBufferSet -> Shader Hash -> Frame -> WriteDescriptor
		std::unordered_map<UniformBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> UniformBufferWriteDescriptorCache;
		std::unordered_map<StorageBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> StorageBufferWriteDescriptorCache;

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

		utils::DumpGPUInfos();

		// TODO: when moving to mt, surround with Renderer::Submit()
		// Create Descriptor Pool
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
		uint32 framesInFlight = config.FramesInFlight;
		for (uint32 i = 0; i < framesInFlight; i++)
		{
			VK_CHECK_RESULT(vkCreateDescriptorPool(device, &pool_info, nullptr, &s_VKRendererData->DescriptorPools[i]));
			s_VKRendererData->DescriptorPoolAllocationCount[i] = 0;
		}

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
	}
	
	void VulkanRenderingAPI::EndFrame()
	{
	}
	
	void VulkanRenderingAPI::BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
	{
	}
	
	void VulkanRenderingAPI::EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer)
	{
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
	}
	
	void VulkanRenderingAPI::DrawStaticMesh(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<StaticModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials, const TransformVertexData *transformBuffer, uint32 transformBufferOffset)
	{
	}
	
	void VulkanRenderingAPI::DrawDynamicMesh(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<DynamicModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials, const TransformVertexData *transformBuffer, uint32 transformBufferOffset)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMesh(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<StaticModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials, const TransformVertexData *transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
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

		/*
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

			const auto &radianceMapImageInfo = radianceMap->GetVulkanDescriptorInfo();
			writeDescriptors[0] = *pbrShader->GetDescriptorSet("u_EnvRadianceTex", 1);
			writeDescriptors[0].dstSet = descriptorSet;
			writeDescriptors[0].pImageInfo = &radianceMapImageInfo;

			const auto &irradianceMapImageInfo = irradianceMap->GetVulkanDescriptorInfo();
			writeDescriptors[1] = *pbrShader->GetDescriptorSet("u_EnvIrradianceTex", 1);
			writeDescriptors[1].dstSet = descriptorSet;
			writeDescriptors[1].pImageInfo = &irradianceMapImageInfo;

			const auto &brdfLutImageInfo = Renderer::GetBRDFLutTexture().As<VulkanTexture2D>()->GetVulkanDescriptorInfo();
			writeDescriptors[2] = *pbrShader->GetDescriptorSet("u_BRDFLUTTexture", 1);
			writeDescriptors[2].dstSet = descriptorSet;
			writeDescriptors[2].pImageInfo = &brdfLutImageInfo;

			const auto &shadowImageInfo = shadow.As<VulkanTexture2D>()->GetDescriptorInfo();
			writeDescriptors[3] = *pbrShader->GetDescriptorSet("u_ShadowMapTexture", 1);
			writeDescriptors[3].dstSet = descriptorSet;
			writeDescriptors[3].pImageInfo = &shadowImageInfo;

			const auto vulkanDevice = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			vkUpdateDescriptorSets(vulkanDevice, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);
		});
		*/
	}
	
	Ref<Environment> VulkanRenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		return Ref<Environment>();
	}
	
	Ref<Texture3D> VulkanRenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		return Ref<Texture3D>();
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

