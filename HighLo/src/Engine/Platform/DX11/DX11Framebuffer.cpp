// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DX11Framebuffer.h"

#include "Engine/Application/Application.h"

#ifdef HIGHLO_API_DX11

namespace highlo
{
	namespace utils
	{
		static bool IsDepthFormat(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::DEPTH24STENCIL8:
				case TextureFormat::DEPTH32F:
					return true;
			}

			return false;
		}
	}

	DX11Framebuffer::DX11Framebuffer(const FramebufferSpecification &spec)
		: m_Specification(spec)
	{
		HL_ASSERT(spec.Attachments.Attachments.size());
		for (auto format : m_Specification.Attachments.Attachments)
		{
			if (!utils::IsDepthFormat(format.Format))
				m_ColorAttachmentFormats.emplace_back(format.Format);
			else
				m_DepthAttachmentFormat = format.Format;
		}

		uint32 width = spec.Width;
		uint32 height = spec.Height;
		if (0 == width)
			width = HLApplication::Get().GetWindow().GetWidth();

		if (0 == height)
			height = HLApplication::Get().GetWindow().GetHeight();

		Resize(width, height, true);
	}

	DX11Framebuffer::~DX11Framebuffer()
	{
	}

	void DX11Framebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Specification.Width == width && m_Specification.Height == height) && m_Specification.NoResize)
			return;

		m_Specification.Width = width;
		m_Specification.Height = height;

		// TODO: Generate DX11 Framebuffer and set the ColorAttachments and the DepthAttachment
	}

	void DX11Framebuffer::AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func)
	{
	}
	
	void DX11Framebuffer::Bind() const
	{
	}

	void DX11Framebuffer::Unbind() const
	{
	}

	void DX11Framebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
	{
	}

	void DX11Framebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
	}

	int32 DX11Framebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		return 0;
	}


}

#endif // HIGHLO_API_DX11
