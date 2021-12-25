// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Engine/Renderer/RenderingAPI.h"

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "VulkanMaterial.h"

namespace highlo
{
	class VulkanRenderingAPI : public RenderingAPI
	{
	public:

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginRenderPass(Ref<CommandBuffer> renderCommandBuffer, Ref<RenderPass> renderPass, bool shouldClear = false) override;
		virtual void EndRenderPass(Ref<CommandBuffer> renderCommandBuffer) override;

		virtual void RenderDynamicModel(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<DynamicModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materialTable,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformOffset,
			uint32 instanceCount) override;

		virtual void RenderStaticModel(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<StaticModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materialTable,
			const Transform &transform) override;

		virtual void RenderDynamicModelWithMaterial(
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
			Allocator additionalUniforms = Allocator()) override;

		virtual void RenderStaticModelWithMaterial(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<StaticModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materialTable,
			const Transform &transform) override;

		virtual void RenderQuad(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<Material> material,
			const Transform &transform) override;

		virtual void RenderGeometry(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<Material> material,
			Ref<VertexBuffer> vertexBuffer,
			Ref<IndexBuffer> indexBuffer,
			const Transform &transform,
			uint32 indexCount = 0) override;

		virtual void SubmitFullscreenQuad(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<Material> material) override;

		virtual void SubmitFullscreenQuad(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<Material> material) override;

		virtual void SubmitFullscreenQuadWithOverrides(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<Material> material,
			Allocator vertexShaderOverrides,
			Allocator fragmentShaderOverrides) override;

		virtual void DispatchComputeShader(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<ComputePipeline> computePipeline,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<Material> material,
			const glm::ivec3 &groups) override;

		virtual void ClearTexture(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<Texture2D> texture) override;

		virtual Ref<Environment> CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize = 2048, uint32 irradianceMapSize = 32) override;

		// Vulkan-specific

		static void UpdateMaterialForRendering(Ref<VulkanMaterial> material, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet);
		static const std::vector<std::vector<VkWriteDescriptorSet>> &RetrieveOrCreateUniformBufferWriteDescriptors(Ref<UniformBufferSet> uniformBufferSet, Ref<VulkanMaterial> vulkanMaterial);
		static const std::vector<std::vector<VkWriteDescriptorSet>> &RetrieveOrCreateStorageBufferWriteDescriptors(Ref<StorageBufferSet> storageBufferSet, Ref<VulkanMaterial> vulkanMaterial);
		static VkDescriptorSet AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo);
	};
}

#endif // HIGHLO_API_VULKAN

