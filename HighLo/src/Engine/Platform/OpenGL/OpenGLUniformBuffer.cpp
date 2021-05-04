#include "HighLoPch.h"
#include "OpenGLUniformBuffer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(const HLString& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32 slot)
		: UniformBuffer(name, layout, parent, slot)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
		glBufferData(GL_UNIFORM_BUFFER, m_DataSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, BindingSlot, m_ID);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLUniformBuffer::UploadToShader()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, m_DataSize, m_Data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

#endif
