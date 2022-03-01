// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Math/Math.h"

#include "Environment.h"
#include "CommandBuffer.h"
#include "RenderPass.h"
#include "ComputePipeline.h"
#include "Engine/Renderer/Shaders/UniformBufferSet.h"
#include "Engine/Renderer/Shaders/StorageBufferSet.h"
#include "Engine/Renderer/Meshes/DynamicModel.h"
#include "Engine/Renderer/Meshes/StaticModel.h"

namespace highlo
{
	class VertexArray;

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

		HLAPI virtual void DrawQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4 &transform = glm::mat4(1.0f)) = 0;

		HLAPI virtual void DrawStaticMesh(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<StaticModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materials,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformBufferOffset) = 0;

		HLAPI virtual void DrawDynamicMesh(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<DynamicModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materials,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformBufferOffset) = 0;

		HLAPI virtual void DrawInstancedStaticMesh(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<StaticModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materials,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount) = 0;

		HLAPI virtual void DrawInstancedDynamicMesh(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<DynamicModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materials,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount) = 0;

		HLAPI virtual void DrawInstancedStaticMeshWithMaterial(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<StaticModel> model,
			uint32 submeshIndex,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount,
			Ref<Material> overrideMaterial) = 0;

		HLAPI virtual void DrawInstancedDynamicMeshWithMaterial(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<DynamicModel> model,
			uint32 submeshIndex,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount,
			Ref<Material> overrideMaterial) = 0;

		HLAPI virtual void SetWireframe(bool wf) = 0;
		HLAPI virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
		HLAPI virtual void SetBlendMode(bool bEnabled) = 0;
		HLAPI virtual void SetMultiSample(bool bEnabled) = 0;
		HLAPI virtual void SetDepthTest(bool bEnabled) = 0;
		HLAPI virtual void SetLineThickness(float thickness) = 0;

		HLAPI virtual Ref<Environment> CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize = 2048, uint32 irradianceMapSize = 32) = 0;
		HLAPI virtual Ref<Texture3D> CreatePreethamSky(float turbidity, float azimuth, float inclination) = 0;
	};
}

