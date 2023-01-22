// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLUniformBuffer.h"

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

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: UniformBuffer(binding, layout), m_Size(size)
	{
		glGenBuffers(1, &m_RendererID);
		temp_gl_error("init - glGenBuffers");
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		temp_gl_error("init - glBindBuffer");
		glBufferData(GL_UNIFORM_BUFFER, m_DataSize, m_Data, GL_DYNAMIC_DRAW);
		temp_gl_error("init - glBufferData");
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
		temp_gl_error("init - glBindBufferBase");
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::Bind() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_RendererID);
		temp_gl_error("glBindBufferBase");
	}

	void OpenGLUniformBuffer::Unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		temp_gl_error("glBindBuffer");
	}
	
	void OpenGLUniformBuffer::UploadToShader()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		temp_gl_error("glBindBuffer");
		glBufferSubData(GL_UNIFORM_BUFFER, 0, m_DataSize, m_Data);
		temp_gl_error("glBufferSubData");
	}
}

#endif
