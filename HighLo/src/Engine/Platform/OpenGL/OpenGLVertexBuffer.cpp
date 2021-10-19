// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

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

	OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<Vertex> &vertices, VertexBufferUsage usage)
		: m_Usage(usage)
	{
		m_Size = (uint32)(vertices.size() * sizeof(Vertex));
		m_LocalData = Allocator::Copy(&vertices[0], m_Size);

		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, instance->m_LocalData.m_Data, utils::ConvertUsageToOpenGL(instance->m_Usage));
		});
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, uint32 size, VertexBufferUsage usage)
		: m_Usage(usage)
	{
		m_Size = size;
		m_LocalData = Allocator::Copy(data, size);

		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, instance->m_LocalData.m_Data, utils::ConvertUsageToOpenGL(instance->m_Usage));
		});
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32 size, VertexBufferUsage usage)
		: m_Usage(usage)
	{
		m_Size = size;

		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glCreateBuffers(1, &instance->m_ID);
			glNamedBufferData(instance->m_ID, instance->m_Size, nullptr, utils::ConvertUsageToOpenGL(instance->m_Usage));
		});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GLuint rendererID = m_ID;
		Renderer::Submit([rendererID]()
		{
			glDeleteBuffers(1, &rendererID);
		});
	}

	void OpenGLVertexBuffer::Bind() const
	{
		Ref<const OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance]()
		{
			glBindBuffer(GL_ARRAY_BUFFER, instance->m_ID);
		});
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		Renderer::Submit([]()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		});
	}

	void OpenGLVertexBuffer::UpdateContents(std::vector<Vertex> &vertices, uint32 offset)
	{
		m_Size = ((uint32)vertices.size()) * sizeof(Vertex);
		m_LocalData = Allocator::Copy(&vertices[0], m_Size);

		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance, offset]()
		{
			glNamedBufferSubData(instance->m_ID, offset, instance->m_Size, instance->m_LocalData.m_Data);
		});
	}
	
	void OpenGLVertexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
		m_Size = size;
		m_LocalData = Allocator::Copy(data, size);

		Ref<OpenGLVertexBuffer> instance = this;
		Renderer::Submit([instance, offset]()
		{
			glNamedBufferSubData(instance->m_ID, offset, instance->m_Size, instance->m_LocalData.m_Data);
		});
	}
}

#endif // HIGHLO_API_OPENGL
