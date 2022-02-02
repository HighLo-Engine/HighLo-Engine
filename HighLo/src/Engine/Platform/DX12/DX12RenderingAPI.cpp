// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX12RenderingAPI.h"

#ifdef HIGHLO_API_DX12

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
	
	void DX12RenderingAPI::BeginRenderPass(Ref<CommandBuffer> renderCommandBuffer, Ref<RenderPass> renderPass, bool shouldClear)
	{
	}
	
	void DX12RenderingAPI::EndRenderPass(Ref<CommandBuffer> renderCommandBuffer)
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
	
	void DX12RenderingAPI::DrawQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4 &transform)
	{
	}
	
	void DX12RenderingAPI::DrawInstancedStaticMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
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
	
	Ref<Environment> DX12RenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		return nullptr;
	}
}

#endif // HIGHLO_API_DX12

