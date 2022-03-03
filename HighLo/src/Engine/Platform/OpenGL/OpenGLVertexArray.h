// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/VertexArray.h"
#include "Engine/Core/Log.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray(const VertexArraySpecification& spec);
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Invalidate() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>> &GetVertexBuffers() override;
		virtual Ref<IndexBuffer> &GetIndexBuffer() override;

		virtual VertexArraySpecification &GetSpecification() override { return m_Specification; }
		virtual const VertexArraySpecification &GetSpecification() const override { return m_Specification; }

	private:

		HLRendererID m_ID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		VertexArraySpecification m_Specification;
	};
}

#endif // HIGHLO_API_OPENGL
