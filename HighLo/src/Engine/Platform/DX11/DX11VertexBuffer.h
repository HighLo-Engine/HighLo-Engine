// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Core/Log.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class DX11VertexBuffer : public VertexBuffer
	{
	public:

		DX11VertexBuffer(void *data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		DX11VertexBuffer(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		virtual ~DX11VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline ID3D11Buffer *Get() const { return m_Buffer.Get(); }
		inline ID3D11Buffer *const *GetAddressOf() const { return m_Buffer.GetAddressOf(); }

		inline UINT GetBufferSize()		const { return m_BufferSize; }
		inline UINT GetStride()			const { return *m_Stride.get(); }
		inline UINT *GetStridePtr()		const { return m_Stride.get(); }

		virtual const BufferLayout &GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout &layout) override { m_Layout = layout; }

		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }
		virtual VertexBufferUsage GetUsage() override { return m_Usage; }

	private:

		ComPtr<ID3D11Buffer>	m_Buffer;
		std::unique_ptr<UINT>	m_Stride;
		UINT					m_BufferSize;
		BufferLayout			m_Layout;
		VertexBufferUsage		m_Usage;
		uint32					m_Size = 0;
		HLRendererID			m_RendererID = 0;
	};
}

#endif // HIGHLO_API_DX11
