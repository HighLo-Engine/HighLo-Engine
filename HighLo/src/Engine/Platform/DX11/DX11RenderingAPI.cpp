// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11RenderingAPI.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"
#include "Engine/Renderer/SceneRenderer.h"

namespace highlo
{
	void DX11RenderingAPI::Init()
	{
	}

	void DX11RenderingAPI::Shutdown()
	{
	}

	void DX11RenderingAPI::ClearScreenColor(const glm::vec4 &color)
	{
		float bgcolor[] = { color.r, color.g, color.b, color.a };
		DX11Resources::s_DeviceContext->ClearRenderTargetView(DX11Resources::s_RenderTargetView.Get(), bgcolor);
	}

	void DX11RenderingAPI::ClearScreenBuffers()
	{
		DX11Resources::s_DeviceContext->ClearDepthStencilView(DX11Resources::s_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		DX11Resources::s_DeviceContext->RSSetState(DX11Resources::s_RasterizerState.Get());
		DX11Resources::s_DeviceContext->OMSetDepthStencilState(DX11Resources::s_DepthStencilState.Get(), 0);
	}

	void DX11RenderingAPI::BeginFrame()
	{
	}

	void DX11RenderingAPI::EndFrame()
	{
	}

	void DX11RenderingAPI::BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
	{
	}

	void DX11RenderingAPI::EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer)
	{
	}

	void DX11RenderingAPI::DrawIndexed(Ref<VertexArray> &va, PrimitiveType type)
	{
		DX11Resources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11Resources::s_DeviceContext->DrawIndexed(va->GetIndexBuffer()->GetCount(), 0, 0);
	}

	void DX11RenderingAPI::DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type, bool depthTest, const glm::mat4 &localTransform)
	{
		DX11Resources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//																	|	how many instances do we need here? @FlareFlax
		DX11Resources::s_DeviceContext->DrawIndexedInstanced(indexCount, indexCount, 0, 0, 0);
	}

	void DX11RenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
		DX11Resources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		DX11Resources::s_DeviceContext->DrawIndexed(va->GetIndexBuffer()->GetCount(), 0, 0);
	}

	void DX11RenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
		DX11Resources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST);
		DX11Resources::s_DeviceContext->DrawIndexed(va->GetIndexBuffer()->GetCount(), 0, 0);
	}

	void DX11RenderingAPI::DrawFullscreenQuad(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<Material> &material, 
		const glm::mat4 &transform)
	{
	}

	void DX11RenderingAPI::DrawStaticMesh(
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

	void DX11RenderingAPI::DrawDynamicMesh(
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

	void DX11RenderingAPI::DrawInstancedStaticMesh(
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

	void DX11RenderingAPI::DrawInstancedDynamicMesh(
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

	void DX11RenderingAPI::DrawInstancedStaticMeshWithMaterial(
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

	void DX11RenderingAPI::DrawInstancedDynamicMeshWithMaterial(
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

	void DX11RenderingAPI::SetWireframe(bool wf)
	{
		D3D11_RASTERIZER_DESC rasterizerdesc;
		ZeroMemory(&rasterizerdesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerdesc.FillMode = wf ? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerdesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		HRESULT result = DX11Resources::s_Device->CreateRasterizerState(&rasterizerdesc, DX11Resources::s_RasterizerState.GetAddressOf());
		if (FAILED(result))
			HL_CORE_ERROR("Failed to create new rasterizer state");

		DX11Resources::s_DeviceContext->RSSetState(DX11Resources::s_RasterizerState.Get());
	}

	void DX11RenderingAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = (FLOAT)x;
		viewport.TopLeftY = (FLOAT)y;

		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)height;

		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		DX11Resources::s_DeviceContext->RSSetViewports(1, &viewport);
	}

	void DX11RenderingAPI::SetBlendMode(bool bEnabled)
	{
	}

	void DX11RenderingAPI::SetMultiSample(bool bEnabled)
	{
	}

	void DX11RenderingAPI::SetDepthTest(bool bEnabled)
	{
	}

	void DX11RenderingAPI::SetLineThickness(float thickness)
	{
	}

	void DX11RenderingAPI::SetSceneEnvironment(const Ref<SceneRenderer> &sceneRenderer, Ref<Environment> &environment, const Ref<Texture2D> &shadow)
	{
	}

	Ref<Environment> DX11RenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		// TODO
		return Ref<Environment>();
	}
	
	Ref<Texture3D> DX11RenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		// TODO
		return Ref<Texture3D>();
	}
}

#endif // HIGHLO_API_DX11
