// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-21) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Core/Allocator.h"

namespace highlo
{
	class MetalIndexBuffer : public IndexBuffer
	{
	public:

		MetalIndexBuffer(std::vector<int32> &indices);
		MetalIndexBuffer(void *data, uint32 size);
		MetalIndexBuffer(uint32 size);
		virtual ~MetalIndexBuffer();

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

#endif // HIGHLO_API_METAL

