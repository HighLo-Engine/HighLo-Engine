// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLUniformBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: UniformBuffer(binding, layout), m_Size(size)
	{
		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance, binding]() mutable
		{
			glGenBuffers(1, &instance->m_RendererID);
			glBindBuffer(GL_UNIFORM_BUFFER, instance->m_RendererID);
			glBufferData(GL_UNIFORM_BUFFER, instance->m_Size, instance->m_Data, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, instance->m_RendererID);
		});
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glDeleteBuffers(1, &instance->m_RendererID);
		});
	}

	void OpenGLUniformBuffer::Bind() const
	{
		Ref<const OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance]()
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, instance->m_Binding, instance->m_RendererID);
		});
	}

	void OpenGLUniformBuffer::Unbind() const
	{
		Renderer::Submit([]()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		});
	}
	
	void OpenGLUniformBuffer::UploadToShader(bool now)
	{
		if (now)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, m_Size, m_Data);
			return;
		}

		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance]()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, instance->m_RendererID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, instance->m_Size, instance->m_Data);
		});
	}
}

#endif
