// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLUniformBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32 size, uint32 binding)
		: m_Size(size), m_Binding(binding)
	{
		m_LocalStorage = new uint8[size];

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		delete[] m_LocalStorage;

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::Bind() const
	{
	//	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	//	glBindBufferRange(GL_UNIFORM_BUFFER, m_Binding, m_RendererID, 0, m_Size);
	}

	void OpenGLUniformBuffer::Unbind() const
	{
	//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLUniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
		if (size < m_Size)
			memcpy(m_LocalStorage, data, size);

		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glNamedBufferSubData(m_RendererID, offset, size, m_LocalStorage);

		// TODO: what about the offset?
		//void *mappedData = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE);
		//memcpy(mappedData, data, size);
		//glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
}

#endif
