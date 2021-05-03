#include "HighLoPch.h"
#include "OpenGLVertexArray.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>
#include "Engine/Renderer/BufferLayout.h"

namespace highlo
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
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_ID);
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

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer)
	{
		if (vertex_buffer->GetLayout().GetElements().size() < 1)
		{
			HL_CORE_ERROR("Vertex Buffer has no layout!");
			return;
		}

		glBindVertexArray(m_ID);
		vertex_buffer->Bind();

		uint32 index = 0;
		const auto& layout = vertex_buffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertex_buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)
	{
		glBindVertexArray(m_ID);
		index_buffer->Bind();

		m_IndexBuffer = index_buffer;
	}

	std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers()
	{
		return m_VertexBuffers;
	}

	Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer()
	{
		return m_IndexBuffer;
	}
}

#endif // HIGHLO_API_OPENGL
