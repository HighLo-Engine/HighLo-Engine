// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/VertexArray.h"

#include "VulkanContext.h"
#include "VulkanComputePipeline.h"
#include "VulkanShader.h"
#include "VulkanTexture3D.h"

namespace highlo
{
	struct VulkanRendererData
	{
		int32 SelectedDrawCall = -1;
		int32 DrawCallCount = 0;

		Ref<Texture2D> BRDFLut = nullptr;
		Ref<VertexBuffer> QuadVertexBuffer = nullptr;
		Ref<IndexBuffer> QuadIndexBuffer = nullptr;
		//VulkanShader::ShaderMaterialDescriptorSet QuadDescriptorSet;
		
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32> DescriptorPoolAllocationCount;
		VkDescriptorSet ActiveRendererDescriptorSet = nullptr;

		// UniformBufferSet -> Shader Hash -> Frame -> WriteDescriptor
		std::unordered_map<UniformBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> UniformBufferWriteDescriptorCache;
		std::unordered_map<StorageBufferSet*, std::unordered_map<uint64, std::vector<std::vector<VkWriteDescriptorSet>>>> StorageBufferWriteDescriptorCache;
		//std::unordered_map<SceneRenderer*, std::vector<VulkanShader::ShaderMaterialDescriptorSet>> RendererDescriptorSets;

		// Default samplers
		VkSampler SamplerClamp = nullptr;
		VkSampler SamplerPoint = nullptr;
	};

	static VulkanRendererData *s_VKRendererData = nullptr;

	void VulkanRenderingAPI::Init()
	{
		s_VKRendererData = new VulkanRendererData();
	}

	void VulkanRenderingAPI::Shutdown()
	{
		delete s_VKRendererData;
	}
	
	void VulkanRenderingAPI::BeginFrame()
	{
	}
	
	void VulkanRenderingAPI::EndFrame()
	{
	}
	
	void VulkanRenderingAPI::BeginRenderPass(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<RenderPass> &renderPass, 
		bool shouldClear)
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
	
	void VulkanRenderingAPI::DrawIndexed(
		uint32 indexCount, 
		Ref<Material> &material, 
		Ref<UniformBufferSet> &uniformBufferSet, 
		PrimitiveType type, 
		bool depthTest,
		const glm::mat4 &localTransform)
	{
	}

	void VulkanRenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawFullscreenQuad(
		Ref<CommandBuffer> &renderCommandBuffer, 
		Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<Material> &material, 
		const glm::mat4 &transform)
	{
	}
	
	void VulkanRenderingAPI::DrawStaticMesh(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<StaticModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materials, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformBufferOffset)
	{
	}
	
	void VulkanRenderingAPI::DrawDynamicMesh(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<DynamicModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materials, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformBufferOffset)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMesh(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<StaticModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materials, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedDynamicMesh(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<DynamicModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materials, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedStaticMeshWithMaterial(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<StaticModel> model, 
		uint32 submeshIndex, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount, 
		Ref<Material> overrideMaterial)
	{
	}
	
	void VulkanRenderingAPI::DrawInstancedDynamicMeshWithMaterial(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<DynamicModel> model, 
		uint32 submeshIndex, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount, 
		Ref<Material> overrideMaterial)
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
		return Ref<Environment>();
	}
	
	Ref<Texture3D> VulkanRenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		const uint32 cubemapSize = Renderer::GetConfig().EnvironmentMapResolution;
		const uint32 irradianceMapSize = 32;

		Ref<Texture3D> envMap = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);

		Ref<Shader> preethamSkyShader = Renderer::GetShaderLibrary()->Get("PreethamSky");
		Ref<VulkanComputePipeline> preethamSkyComputePipeline = Ref<VulkanComputePipeline>::Create(preethamSkyShader);

