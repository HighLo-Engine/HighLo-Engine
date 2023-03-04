// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Renderer/Renderer.h"
#include "Engine/Graphics/RendererCapabilities.h"

namespace highlo
{
	struct VulkanRendererData
	{
		RendererCapabilities RenderCaps;

		Ref<Texture2D> BRDFLut = nullptr;
		Ref<VertexBuffer> FullscreenQuadVertexBuffer = nullptr;
		Ref<IndexBuffer> FullscreenQuadIndexBuffer = nullptr;
		
		//VulkanShader::ShaderMaterialDescriptorSet QuadDescriptorSet;
		//std::unordered_map<SceneRenderer*, std::vector<VulkanShader::ShaderMaterialDescriptorSet>> RendererDescriptorSet;
		
		VkDescriptorSet ActiveRendererDescriptorSet = nullptr;
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32_t> DescriptorPoolAllocationCount;

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
		return VkDescriptorSet();
	}
	
	VkSampler VulkanRenderingAPI::GetClampSampler()
	{
		return VkSampler();
	}
	
	VkSampler VulkanRenderingAPI::GetPointSampler()
	{
		return VkSampler();
	}
	
	uint32 VulkanRenderingAPI::GetDescriptorAllocationCount(uint32 frameIndex)
	{
		return 0;
	}
	
	int32 &VulkanRenderingAPI::GetSelectedDrawCall()
	{
		return s_VKRendererData->SelectedDrawCall;
	}
}

#endif // HIGHLO_API_VULKAN

