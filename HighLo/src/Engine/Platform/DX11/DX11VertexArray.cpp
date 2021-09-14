// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11VertexArray.h"

#include "DX11VertexBuffer.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	void DX11VertexArray::Bind() const
	{
		UINT offset = 0;
		for (auto i = 0; i < m_VertexBuffers.size(); i++)
		{
			// TODO @FlareCoding: TEST - This should work
			Ref<DX11VertexBuffer> dxVertexBuffer = m_VertexBuffers[i].As<DX11VertexBuffer>();

			DX11Resources::s_DeviceContext->IASetVertexBuffers(
				i,
				1,
				dxVertexBuffer->GetAddressOf(),
				dxVertexBuffer->GetStridePtr(),
				&offset
			);
		}

		m_IndexBuffer->Bind();
	}

	void DX11VertexArray::Unbind() const {}

	void DX11VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer)
	{
		m_VertexBuffers.push_back(vertex_buffer);
	}

	void DX11VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)
	{
		m_IndexBuffer = index_buffer;
	}

	std::vector<Ref<VertexBuffer>>& DX11VertexArray::GetVertexBuffers()
	{
		return m_VertexBuffers;
	}

	Ref<IndexBuffer>& DX11VertexArray::GetIndexBuffer()
	{
		return m_IndexBuffer;
	}
}

#endif // HIGHLO_API_DX11
