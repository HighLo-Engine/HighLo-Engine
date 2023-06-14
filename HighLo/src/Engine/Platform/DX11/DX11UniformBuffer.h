// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/UniformBuffer.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class DX11UniformBuffer : public UniformBuffer
	{
	public:

		DX11UniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout);
		virtual ~DX11UniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;

	private:

		ComPtr<ID3D11Buffer> m_Buffer;
		uint32 m_Size = 0;
	};
}

#endif

