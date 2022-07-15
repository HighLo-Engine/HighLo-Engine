#pragma once

#include "Engine/Graphics/Shaders/StorageBuffer.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class DX11StorageBuffer : public StorageBuffer
	{
	public:

		DX11StorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());
		virtual ~DX11StorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;
		virtual void Resize(uint32 size) override;

	private:

		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_DX11

