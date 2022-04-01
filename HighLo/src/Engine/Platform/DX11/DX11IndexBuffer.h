// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Core/Log.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class DX11IndexBuffer : public IndexBuffer
	{
	public:

		DX11IndexBuffer(std::vector<int32> &indices);
		DX11IndexBuffer(void *data, uint32 size);
		DX11IndexBuffer(uint32 size);
		virtual ~DX11IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32 GetCount() override { return (uint32)m_BufferSize; }
		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;
		virtual void UpdateContents(std::vector<int32> &indices, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }

	private:

		HLRendererID			m_RendererID = 0;
		ComPtr<ID3D11Buffer>	m_Buffer;
		UINT					m_BufferSize;
	};
}

#endif // HIGHLO_API_DX11
