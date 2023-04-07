// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLVertexBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL
#include <glad/glad.h>

namespace highlo
{
	namespace utils
	{
		static GLenum ConvertUsageToOpenGL(VertexBufferUsage usage)
		{
			switch (usage)
			{
				case VertexBufferUsage::None:
				case VertexBufferUsage::Static:
					return GL_STATIC_DRAW;

				case VertexBufferUsage::Dynamic:
					return GL_DYNAMIC_DRAW;
			}

			HL_ASSERT(false);
			return 0;
		}
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		m_LocalData = Allocator::Copy(data, size);

		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, instance->m_LocalData.Data, utils::ConvertUsageToOpenGL(instance->m_Usage));
		});
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32 size, VertexBufferUsage usage)
		: m_Usage(usage), m_Size(size)
	{
		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, nullptr, utils::ConvertUsageToOpenGL(instance->m_Usage));
		});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GLuint rendererID = m_ID;
		Renderer::SubmitWithoutResources([rendererID]()
		{
			glDeleteBuffers(1, &rendererID);
		});
	}

	void OpenGLVertexBuffer::Bind() const
	{
		GLuint rendererID = m_ID;
		Renderer::Submit([rendererID]()
		{
			glBindBuffer(GL_ARRAY_BUFFER, rendererID);
		});
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		Renderer::Submit([]()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		});
	}
	
	void OpenGLVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		Bind();

		// Free existing memory
		m_LocalData.Release();

		m_Size = size;
		m_LocalData = Allocator::Copy(data, size);

		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance, offset]()
		{
			glNamedBufferSubData(instance->m_ID, offset, instance->m_Size, instance->m_LocalData.Data);
		});
	}
}

#endif // HIGHLO_API_OPENGL
