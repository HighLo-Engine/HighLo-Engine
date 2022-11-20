// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#include "Engine/Graphics/VertexArray.h"

namespace highlo
{
	void VulkanRenderingAPI::Init()
	{
	}

	void VulkanRenderingAPI::Shutdown()
	{
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
		return Ref<Texture3D>();
	}

	VkDescriptorSet VulkanRenderingAPI::AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo)
	{
		// TODO
		VkDescriptorSet s;
		return s;
	}

	void VulkanRenderingAPI::UpdateMaterialForRendering(Ref<VulkanMaterial> &material, Ref<UniformBufferSet> &uniformBufferSet, Ref<StorageBufferSet> &storageBufferSet)
	{
	}

	VkSampler VulkanRenderingAPI::GetClampSampler()
	{
		// TODO
		VkSampler sampler;
		return sampler;
	}

	VkSampler VulkanRenderingAPI::GetPointSampler()
	{
		// TODO
		VkSampler sampler;
		return sampler;
	}

	uint32 VulkanRenderingAPI::GetDescriptorAllocationCount(uint32 frameIndex)
	{
		return 0;
	}

	int32 &VulkanRenderingAPI::GetSelectedDrawCall()
	{
		int32 v = 0;
		return v;
	}
}

