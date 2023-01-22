// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-07-15) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/StorageBuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12StorageBuffer : public StorageBuffer
	{
	public:

		DX12StorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());
		virtual ~DX12StorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;
		virtual void Resize(uint32 size) override;

	private:

		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_DX12

