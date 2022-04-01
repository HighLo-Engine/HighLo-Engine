// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11IndexBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11IndexBuffer::DX11IndexBuffer(std::vector<int32> &indices)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		m_BufferSize = (UINT)indices.size();

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DEFAULT;
		vbedsc.ByteWidth = sizeof(int32) * (UINT)indices.size();
		vbedsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbedsc.CPUAccessFlags = 0;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = &indices[0];

		DX11Resources::s_Device->CreateBuffer(&vbedsc, &vbdata, m_Buffer.GetAddressOf());
	}

	DX11IndexBuffer::DX11IndexBuffer(void *data, uint32 size)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		m_BufferSize = (UINT)size;

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DEFAULT;
		vbedsc.ByteWidth = sizeof(int) * (UINT)size;
		vbedsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbedsc.CPUAccessFlags = 0;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = &data;

		DX11Resources::s_Device->CreateBuffer(&vbedsc, &vbdata, m_Buffer.GetAddressOf());
	}

	DX11IndexBuffer::DX11IndexBuffer(uint32 size)
	{
// TODO: dynamic index buffer
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{
	}

	void DX11IndexBuffer::Bind() const
	{
		DX11Resources::s_DeviceContext->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Unbind() const {}

	void DX11IndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
	}

	void DX11IndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
	}
}

#endif // HIGHLO_API_DX11
