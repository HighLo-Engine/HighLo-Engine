// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLUniformBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(const HLString &name, const std::vector<UniformVariable> &layout, UniformBufferParentShader parent, uint32 slot)
		: UniformBuffer(name, layout, parent, slot)
	{
		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glBindBuffer(GL_UNIFORM_BUFFER, instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, nullptr, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, instance->BindingSlot, instance->m_ID);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		});
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		GLuint rendererID = m_ID;
		Renderer::Submit([rendererID]()
		{
			glDeleteBuffers(1, &rendererID);
		});
	}

	void OpenGLUniformBuffer::UploadToShader(uint32 offset)
	{
		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance, offset]()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, instance->m_ID);
			glNamedBufferSubData(instance->m_ID, offset, instance->m_Size, instance->m_Data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		});
	}
}

#endif
