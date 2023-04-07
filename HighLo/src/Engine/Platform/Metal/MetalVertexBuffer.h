// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-21) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Core/Allocator.h"

namespace highlo
{
	class MetalVertexBuffer : public VertexBuffer
	{
	public:

		MetalVertexBuffer(void *data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		MetalVertexBuffer(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		virtual ~MetalVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }
		virtual VertexBufferUsage GetUsage() override { return m_Usage; }

	private:

		HLRendererID m_RendererID = 0;
		VertexBufferUsage m_Usage;
		Allocator m_Buffer;
		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_METAL

