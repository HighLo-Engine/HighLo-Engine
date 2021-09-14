// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLVertexBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include <glad/glad.h>

namespace highlo
{
	namespace utils
	{
		static GLenum ConvertUsageToOpenGL(VertexBufferUsage usage)
		{
			switch (usage)
			{
				case VertexBufferUsage::None:
				case VertexBufferUsage::Static:
					return GL_STATIC_DRAW;

				case VertexBufferUsage::Dynamic:
					return GL_DYNAMIC_DRAW;
			}

			HL_ASSERT(false);
			return 0;
		}
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<Vertex>& vertices, VertexBufferUsage usage)
		: m_Usage(usage)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], utils::ConvertUsageToOpenGL(usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, size_t size, uint32 stride, VertexBufferUsage usage)
		: m_Usage(usage)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, utils::ConvertUsageToOpenGL(usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size, VertexBufferUsage usage)
		: m_Usage(usage)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, utils::ConvertUsageToOpenGL(usage));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::UpdateContents(std::vector<Vertex>& vertices)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
	}
	
	void OpenGLVertexBuffer::UpdateContents(void* data, size_t size)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}

#endif // HIGHLO_API_OPENGL
