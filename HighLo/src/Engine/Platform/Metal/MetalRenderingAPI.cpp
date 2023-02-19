// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MetalRenderingAPI.h"

#ifdef HIGHLO_API_METAL

namespace highlo
{
	struct MetalRendererData
	{

	};

	static MetalRendererData *s_RendererData = nullptr;

	void MetalRenderingAPI::Init()
	{
		s_RendererData = new MetalRendererData();
	}

	void MetalRenderingAPI::Shutdown()
	{
		delete s_RendererData;
		s_RendererData = nullptr;
	}
	
	void MetalRenderingAPI::BeginFrame()
	{
	}
	
	void MetalRenderingAPI::EndFrame()
	{
	}
	
	void MetalRenderingAPI::BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
	{
	}
	
	void MetalRenderingAPI::EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer)
	{
	}
	
	void MetalRenderingAPI::ClearScreenColor(const glm::vec4 &color)
	{
	}
	
	void MetalRenderingAPI::ClearScreenBuffers()
	{
	}
	
	void MetalRenderingAPI::DrawIndexed(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void MetalRenderingAPI::DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type, bool depthTest, const glm::mat4 &localTransform)
	{
	}
	
	void MetalRenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
	}
	
	void MetalRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void MetalRenderingAPI::DrawFullscreenQuad(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<Material> &material, 
		const glm::mat4 &transform)
	{
	}
	
	void MetalRenderingAPI::DrawStaticMesh(
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
	
	void MetalRenderingAPI::DrawDynamicMesh(
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
	
	void MetalRenderingAPI::DrawInstancedStaticMesh(
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
	
	void MetalRenderingAPI::DrawInstancedDynamicMesh(
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
	
	void MetalRenderingAPI::DrawInstancedStaticMeshWithMaterial(
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
	
	void MetalRenderingAPI::DrawInstancedDynamicMeshWithMaterial(
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
	
	void MetalRenderingAPI::SetWireframe(bool wf)
	{
	}
	
	void MetalRenderingAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
	}
	
	void MetalRenderingAPI::SetBlendMode(bool bEnabled)
	{
	}
	
	void MetalRenderingAPI::SetMultiSample(bool bEnabled)
	{
	}
	
	void MetalRenderingAPI::SetDepthTest(bool bEnabled)
	{
	}
	
	void MetalRenderingAPI::SetLineThickness(float thickness)
	{
	}
	
	void MetalRenderingAPI::SetSceneEnvironment(const Ref<SceneRenderer> &sceneRenderer, Ref<Environment> &environment, const Ref<Texture2D> &shadow)
	{
	}
	
	Ref<Environment> MetalRenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		return Ref<Environment>();
	}
	
	Ref<Texture3D> MetalRenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		return Ref<Texture3D>();
	}
}

#endif // HIGHLO_API_METAL

