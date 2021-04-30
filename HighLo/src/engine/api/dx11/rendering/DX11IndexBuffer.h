#pragma once
#include <engine/rendering/IndexBuffer.h>

#ifdef HIGHLO_API_DX11
#include <api/dx11/DX11Resources.h>
#include <vector>

namespace highlo
{
	class DX11IndexBuffer : public IndexBuffer
	{
	public:
		DX11IndexBuffer(std::vector<int>& indices);

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual int  GetCount() override;

	private:
		ComPtr<ID3D11Buffer>	m_Buffer;
		UINT					m_BufferSize;
	};
}

#endif // HIGHLO_API_DX11
