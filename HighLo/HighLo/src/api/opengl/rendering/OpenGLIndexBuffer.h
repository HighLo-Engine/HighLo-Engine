#pragma once
#include <rendering/IndexBuffer.h>

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
		uint32_t m_ID;
		uint32_t m_Count = 0;
	};
}

#endif // HIGHLO_API_OPENGL
