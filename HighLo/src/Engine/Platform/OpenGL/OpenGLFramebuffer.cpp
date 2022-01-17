// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLFramebuffer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Application/Application.h"
#include "Engine/Renderer/Renderer.h"
#include "OpenGLTexture2D.h"
#include "OpenGLUtils.h"

namespace highlo
{
	namespace utils
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static GLenum DataType(GLenum format)
		{
			switch (format)
			{
				case GL_RGBA8:
					return GL_UNSIGNED_BYTE;

				case GL_RG16F:
				case GL_RG32F:
				case GL_RGBA16F:
				case GL_RGBA32F:
					return GL_FLOAT;

				case GL_DEPTH24_STENCIL8:
					return GL_UNSIGNED_INT_24_8;
			}

			HL_ASSERT(false);
			return 0;
		}

		static GLenum DepthAttachmentType(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::DEPTH32F:
					return GL_DEPTH_ATTACHMENT;

				case TextureFormat::DEPTH24STENCIL8:
					return GL_DEPTH_STENCIL_ATTACHMENT;
			}
			HL_ASSERT(false, "Unknown format");
			return 0;
		}

		static Ref<Texture> CreateTexture(int32 samples, TextureFormat format, uint32 width, uint32 height, int32 index)
		{
			bool multisampled = samples > 1;
			Ref<Texture> texture = Texture2D::Create(format, width, height);
			Ref<OpenGLTexture2D> glTexture = texture.As<OpenGLTexture2D>();

			glBindTexture(utils::TextureTarget(multisampled), glTexture->GetRendererID());
			glTexture->CreateSampler(TextureProperties());
			return texture;
		}

		static void AttachColorAttachment(HLRendererID id, int32 samples, GLenum internalFormat, GLenum format, uint32 width, uint32 height, int32 index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, utils::TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthAttachment(HLRendererID id, int32 samples, GLenum format, GLenum attachmentType, uint32 width, uint32 height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, utils::TextureTarget(multisampled), id, 0);
		}

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

		static GLenum HighLoFBTextureFormatToGL(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGBA:       return GL_RGBA8;
				case TextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			HL_ASSERT(false);
			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification &spec)
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

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Specification.Width == width && m_Specification.Height == height) && m_Specification.NoResize)
			return;

		m_Specification.Width = width;
		m_Specification.Height = height;

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			for (uint32 i = 0; i < m_ColorAttachments.size(); ++i)
			{
				m_ColorAttachments[i]->Release();
				m_ColorAttachments[i] = nullptr;
			}

			m_ColorAttachments.clear();
			m_DepthAttachment->Release();
			m_DepthAttachment = nullptr;
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		bool multisampled = m_Specification.Samples > 1;

		if (m_ColorAttachmentFormats.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentFormats.size());

			// Create color attachments
			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				m_ColorAttachments[i] = utils::CreateTexture(m_Specification.Samples, m_ColorAttachmentFormats[i], m_Specification.Width, m_Specification.Height, int32(i));
				switch (m_ColorAttachmentFormats[i])
				{
					case TextureFormat::RGBA:
						utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(i));
						break;

					case TextureFormat::RED_INTEGER:
						utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, int32(i));
						break;
				}
			}
		}

		if (m_DepthAttachmentFormat != TextureFormat::None)
		{
			m_DepthAttachment = utils::CreateTexture(m_Specification.Samples, m_DepthAttachmentFormat, m_Specification.Width, m_Specification.Height, 0);
			glBindTexture(utils::TextureTarget(multisampled), m_DepthAttachment->GetRendererID());
			switch (m_DepthAttachmentFormat)
			{
				case TextureFormat::DEPTH24STENCIL8:
					utils::AttachDepthAttachment(m_DepthAttachment->GetRendererID(), m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			HL_ASSERT(m_ColorAttachments.size() <= 4, "We only support 4 color passes for now");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Draw only depth-pass
			glDrawBuffer(GL_NONE);
		}

		HL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
	{
		glBindTextureUnit(slot, m_ColorAttachments[attachmentIndex]->GetRendererID());
	}

	void OpenGLFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
		HL_ASSERT(attachmentIndex < m_ColorAttachments.size());
		auto &format = m_ColorAttachmentFormats[attachmentIndex];

		glClearTexImage(m_ColorAttachments[attachmentIndex]->GetRendererID(), 0, utils::HighLoFBTextureFormatToGL(format), GL_INT, &value);
	}
	
	int32 OpenGLFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		HL_ASSERT(attachmentIndex < m_ColorAttachments.size());
		int32 pixelData = -1;
		
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		
		return pixelData;
	}
}

#endif // HIGHLO_API_OPENGL

