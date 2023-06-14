// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_DX12

#include "Engine/Graphics/VertexArray.h"

namespace highlo
{
	class DX12VertexArray : public VertexArray
	{
	public:

		DX12VertexArray(const VertexArraySpecification &spec);
		virtual ~DX12VertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Invalidate() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>> &GetVertexBuffers() override { return m_VertexBuffers; }
		virtual Ref<IndexBuffer> &GetIndexBuffer() override { return m_IndexBuffer; }

		virtual VertexArraySpecification &GetSpecification() override { return m_Specification; }
		virtual const VertexArraySpecification &GetSpecification() const override { return m_Specification; }

	private:

		VertexArraySpecification m_Specification;
		HLRendererID m_RendererID = 0;

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;
	};
}

#endif // HIGHLO_API_DX12

