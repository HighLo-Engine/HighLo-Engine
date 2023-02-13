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

		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, instance->m_LocalData.Data, GL_STATIC_DRAW);
		});
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(void *data, uint32 size)
	{
		m_Count = size / sizeof(int32);
		m_Size = size;
		m_LocalData = Allocator::Copy(data, size);

		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, instance->m_LocalData.Data, GL_STATIC_DRAW);
		});
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32 size)
	{
		m_Count = size / sizeof(int32);
		m_Size = size;

		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, nullptr, GL_DYNAMIC_DRAW);
		});
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glDeleteBuffers(1, &instance->m_ID);
		});
	}

	void OpenGLIndexBuffer::Bind() const
	{
		GLuint rendererID = m_ID;
		Renderer::Submit([rendererID]()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
		});
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		Renderer::Submit([]()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		});
	}

	void OpenGLIndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		m_Count = size / sizeof(int32);
		m_Size = size;
		m_LocalData = Allocator::Copy(data, size);

		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance, offset]() mutable
		{
			glNamedBufferSubData(instance->m_ID, offset, instance->m_Size, instance->m_LocalData.Data);
		});
	}
	
	void OpenGLIndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
		m_Count = (uint32)indices.size();
		m_Size = ((uint32)indices.size()) * sizeof(int32);
		m_LocalData = Allocator::Copy(&indices[0], m_Size);

		Ref<OpenGLIndexBuffer> instance = this;
		Renderer::Submit([instance, offset]() mutable
		{
			glNamedBufferSubData(instance->m_ID, offset, instance->m_Size, instance->m_LocalData.Data);
		});
	}
}

#endif // HIGHLO_API_OPENGL
