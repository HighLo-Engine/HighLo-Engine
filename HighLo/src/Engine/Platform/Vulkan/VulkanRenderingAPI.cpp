// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#include "Engine/Renderer/SceneRenderer.h"
#include "Engine/Renderer/Shaders/UniformBufferSet.h"
#include "Engine/Renderer/Shaders/StorageBufferSet.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanContext.h"
#include "VulkanShader.h"

namespace highlo
{
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
	};

	static VulkanRendererData *s_VulkanRendererData = nullptr;

	void VulkanRenderingAPI::Init()
	{
		s_VulkanRendererData = new VulkanRendererData();
	}
	
	void VulkanRenderingAPI::Shutdown()
	{
		VulkanShader::ClearUniformBuffers();
		delete s_VulkanRendererData;
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
	
	void VulkanRenderingAPI::DrawIndexed(uint32 indexCount, PrimitiveType type, bool depthTest)
	{
	}
	
	void VulkanRenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::BeginFrame()
	{
	}
	
	void VulkanRenderingAPI::EndFrame()
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

	Ref<Environment> VulkanRenderingAPI::CreateEnvironment(const HLString &path, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		// TODO
		return nullptr;
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

