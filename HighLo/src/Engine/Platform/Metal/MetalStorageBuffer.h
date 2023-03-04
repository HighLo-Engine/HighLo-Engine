// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/Shaders/StorageBuffer.h"

namespace highlo
{
	class MetalStorageBuffer : public StorageBuffer
	{
	public:

		MetalStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());
		virtual ~MetalStorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;
		virtual void Resize(uint32 size) override;

	private:

		HLRendererID m_RendererID = 0;
		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_METAL

