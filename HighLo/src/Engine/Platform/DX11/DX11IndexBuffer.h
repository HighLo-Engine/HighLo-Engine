#pragma once

#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class DX11IndexBuffer : public IndexBuffer
	{
	public:
		DX11IndexBuffer(std::vector<int>& indices);

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual int32 GetCount() override { return (int32)m_BufferSize; }

	private:
		ComPtr<ID3D11Buffer>	m_Buffer;
		UINT					m_BufferSize;
	};
}

#endif // HIGHLO_API_DX11
