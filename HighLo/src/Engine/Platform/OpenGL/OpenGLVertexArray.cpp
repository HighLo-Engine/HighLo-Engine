// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLVertexArray.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/BufferLayout.h"

namespace highlo
{
	namespace utils
	{
		static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
		{
			switch (type)
			{
				case ShaderDataType::Float:    return GL_FLOAT;
				case ShaderDataType::Float2:   return GL_FLOAT;
				case ShaderDataType::Float3:   return GL_FLOAT;
				case ShaderDataType::Float4:   return GL_FLOAT;
				case ShaderDataType::Mat3:     return GL_FLOAT;
				case ShaderDataType::Mat4:     return GL_FLOAT;
				case ShaderDataType::Int:      return GL_INT;
				case ShaderDataType::Int2:     return GL_INT;
				case ShaderDataType::Int3:     return GL_INT;

				// OpenGL has problems with uploading Int4s on the GPU unless
				// they are defined as GL_FLOAT.
				case ShaderDataType::Int4:     return GL_FLOAT;

				case ShaderDataType::Bool:     return GL_BOOL;
			}

			return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray(const VertexArraySpecification& spec)
		: m_Specification(spec)
	{
		HL_ASSERT(spec.Shader);
		HL_ASSERT(spec.RenderPass);
		Invalidate();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::Invalidate()
	{
		if (m_ID)
			glDeleteVertexArrays(1, &m_ID);

		glGenVertexArrays(1, &m_ID);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
	{
		if (vertexBuffer->GetLayout().GetElements().size() < 1)
		{
			HL_CORE_ERROR("Vertex Buffer has no layout!");
			return;
		}

		vertexBuffer->Bind();

		uint32 attribIndex = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(attribIndex);
			glVertexAttribPointer(attribIndex,
				element.GetComponentCount(),
				utils::ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset);
			++attribIndex;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
	{
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	std::vector<Ref<VertexBuffer>> &OpenGLVertexArray::GetVertexBuffers()
	{
		return m_VertexBuffers;
	}

	Ref<IndexBuffer> &OpenGLVertexArray::GetIndexBuffer()
	{
		return m_IndexBuffer;
	}
}

#endif // HIGHLO_API_OPENGL
