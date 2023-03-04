// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_DX12

#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Core/Allocator.h"

namespace highlo
{
	class DX12IndexBuffer : public IndexBuffer
	{
	public:

		DX12IndexBuffer(std::vector<int32> &indices);
		DX12IndexBuffer(void *data, uint32 size);
		DX12IndexBuffer(uint32 size);
		virtual ~DX12IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32 GetCount() override { return m_Count; }
		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;
		virtual void UpdateContents(std::vector<int32> &indices, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }

	private:

		HLRendererID m_RendererID = 0;
		uint32 m_Size = 0;
		uint32 m_Count = 0;
		Allocator m_Buffer;
	};
}

#endif // HIGHLO_API_DX12

