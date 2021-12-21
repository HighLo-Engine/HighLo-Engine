#pragma once

#include "Engine/Renderer/Shaders/StorageBuffer.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLStorageBuffer : public StorageBuffer
	{
	public:

		OpenGLStorageBuffer(uint32 size, uint32 binding);
		virtual ~OpenGLStorageBuffer();

		virtual void SetData(const void *data, uint32 size, uint32 offset = 0) override;
		virtual void Resize(uint32 size) override;

		virtual uint32 GetBinding() const override { return m_Binding; }

	private:

		uint32 m_Binding = 0;
		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_OPENGL

