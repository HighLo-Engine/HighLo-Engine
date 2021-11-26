#include "HighLoPch.h"
#include "OpenGLStorageBuffer.h"

#include "Engine/Renderer/Renderer.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32 size, uint32 binding)
		: m_Size(size), m_Binding(binding)
	{
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
	}

	void OpenGLStorageBuffer::SetData(const void *data, uint32 size, uint32 offset)
	{
	}

	void OpenGLStorageBuffer::Resize(uint32 size)
	{
	}
}

#endif // HIGHLO_API_OPENGL

