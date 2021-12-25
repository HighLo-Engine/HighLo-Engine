// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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

		HLAPI virtual void BeginRenderPass(Ref<CommandBuffer> renderCommandBuffer, Ref<RenderPass> renderPass, bool shouldClear = false) = 0;
		HLAPI virtual void EndRenderPass(Ref<CommandBuffer> renderCommandBuffer) = 0;

		HLAPI virtual void RenderDynamicModel(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<DynamicModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materialTable,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformOffset,
			uint32 instanceCount) = 0;

		HLAPI virtual void RenderStaticModel(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<StaticModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materialTable,
			const Transform &transform) = 0;

		HLAPI virtual void RenderDynamicModelWithMaterial(
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
			Allocator additionalUniforms = Allocator()) = 0;

		HLAPI virtual void RenderStaticModelWithMaterial(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet, 
			Ref<StorageBufferSet> storageBufferSet, 
			Ref<StaticModel> model, 
			uint32 submeshIndex, 
			Ref<MaterialTable> materialTable, 
			const Transform &transform) = 0;

		HLAPI virtual void RenderQuad(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<Material> material,
			const Transform &transform) = 0;

		HLAPI virtual void RenderGeometry(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<Material> material,
			Ref<VertexBuffer> vertexBuffer,
			Ref<IndexBuffer> indexBuffer,
			const Transform &transform,
			uint32 indexCount = 0) = 0;

		HLAPI virtual void SubmitFullscreenQuad(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<Material> material) = 0;

		HLAPI virtual void SubmitFullscreenQuad(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<Material> material) = 0;

		HLAPI virtual void SubmitFullscreenQuadWithOverrides(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<Material> material,
			Allocator vertexShaderOverrides,
			Allocator fragmentShaderOverrides) = 0;

		HLAPI virtual void DispatchComputeShader(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<ComputePipeline> computePipeline,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<Material> material,
			const glm::ivec3 &groups) = 0;

		HLAPI virtual void ClearTexture(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<Texture2D> texture) = 0;

		HLAPI virtual Ref<Environment> CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize = 2048, uint32 irradianceMapSize = 32) = 0;
	};
}

