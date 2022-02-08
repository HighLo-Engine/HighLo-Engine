#include "HighLoPch.h"
#include "OpenGLStorageBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32 size, uint32 binding)
		: m_Size(size), m_Binding(binding)
	{
		m_LocalStorage = new uint8[size];

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glNamedBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
		delete[] m_LocalStorage;

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLStorageBuffer::Bind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, m_Binding, m_RendererID, 0, m_Size);
	}

	void OpenGLStorageBuffer::Unbind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OpenGLStorageBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
		memcpy(m_LocalStorage, data, size);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glNamedBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, m_LocalStorage);
	}

	void OpenGLStorageBuffer::Resize(uint32 size)
	{
		if (size != m_Size)
		{
			m_Size = size;

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			glNamedBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, m_LocalStorage);
		}
	}
}

#endif // HIGHLO_API_OPENGL

