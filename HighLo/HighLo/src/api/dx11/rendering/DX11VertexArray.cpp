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
			DX11Resources::s_DeviceContext->IASetVertexBuffers(
				i,
				1,
				static_cast<DX11VertexBuffer*>(m_VertexBuffers.at(i).get())->GetAddressOf(),
				static_cast<DX11VertexBuffer*>(m_VertexBuffers.at(i).get())->GetStridePtr(),
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

	const std::vector<Ref<VertexBuffer>>& DX11VertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const Ref<IndexBuffer>& DX11VertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}

#endif // HIGHLO_API_DX11
