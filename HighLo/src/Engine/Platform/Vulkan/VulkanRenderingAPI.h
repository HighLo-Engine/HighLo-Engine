// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/RenderingAPI.h"
#include "Vulkan.h"

namespace highlo
{
	class VulkanRenderingAPI : public RenderingAPI
	{
	public:

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear = false) override;
		virtual void EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer) override;

		virtual void ClearScreenColor(const glm::vec4 &color) override;
		virtual void ClearScreenBuffers() override;

		virtual void DrawIndexed(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Triangles) override;
		virtual void DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type = PrimitiveType::Triangles, bool depthTest = true, const glm::mat4 &localTransform = glm::mat4(1.0f)) override;
		virtual void DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type = PrimitiveType::Triangles) override;
		virtual void DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Patch) override;

		virtual void DrawFullscreenQuad(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<Material> &material,
			const glm::mat4 &transform = glm::mat4(1.0f)
		) override;

		virtual void DrawStaticMesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<StaticModel> &model,
			uint32 submeshIndex,
			const Ref<MaterialTable> &materials,
			const TransformVertexData *transformBuffer,
			uint32 transformBufferOffset
		) override;

		virtual void DrawDynamicMesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<DynamicModel> &model,
			uint32 submeshIndex,
			const Ref<MaterialTable> &materials,
			const TransformVertexData *transformBuffer,
			uint32 transformBufferOffset
		) override;

		virtual void DrawInstancedStaticMesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<StaticModel> &model,
			uint32 submeshIndex,
			const Ref<MaterialTable> &materials,
			const TransformVertexData *transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount
		) override;

		virtual void DrawInstancedDynamicMesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<DynamicModel> &model,
			uint32 submeshIndex,
			const Ref<MaterialTable> &materials,
			const TransformVertexData *transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount
		) override;

		virtual void DrawInstancedStaticMeshWithMaterial(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<StaticModel> &model,
			uint32 submeshIndex,
			const TransformVertexData *transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount,
			Ref<Material> &overrideMaterial
		) override;

		virtual void DrawInstancedDynamicMeshWithMaterial(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<DynamicModel> &model,
			uint32 submeshIndex,
			const TransformVertexData *transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount,
			Ref<Material> &overrideMaterial
		) override;

		virtual void SetWireframe(bool wf) override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
		virtual void SetBlendMode(bool bEnabled) override;
		virtual void SetMultiSample(bool bEnabled) override;
		virtual void SetDepthTest(bool bEnabled) override;
		virtual void SetLineThickness(float thickness) override;

		virtual void SetSceneEnvironment(const Ref<SceneRenderer> &sceneRenderer, Ref<Environment> &environment, const Ref<Texture2D> &shadow) override;
		virtual Ref<Environment> CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize = 2048, uint32 irradianceMapSize = 32) override;
		virtual Ref<Texture3D> CreatePreethamSky(float turbidity, float azimuth, float inclination) override;

		// Vulkan-specific
		static VkDescriptorSet AllocateDescriptorSet(VkDescriptorSetAllocateInfo &allocInfo);

		// TODO: move this to vulkan material
	//	static void UpdateMaterialForRendering(Ref<VulkanMaterial> &material, Ref<UniformBufferSet> &uniformBufferSet, Ref<StorageBufferSet> &storageBufferSet);
		
		static VkSampler GetClampSampler();
		static VkSampler GetPointSampler();

		static uint32 GetDescriptorAllocationCount(uint32 frameIndex = 0);
		static int32 &GetSelectedDrawCall();
	};


}

#endif // HIGHLO_API_VULKAN

