// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/RenderingAPI.h"
#include "Engine/Core/Log.h"
#include "Engine/Renderer/VertexArray.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	class DX11RenderingAPI : public RenderingAPI
	{
	public:

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void ClearScreenColor(const glm::vec4 &color) override;
		virtual void ClearScreenBuffers() override;

		virtual void BeginRenderPass(Ref<CommandBuffer> renderCommandBuffer, Ref<RenderPass> renderPass, bool shouldClear = false) override;
		virtual void EndRenderPass(Ref<CommandBuffer> renderCommandBuffer) override;

		virtual void DrawIndexed(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Triangles) override;
		virtual void DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type = PrimitiveType::Triangles, bool depthTest = true, const glm::mat4 &localTransform = glm::mat4(1.0f)) override;
		virtual void DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type = PrimitiveType::Triangles) override;
		virtual void DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type = PrimitiveType::Patch) override;

		virtual void DrawQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4 &transform = glm::mat4(1.0f)) override;
		virtual void DrawInstancedStaticMesh(
			Ref<CommandBuffer> renderCommandBuffer,
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet,
			Ref<StorageBufferSet> storageBufferSet,
			Ref<StaticModel> model,
			uint32 submeshIndex,
			Ref<MaterialTable> materials,
			Ref<VertexBuffer> transformBuffer,
			uint32 transformBufferOffset,
			uint32 instanceCount) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void SetWireframe(bool wf) override;
		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
		virtual void SetBlendMode(bool bEnabled) override;
		virtual void SetMultiSample(bool bEnabled) override;
		virtual void SetDepthTest(bool bEnabled) override;
		virtual void SetLineThickness(float thickness) override;

		virtual Ref<Environment> CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize = 2048, uint32 irradianceMapSize = 32) override;
	};
}


#endif // HIGHLO_API_DX11
