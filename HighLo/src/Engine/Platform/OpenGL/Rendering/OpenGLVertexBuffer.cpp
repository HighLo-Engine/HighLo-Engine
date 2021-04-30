#include "HighLoPch.h"
#include "OpenGLVertexBuffer.h"

#ifdef HIGHLO_API_OPENGL
#include <glad/glad.h>

namespace highlo
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, size_t size, uint32 stride)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void OpenGLVertexBuffer::Unbind()
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
