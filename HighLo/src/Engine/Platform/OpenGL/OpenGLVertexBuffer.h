// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Allocator.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void *data, uint32 size, VertexBufferUsage usage);
		OpenGLVertexBuffer(uint32 size, VertexBufferUsage usage);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout &GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout &layout) override { m_Layout = layout; }

		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;

		inline virtual HLRendererID GetRendererID() override { return m_ID; }
		inline virtual VertexBufferUsage GetUsage() override { return m_Usage; }

	private:

		BufferLayout m_Layout;
		VertexBufferUsage m_Usage;
		HLRendererID m_ID;
		uint32 m_Size = 0;
		Allocator m_LocalData;
	};
}

#endif // HIGHLO_API_OPENGL
