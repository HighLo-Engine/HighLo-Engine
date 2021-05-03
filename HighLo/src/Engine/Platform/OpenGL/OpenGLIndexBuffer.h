#pragma once

#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Core/HLLog.h"

#ifdef HIGHLO_API_OPENGL
namespace highlo
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(std::vector<int32>& indices);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual int32 GetCount() const override { return m_Count; }

	private:
		uint32 m_ID;
		uint32 m_Count = 0;
	};
}

#endif // HIGHLO_API_OPENGL
