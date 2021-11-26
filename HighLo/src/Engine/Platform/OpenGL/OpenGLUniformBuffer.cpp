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

		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance, size, binding]() mutable
		{
			glCreateBuffers(1, &instance->m_RendererID);
			glNamedBufferData(instance->m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, instance->m_RendererID);
		});
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		delete[] m_LocalStorage;

		HLRendererID rendererID = m_RendererID;
		Renderer::Submit([rendererID]()
		{
			glDeleteBuffers(1, &rendererID);
		});
	}

	void OpenGLUniformBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
		memcpy(m_LocalStorage, data, size);

		Ref<OpenGLUniformBuffer> instance = this;
		Renderer::Submit([instance, offset, size]()
		{
			glNamedBufferSubData(instance->m_RendererID, offset, size, instance->m_LocalStorage);
		});
	}
}

#endif
