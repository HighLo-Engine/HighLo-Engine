#pragma once

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "engine/rendering/Framebuffer.h"

namespace highlo
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:

		OpenGLFramebuffer(const FramebufferSpecification &spec);
		virtual ~OpenGLFramebuffer();

		virtual void Resize(uint32 width, uint32 height, bool forceRecreate = false) override;
		virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func) override {}

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void BindTexture(uint32 attachmentIndex = 0, uint32 slot = 0) const override;

		inline virtual Ref<Image2D> GetImage(uint32 attachmentIndex = 0) const override { return m_ColorAttachments[attachmentIndex]; }
		inline virtual Ref<Image2D> GetDepthImage() const override { return m_DepthAttachment; }

		inline virtual uint32 GetWidth() const override { return m_Width; }
		inline virtual uint32 GetHeight() const override { return m_Height; }

		virtual void ClearAttachment(uint32 attachmentIndex, int32 value) override;
		virtual int32 ReadPixel(uint32 attachmentIndex, int32 x, int32 y) override;

		inline virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		inline virtual const FramebufferSpecification &GetSpecification() const override { return m_Specification; }

	private:

		FramebufferSpecification m_Specification;
		HLRendererID m_RendererID = 0;

		std::vector<Ref<Image2D>> m_ColorAttachments;
		Ref<Image2D> m_DepthAttachment;

		std::vector<ImageFormat> m_ColorAttachmentFormats;
		ImageFormat m_DepthAttachmentFormat = ImageFormat::None;

		uint32 m_Width = 0;
		uint32 m_Height = 0;
	};
}

#endif // HIGHLO_API_OPENGL