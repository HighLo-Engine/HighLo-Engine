#pragma once

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform/DX11/DX11Resources.h"

namespace highlo
{
	class DX11VertexArray : public VertexArray
	{
	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

		virtual std::vector<Ref<VertexBuffer>>& GetVertexBuffers() override;
		virtual Ref<IndexBuffer>& GetIndexBuffer() override;

	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}

#endif // HIGHLO_API_DX11
