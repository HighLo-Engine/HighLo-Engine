// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11VertexBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11VertexBuffer::DX11VertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		if (m_Buffer.Get() != nullptr)
			m_Buffer.Reset();

		m_BufferSize = (UINT)size;
		if (m_Stride.get() == nullptr)
			m_Stride = std::make_unique<UINT>(sizeof(data));

		D3D11_BUFFER_DESC vbedsc;
		ZeroMemory(&vbedsc, sizeof(D3D11_BUFFER_DESC));

		vbedsc.Usage = D3D11_USAGE_DYNAMIC;
		vbedsc.ByteWidth = size;
		vbedsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbedsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbedsc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vbdata;
		ZeroMemory(&vbdata, sizeof(D3D11_SUBRESOURCE_DATA));
		vbdata.pSysMem = &data;

		DX11Resources::s_Device->CreateBuffer(&vbedsc, &vbdata, m_Buffer.GetAddressOf());
	}

	DX11VertexBuffer::DX11VertexBuffer(uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
	}

	void DX11VertexBuffer::Bind() const {}
	void DX11VertexBuffer::Unbind() const {}

	void DX11VertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
// TODO: Add offset
// TODO: This crashes

		/*
		D3D11_MAPPED_SUBRESOURCE resource;
		DX11Resources::s_DeviceContext->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, data, size);
		DX11Resources::s_DeviceContext->Unmap(m_Buffer.Get(), 0);
		*/
	}
}

#endif // HIGHLO_API_DX11
