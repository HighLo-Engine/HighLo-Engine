// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12RenderingAPI.h"

#ifdef HIGHLO_API_DX12

#include "Engine/Renderer/SceneRenderer.h"

namespace highlo
{
	void DX12RenderingAPI::Init()
	{
	}

	void DX12RenderingAPI::Shutdown()
	{
	}
	
	void DX12RenderingAPI::BeginFrame()
	{
	}
	
	void DX12RenderingAPI::EndFrame()
	{
	}
	
	void DX12RenderingAPI::BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
	{
	}
	
	void DX12RenderingAPI::EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer)
	{
	}
	
	void DX12RenderingAPI::ClearScreenColor(const glm::vec4 &color)
	{
	}
	
	void DX12RenderingAPI::ClearScreenBuffers()
	{
	}
	
	void DX12RenderingAPI::DrawIndexed(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void DX12RenderingAPI::DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type, bool depthTest, const glm::mat4 &localTransform)
	{
	}
	
	void DX12RenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
	}
	
	void DX12RenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
	}

	void DX12RenderingAPI::DrawFullscreenQuad(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<Material> &material, 
		const glm::mat4 &transform)
	{
	}

	void DX12RenderingAPI::DrawStaticMesh(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<StaticModel> &model, 
		uint32 submeshIndex, 
		const Ref<MaterialTable> &materials, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset)
	{
	}

	void DX12RenderingAPI::DrawDynamicMesh(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<DynamicModel> &model, 
		uint32 submeshIndex, 
		const Ref<MaterialTable> &materials, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset)
	{
	}
	
	void DX12RenderingAPI::DrawInstancedStaticMesh(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<StaticModel> &model, 
		uint32 submeshIndex, 
		const Ref<MaterialTable> &materials, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount)
	{
	}

	void DX12RenderingAPI::DrawInstancedDynamicMesh(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<DynamicModel> &model, 
		uint32 submeshIndex, 
		const Ref<MaterialTable> &materials, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount)
	{
	}

	void DX12RenderingAPI::DrawInstancedStaticMeshWithMaterial(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<StaticModel> &model, 
		uint32 submeshIndex, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount, 
		Ref<Material> &overrideMaterial)
	{
	}

	void DX12RenderingAPI::DrawInstancedDynamicMeshWithMaterial(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va,
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<DynamicModel> &model, 
		uint32 submeshIndex, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount, 
		Ref<Material> &overrideMaterial)
	{
	}
	
	void DX12RenderingAPI::SetWireframe(bool wf)
	{
	}
	
	void DX12RenderingAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
	}
	
	void DX12RenderingAPI::SetBlendMode(bool bEnabled)
	{
	}
	
	void DX12RenderingAPI::SetMultiSample(bool bEnabled)
	{
	}
	
	void DX12RenderingAPI::SetDepthTest(bool bEnabled)
	{
	}
	
	void DX12RenderingAPI::SetLineThickness(float thickness)
	{
	}
	
	void DX12RenderingAPI::SetSceneEnvironment(const Ref<SceneRenderer> &sceneRenderer, Ref<Environment> &environment, const Ref<Texture2D> &shadow)
	{
	}

	Ref<Environment> DX12RenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		// TODO
		return nullptr;
	}
	
	Ref<Texture3D> DX12RenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		// TODO
		return Ref<Texture3D>();
	}
}

#endif // HIGHLO_API_DX12

