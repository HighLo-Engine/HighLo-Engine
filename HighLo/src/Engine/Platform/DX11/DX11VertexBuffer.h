#pragma once

#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class DX11VertexBuffer : public VertexBuffer
	{
	public:
		DX11VertexBuffer(std::vector<Vertex>& vertices);
		DX11VertexBuffer(void* data, size_t size, uint32 stride);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline ID3D11Buffer* Get() const { return m_Buffer.Get(); }
		inline ID3D11Buffer* const* GetAddressOf() const { return m_Buffer.GetAddressOf(); }

		inline UINT GetBufferSize()		const { return m_BufferSize; }
		inline UINT GetStride()			const { return *m_Stride.get(); }
		inline UINT* GetStridePtr()		const { return m_Stride.get(); }

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual void UpdateContents(std::vector<Vertex>& vertices) override;
		virtual void UpdateContents(void* data, size_t size) override;

	private:
		ComPtr<ID3D11Buffer>	m_Buffer;
		std::unique_ptr<UINT>	m_Stride;
		UINT					m_BufferSize;
		BufferLayout			m_Layout;
	};
}

#endif // HIGHLO_API_DX11
