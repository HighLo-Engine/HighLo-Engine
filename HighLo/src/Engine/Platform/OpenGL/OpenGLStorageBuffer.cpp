#include "HighLoPch.h"
#include "OpenGLStorageBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout)
		: StorageBuffer(binding, layout), m_Size(size)
	{
		Ref<OpenGLStorageBuffer> instance = this;
		Renderer::Submit([instance, binding]() mutable
		{
			glGenBuffers(1, &instance->m_RendererID);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance->m_RendererID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, instance->m_Size, instance->m_Data, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, instance->m_RendererID);
		});
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
		GLuint rendererID = m_RendererID;
		Renderer::SubmitWithoutResources([rendererID]() mutable
		{
			glDeleteBuffers(1, &rendererID);
		});
	}

	void OpenGLStorageBuffer::Bind() const
	{
		Ref<const OpenGLStorageBuffer> instance = this;
		Renderer::Submit([instance]()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance->m_RendererID);
			glBindBufferRange(GL_SHADER_STORAGE_BUFFER, instance->m_Binding, instance->m_RendererID, 0, instance->m_Size);
		});
	}

	void OpenGLStorageBuffer::Unbind() const
	{
		Renderer::Submit([]()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		});
	}

	void OpenGLStorageBuffer::UploadToShader()
	{
		Ref<OpenGLStorageBuffer> instance = this;
		Renderer::Submit([instance]()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance->m_RendererID);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, instance->m_Size, instance->m_Data);
		});
	}

	void OpenGLStorageBuffer::Resize(uint32 size)
	{
		if (size == m_Size)
			return;

		m_Size = size;

		Ref<OpenGLStorageBuffer> instance = this;
		Renderer::Submit([instance]()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, instance->m_RendererID);
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, instance->m_Size, instance->m_Data);
		});
	}
}

#endif // HIGHLO_API_OPENGL

