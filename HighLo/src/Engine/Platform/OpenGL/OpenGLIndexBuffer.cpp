// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLIndexBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<int32> &indices)
	{
		m_Count = (uint32)indices.size();
		m_Size = ((uint32)indices.size()) * sizeof(int32);
		m_LocalData = Allocator::Copy(&indices[0], m_Size);

		glCreateBuffers(1, &m_ID);
		Bind();

		glNamedBufferData(m_ID, m_Size, m_LocalData.Data, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(void *data, uint32 size)
	{
		m_Count = size / sizeof(int32);
		m_Size = size;
		m_LocalData = Allocator::Copy(data, size);

		glCreateBuffers(1, &m_ID);
		Bind();

		glNamedBufferData(m_ID, m_Size, m_LocalData.Data, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32 size)
	{
		m_Count = size / sizeof(int32);
		m_Size = size;

		glCreateBuffers(1, &m_ID);
		Bind();

		glNamedBufferData(m_ID, m_Size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		m_Count = size / sizeof(int32);
		m_Size = size;
		m_LocalData = Allocator::Copy(data, size);

		glNamedBufferSubData(m_ID, offset, m_Size, m_LocalData.Data);
	}
	
	void OpenGLIndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
		m_Count = (uint32)indices.size();
		m_Size = ((uint32)indices.size()) * sizeof(int32);
		m_LocalData = Allocator::Copy(&indices[0], m_Size);

		glNamedBufferSubData(m_ID, offset, m_Size, m_LocalData.Data);
	}
}

#endif // HIGHLO_API_OPENGL
