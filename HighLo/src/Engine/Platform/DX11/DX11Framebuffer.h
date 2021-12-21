// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

// TODO: @FlareCoding

#ifdef HIGHLO_API_DX11

#include "Engine/Renderer/Framebuffer.h"

namespace highlo
{
	class DX11Framebuffer : public Framebuffer
	{
	public:

		DX11Framebuffer(const FramebufferSpecification &spec);
		virtual ~DX11Framebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32 width, uint32 height, bool forceRecreate = false) override;
		virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func) override;

		virtual void BindTexture(uint32 attachmentIndex = 0, uint32 slot = 0) const override;
		virtual uint32 GetWidth() const override { return m_Specification.Width; }
		virtual uint32 GetHeight() const override { return m_Specification.Height; }

		virtual void ClearAttachment(uint32 attachmentIndex, int32 value) override;
		virtual int32 ReadPixel(uint32 attachmentIndex, int32 x, int32 y) override;

		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual Ref<Texture> GetImage(uint32 attachmentIndex = 0) const override { return m_ColorAttachments[attachmentIndex]; }
		virtual Ref<Texture> GetDepthImage() const override { return m_DepthAttachment; }

		virtual FramebufferSpecification &GetSpecification() override { return m_Specification; }
		virtual const FramebufferSpecification &GetSpecification() const override { return m_Specification; }

	private:

		FramebufferSpecification m_Specification;
		HLRendererID m_RendererID = 0;

		std::vector<Ref<Texture>> m_ColorAttachments;
		Ref<Texture> m_DepthAttachment;

		std::vector<TextureFormat> m_ColorAttachmentFormats;
		TextureFormat m_DepthAttachmentFormat = TextureFormat::None;
	};
}

#endif // HIGHLO_API_DX11