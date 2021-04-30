#pragma once

#include "Engine/Renderer/Image.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLImage2D : public Image2D
	{
	public:

		OpenGLImage2D(ImageFormat format, uint32 width, uint32 height, Allocator buffer);
		OpenGLImage2D(ImageFormat format, uint32 width, uint32 height, const void *data = nullptr);
		virtual ~OpenGLImage2D();

		virtual void Invalidate() override;
		virtual void Release() override;

		virtual ImageFormat GetFormat() const override { return m_Format; }
		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }

		virtual Allocator GetBuffer() const override { return m_ImageData; }
		virtual Allocator &GetBuffer() override { return m_ImageData; }

		HLRendererID &GetRendererID() { return m_RendererID; }
		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		HLRendererID &GetSamplerRendererID() { return m_SamplerRendererID; }
		virtual HLRendererID GetSamplerRendererID() const override { return m_SamplerRendererID; }

		virtual uint64 GetHash() const override { return (uint64)m_RendererID; }

	private:

		HLRendererID m_RendererID = 0;
		HLRendererID m_SamplerRendererID = 0;

		uint32 m_Width, m_Height;
		ImageFormat m_Format;
		Allocator m_ImageData;
	};
}

#endif // HIGHLO_API_OPENGL