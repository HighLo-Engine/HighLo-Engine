// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Core/Log.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

		virtual std::vector<Ref<VertexBuffer>>& GetVertexBuffers() override;
		virtual Ref<IndexBuffer>& GetIndexBuffer() override;

	private:
		uint32 m_ID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}

#endif // HIGHLO_API_OPENGL
