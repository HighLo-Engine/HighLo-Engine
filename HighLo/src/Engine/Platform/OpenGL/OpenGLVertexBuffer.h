#pragma once

#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Core/Log.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const std::vector<Vertex>& vertices, VertexBufferUsage usage);
		OpenGLVertexBuffer(void* data, size_t size, uint32 stride, VertexBufferUsage usage);
		OpenGLVertexBuffer(size_t size, VertexBufferUsage usage);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual void UpdateContents(std::vector<Vertex>& vertices) override;
		virtual void UpdateContents(void* data, size_t size) override;

		inline virtual HLRendererID GetRendererID() override { return m_ID; }
		inline virtual VertexBufferUsage GetUsage() override { return m_Usage; }

	private:
		BufferLayout m_Layout;
		VertexBufferUsage m_Usage;
		uint32_t m_ID;
	};
}

#endif // HIGHLO_API_OPENGL
