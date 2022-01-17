// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLVertexBuffer.h"

#include "Engine/Renderer/Renderer.h"

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

	OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		m_LocalData = Allocator::Copy(data, size);

		glCreateBuffers(1, &m_ID);
		Bind();

		glNamedBufferData(m_ID, m_Size, m_LocalData.Data, utils::ConvertUsageToOpenGL(m_Usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		glCreateBuffers(1, &m_ID);
		Bind();
		
		glNamedBufferData(m_ID, m_Size, nullptr, utils::ConvertUsageToOpenGL(m_Usage));
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
	
	void OpenGLVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		Bind();

		// Free existing memory
		m_LocalData.Release();

		m_Size = size;
		m_LocalData = Allocator::Copy(data, size);

		glNamedBufferSubData(m_ID, offset, m_Size, m_LocalData.Data);
	}
}

#endif // HIGHLO_API_OPENGL
