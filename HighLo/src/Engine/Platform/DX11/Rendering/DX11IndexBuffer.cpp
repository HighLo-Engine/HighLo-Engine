#include "HighLoPch.h"
#include "DX11IndexBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11IndexBuffer::DX11IndexBuffer(std::vector<int>& indices)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		m_BufferSize = (UINT)indices.size();

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DEFAULT;
		vbedsc.ByteWidth = sizeof(int) * (UINT)indices.size();
		vbedsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbedsc.CPUAccessFlags = 0;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = &indices[0];

		DX11Resources::s_Device->CreateBuffer(&vbedsc, &vbdata, m_Buffer.GetAddressOf());
	}

	void DX11IndexBuffer::Bind()
	{
		DX11Resources::s_DeviceContext->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Unbind() {}

	int DX11IndexBuffer::GetCount()
	{
		return (int)m_BufferSize;
	}
}

#endif // HIGHLO_API_DX11
