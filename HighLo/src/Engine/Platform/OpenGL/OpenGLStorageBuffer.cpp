#include "HighLoPch.h"
#include "OpenGLStorageBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: StorageBuffer(binding, layout), m_Size(size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, m_Data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
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

	void OpenGLStorageBuffer::UploadToShader()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_DataSize, m_Data);
	}

	void OpenGLStorageBuffer::Resize(uint32 size)
	{
		if (size != m_Size)
		{
			m_Size = size;

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, m_Data);
		}
	}
}

#endif // HIGHLO_API_OPENGL

