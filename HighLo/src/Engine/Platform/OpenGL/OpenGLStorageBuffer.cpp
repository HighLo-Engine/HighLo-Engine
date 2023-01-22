#include "HighLoPch.h"
#include "OpenGLStorageBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	static void temp_gl_error(const char *source)
	{
		GLenum error = glGetError();
		while (error != GL_NO_ERROR)
		{
			HL_CORE_ERROR("GLERROR - source: {0}, error: {1}", source, error);
			error = glGetError();
		}
	}

	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: StorageBuffer(binding, layout), m_Size(size)
	{
		glGenBuffers(1, &m_RendererID);
		temp_gl_error("SB - init - glGenBuffers");
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		temp_gl_error("SB - init - glBindBuffer");
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, m_Data, GL_DYNAMIC_DRAW);
		temp_gl_error("SB - init - glBufferData");
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
		temp_gl_error("SB - init - glBindBufferBase");
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLStorageBuffer::Bind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		temp_gl_error("SB - bind - glBindBuffer");
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, m_Binding, m_RendererID, 0, m_Size);
		temp_gl_error("SB - bind - glBindBufferRange");
	}

	void OpenGLStorageBuffer::Unbind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		temp_gl_error("SB - unbind - glBindBuffer");
	}

	void OpenGLStorageBuffer::UploadToShader()
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		temp_gl_error("SB - UploadToShader - glBindBuffer");
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_DataSize, m_Data);
		temp_gl_error("SB - UploadToShader - glBufferSubData");
	}

	void OpenGLStorageBuffer::Resize(uint32 size)
	{
		if (size != m_Size)
		{
			m_Size = size;

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
			temp_gl_error("SB - Resize - glBindBuffer");
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, m_Data);
			temp_gl_error("SB - Resize - glBufferSubData");
		}
	}
}

#endif // HIGHLO_API_OPENGL

