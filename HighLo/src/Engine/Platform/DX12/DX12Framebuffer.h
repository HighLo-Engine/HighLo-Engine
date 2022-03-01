// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/Renderer/Framebuffer.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12Framebuffer : public Framebuffer
	{
	public:

		DX12Framebuffer(const FramebufferSpecification &spec);
		virtual ~DX12Framebuffer();

		virtual void Resize(uint32 width, uint32 height, bool forceRecreate = false) override;
		virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func) override {}

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void BindTexture(uint32 attachmentIndex = 0, uint32 slot = 0) const override;

		inline virtual Ref<Texture> GetImage(uint32 attachmentIndex = 0) const override { return m_ColorAttachments[attachmentIndex]; }
		inline virtual Ref<Texture> GetDepthImage() const override { return m_DepthAttachment; }

		inline virtual uint32 GetWidth() const override { return m_Specification.Width; }
		inline virtual uint32 GetHeight() const override { return m_Specification.Height; }

		virtual void ClearAttachment(uint32 attachmentIndex, int32 value) override;
		virtual int32 ReadPixel(uint32 attachmentIndex, int32 x, int32 y) override;

		inline virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		inline virtual FramebufferSpecification &GetSpecification() override { return m_Specification; }
		inline virtual const FramebufferSpecification &GetSpecification() const override { return m_Specification; }

	private:

		FramebufferSpecification m_Specification;
		HLRendererID m_RendererID = 0;

		std::vector<Ref<Texture>> m_ColorAttachments;
		Ref<Texture> m_DepthAttachment;

		std::vector<TextureFormat> m_ColorAttachmentFormats;
		TextureFormat m_DepthAttachmentFormat = TextureFormat::None;
	};
}

#endif // HIGHLO_API_DX12