		glm::vec3 params = { turbidity, azimuth, inclination };
		Renderer::Submit([preethamSkyComputePipeline, envMap, cubemapSize, params]() mutable
		{
			VkDevice device = VulkanContext::GetVulkanCurrentDevice()->GetNativeDevice();
			Ref<VulkanShader> shader = preethamSkyComputePipeline->GetShader().As<VulkanShader>();

			std::array<VkWriteDescriptorSet, 1> writeDescriptors;
			auto descriptorSet = shader->CreateDescriptorSets();
			Ref<VulkanTexture3D> envUnfilteredCubemap = envMap.As<VulkanTexture3D>();
			writeDescriptors[0] = *shader->GetDescriptorSet("o_CubeMap");
			writeDescriptors[0].dstSet = descriptorSet.Descriptors[0]; // Should this be set inside the shader?
		//	writeDescriptors[0].pImageInfo = &envUnfilteredCubemap->GetVulkanDescriptorInfo();

			vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, NULL);

			preethamSkyComputePipeline->Begin();
		//	preethamSkyComputePipeline->SetPushConstants(&params, sizeof(glm::vec3));
		//	preethamSkyComputePipeline->Dispatch(descriptorSet.DescriptorSets[0], cubemapSize / 32, cubemapSize / 32, 6);
			preethamSkyComputePipeline->End();

			envUnfilteredCubemap->GenerateMips(true);
		});

		return envMap;
	}

	VkDescriptorSet VulkanRenderingAPI::AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo)
	{
		uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
		allocInfo.descriptorPool = s_VKRendererData->DescriptorPools[bufferIndex];
		VkDevice device = VulkanContext::GetVulkanCurrentDevice()->GetNativeDevice();

		VkDescriptorSet result;
		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &result));
		s_VKRendererData->DescriptorPoolAllocationCount[bufferIndex] += allocInfo.descriptorSetCount;
		return result;
	}

	void VulkanRenderingAPI::UpdateMaterialForRendering(Ref<VulkanMaterial> &material, Ref<UniformBufferSet> &uniformBufferSet, Ref<StorageBufferSet> &storageBufferSet)
	{
	}

	VkSampler VulkanRenderingAPI::GetClampSampler()
	{
		if (s_VKRendererData->SamplerClamp)
			return s_VKRendererData->SamplerClamp;

		VkSamplerCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.maxAnisotropy = 1.0f;
		createInfo.magFilter = VK_FILTER_LINEAR;
		createInfo.minFilter = VK_FILTER_LINEAR;
		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.addressModeV = createInfo.addressModeU;
		createInfo.addressModeW = createInfo.addressModeU;
		createInfo.mipLodBias = 0.0f;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = 100.0f;
		createInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

		VkDevice device = VulkanContext::GetVulkanCurrentDevice()->GetNativeDevice();
		VK_CHECK_RESULT(vkCreateSampler(device, &createInfo, nullptr, &s_VKRendererData->SamplerClamp));
		return s_VKRendererData->SamplerClamp;
	}

	VkSampler VulkanRenderingAPI::GetPointSampler()
	{
		if (s_VKRendererData->SamplerPoint)
			return s_VKRendererData->SamplerPoint;

		VkSamplerCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.maxAnisotropy = 1.0f;
		createInfo.magFilter = VK_FILTER_NEAREST;
		createInfo.minFilter = VK_FILTER_NEAREST;
		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.mipLodBias = 0.0f;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = 100.0f;
		createInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

		VkDevice device = VulkanContext::GetVulkanCurrentDevice()->GetNativeDevice();
		VK_CHECK_RESULT(vkCreateSampler(device, &createInfo, nullptr, &s_VKRendererData->SamplerPoint));
		return s_VKRendererData->SamplerPoint;
	}

	uint32 VulkanRenderingAPI::GetDescriptorAllocationCount(uint32 frameIndex)
	{
		HL_ASSERT(frameIndex < s_VKRendererData->DescriptorPoolAllocationCount.size());
		return s_VKRendererData->DescriptorPoolAllocationCount[frameIndex];
	}

	int32 &VulkanRenderingAPI::GetSelectedDrawCall()
	{
		return s_VKRendererData->SelectedDrawCall;
	}
}

#endif // HIGHLO_API_VULKAN

