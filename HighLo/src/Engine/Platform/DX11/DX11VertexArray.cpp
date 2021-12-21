// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11VertexArray.h"

#include "DX11VertexBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	DX11VertexArray::DX11VertexArray()
	{
	}

	DX11VertexArray::~DX11VertexArray()
	{
	}

	void DX11VertexArray::Bind() const
	{
		UINT offset = 0;
		for (auto i = 0; i < m_VertexBuffers.size(); i++)
		{
			// TODO: @FlareCoding: TEST - This should work
			// TODO: This does not work

			/*
			Ref<DX11VertexBuffer> dxVertexBuffer = m_VertexBuffers[i].As<DX11VertexBuffer>();
			DX11Resources::s_DeviceContext->IASetVertexBuffers(
				i,
				1,
				dxVertexBuffer->GetAddressOf(),
				dxVertexBuffer->GetStridePtr(),
				&offset
			);
			*/
		}
	}

	void DX11VertexArray::Unbind() const {}

	void DX11VertexArray::Invalidate()
	{
	}

	void DX11VertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
	{
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void DX11VertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}
}

#endif // HIGHLO_API_DX11

