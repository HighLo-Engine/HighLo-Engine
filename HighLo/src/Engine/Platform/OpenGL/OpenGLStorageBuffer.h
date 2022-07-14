#pragma once

#include "Engine/Graphics/Shaders/StorageBuffer.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLStorageBuffer : public StorageBuffer
	{
	public:

		OpenGLStorageBuffer(uint32 size, uint32 binding, const std::vector<UniformVariable> &layout = std::vector<UniformVariable>());
		virtual ~OpenGLStorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadToShader() override;
		virtual void Resize(uint32 size) override;

	private:

		HLRendererID m_RendererID = 0;
		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_OPENGL

