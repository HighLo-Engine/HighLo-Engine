// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		delete[] m_LocalStorage;

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
		memcpy(m_LocalStorage, data, size);

		glNamedBufferSubData(m_RendererID, offset, size, m_LocalStorage);
	}
}

#endif
