// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once
#include "Engine/Renderer/UniformBuffer.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class DX11UniformBuffer : public UniformBuffer
	{
	public:
		DX11UniformBuffer(const HLString& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32 slot = 0);

		virtual void UploadToShader() override;

	private:
		ComPtr<ID3D11Buffer> m_Buffer;
	};
}

#endif
