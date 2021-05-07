#include "HighLoPch.h"
#include "DX11RenderingAPI.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	void DX11RenderingAPI::ClearScreenColor(const glm::vec4& color)
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

	void DX11RenderingAPI::DrawIndexed(Ref<VertexArray>& va)
	{
		DX11Resources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11Resources::s_DeviceContext->DrawIndexed(va->GetIndexBuffer()->GetCount(), 0, 0);
	}

	void DX11RenderingAPI::DrawInstanced(Ref<VertexArray>& va, uint32 count)
	{
		DX11Resources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11Resources::s_DeviceContext->DrawIndexedInstanced(va->GetIndexBuffer()->GetCount(), count, 0, 0, 0);
	}

	void DX11RenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray>& va)
	{
		DX11Resources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		DX11Resources::s_DeviceContext->DrawIndexed(va->GetIndexBuffer()->GetCount(), 0, 0);
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

	Ref<Environment> DX11RenderingAPI::CreateEnvironment(const HLString &path)
	{
		return Ref<Environment>();
	}
}

#endif // HIGHLO_API_DX11
