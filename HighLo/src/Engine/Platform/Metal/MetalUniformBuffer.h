// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/Shaders/UniformBuffer.h"

namespace highlo
{
	class MetalUniformBuffer : public UniformBuffer
	{
	public:

		MetalUniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout);
		virtual ~MetalUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;

	private:

		HLRendererID m_RendererID = 0;
		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_METAL

