// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-15) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/UniformBuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12UniformBuffer : public UniformBuffer
	{
	public:

		DX12UniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());
		virtual ~DX12UniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;

	private:

		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_DX12

