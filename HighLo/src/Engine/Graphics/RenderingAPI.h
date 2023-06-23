// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Math/Math.h"

#include "CommandBuffer.h"
#include "RenderPass.h"
#include "ComputePipeline.h"
#include "Engine/Renderer/Environment.h"
#include "Engine/Graphics/Shaders/UniformBufferSet.h"
#include "Engine/Graphics/Shaders/StorageBufferSet.h"
#include "Engine/Graphics/Meshes/DynamicModel.h"
#include "Engine/Graphics/Meshes/StaticModel.h"

namespace highlo
{
	class VertexArray;
	class SceneRenderer;

	enum class PrimitiveType
	{
		None = 0,
		Triangles,
		TriangleStrip,
		Lines,
		LineStrip,
		Patch
	};

	class RenderingAPI
	{
	public:

		HLAPI virtual void Init() = 0;
		HLAPI virtual void Shutdown() = 0;

		HLAPI virtual void BeginFrame() = 0;
		HLAPI virtual void EndFrame() = 0;

		HLAPI virtual void BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear = false) = 0;
		HLAPI virtual void EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer) = 0;

		HLAPI virtual void ClearScreenColor(const glm::vec4 &color) = 0;
		HLAPI virtual void ClearScreenBuffers() = 0;

		HLAPI virtual void DrawIndexed(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Triangles) = 0;
		HLAPI virtual void DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type = PrimitiveType::Triangles, bool depthTest = true, const glm::mat4 &localTransform = glm::mat4(1.0f)) = 0;
		HLAPI virtual void DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type = PrimitiveType::Triangles) = 0;
		HLAPI virtual void DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Patch) = 0;

		HLAPI virtual void DrawFullscreenQuad(
			const Ref<CommandBuffer> &renderCommandBuffer, 
			const Ref<VertexArray> &va, 
			const Ref<UniformBufferSet> &uniformBufferSet, 
			const Ref<StorageBufferSet> &storageBufferSet, 
			Ref<Material> &material, 
			const glm::mat4 &transform = glm::mat4(1.0f)
		) = 0;

		HLAPI virtual void DrawStaticMesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<StaticModel> &model,
			uint32 submeshIndex,
			const Ref<MaterialTable> &materials,
			const Ref<VertexBuffer> &transformBuffer,
			uint32 transformBufferOffset
		) = 0;

		HLAPI virtual void DrawDynamicMesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<DynamicModel> &model,
			uint32 submeshIndex,
			const Ref<MaterialTable> &materials,
			const Ref<VertexBuffer> &transformBuffer,
			uint32 transformBufferOffset
		) = 0;

		HLAPI virtual void DrawInstancedStaticMesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<StaticModel> &model,
			uint32 submeshIndex,
			const Ref<MaterialTable> &materials,
			const Ref<VertexBuffer> &transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount
		) = 0;

		HLAPI virtual void DrawInstancedDynamicMesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<DynamicModel> &model,
			uint32 submeshIndex,
			const Ref<MaterialTable> &materials,
			const Ref<VertexBuffer> &transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount
		) = 0;

		HLAPI virtual void DrawInstancedStaticMeshWithMaterial(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<StaticModel> &model,
			uint32 submeshIndex,
			const Ref<VertexBuffer> &transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount,
			Ref<Material> &overrideMaterial
		) = 0;

		HLAPI virtual void DrawInstancedDynamicMeshWithMaterial(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			Ref<DynamicModel> &model,
			uint32 submeshIndex,
			const Ref<VertexBuffer> &transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount,
			Ref<Material> &overrideMaterial
		) = 0;

		HLAPI virtual void DrawQuad(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<Material> &material,
			const glm::mat4 &transform = glm::mat4(1.0f)
		) = 0;

		HLAPI virtual void DrawInstancedDynamicSubmesh(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			const Ref<DynamicModel> &model, uint32 submeshIndex,
			const Ref<MaterialTable> &materialTable,
			const Ref<VertexBuffer> &transformBuffer, uint32 transformOffset,
			const std::vector<Ref<StorageBuffer>> &boneTransformUBs, uint32 boneTransformsOffset,
			uint32 instanceCount) = 0;

		HLAPI virtual void DrawInstancedStaticMeshWithMaterial(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			const Ref<StaticModel> &model, uint32 submeshIndex,
			const Ref<VertexBuffer> &transformBuffer, uint32 transformOffset,
			const std::vector<Ref<StorageBuffer>> &boneTransformUBs, uint32 boneTransformsOffset,
			uint32 instanceCount,
			const Ref<Material> &material,
			Allocator additionalUniforms = Allocator()) = 0;

		HLAPI virtual void DrawInstancedDynamicMeshWithMaterial(
			const Ref<CommandBuffer> &renderCommandBuffer,
			const Ref<VertexArray> &va,
			const Ref<UniformBufferSet> &uniformBufferSet,
			const Ref<StorageBufferSet> &storageBufferSet,
			const Ref<DynamicModel> &model, uint32 submeshIndex,
			const Ref<VertexBuffer> &transformBuffer, uint32 transformOffset,
			const std::vector<Ref<StorageBuffer>> &boneTransformUBs, uint32 boneTransformsOffset,
			uint32 instanceCount,
			const Ref<Material> &material,
			Allocator additionalUniforms = Allocator()) = 0;

		HLAPI virtual void SetWireframe(bool wf) = 0;
		HLAPI virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
		HLAPI virtual void SetBlendMode(bool bEnabled) = 0;
		HLAPI virtual void SetMultiSample(bool bEnabled) = 0;
		HLAPI virtual void SetDepthTest(bool bEnabled) = 0;
		HLAPI virtual void SetLineThickness(float thickness) = 0;

		HLAPI virtual void SetSceneEnvironment(const Ref<SceneRenderer> &sceneRenderer, Ref<Environment> &environment, const Ref<Texture2D> &shadow) = 0;
		HLAPI virtual Ref<Environment> CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize = 2048, uint32 irradianceMapSize = 32) = 0;
		HLAPI virtual Ref<Texture3D> CreatePreethamSky(float turbidity, float azimuth, float inclination) = 0;
	};
}

