#pragma once

#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_OPENGL
namespace highlo
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(std::vector<int>& indices);
		~OpenGLIndexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual int  GetCount() override { return (int)m_Count; }

	private:
		uint32 m_ID;
		uint32 m_Count = 0;
	};
}

#endif // HIGHLO_API_OPENGL
