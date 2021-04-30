#pragma once

#include "engine/rendering/VertexBuffer.h"
#include "engine/logging/Logger.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const std::vector<Vertex>& vertices);
		OpenGLVertexBuffer(void* data, size_t size, uint32 stride);
		~OpenGLVertexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual void UpdateContents(std::vector<Vertex>& vertices) override;
		virtual void UpdateContents(void* data, size_t size) override;

	private:
		BufferLayout m_Layout;
		uint32_t m_ID;
	};
}

#endif // HIGHLO_API_OPENGL
