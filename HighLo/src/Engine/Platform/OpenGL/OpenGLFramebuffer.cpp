// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLFramebuffer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Application/Application.h"
#include "Engine/Renderer/Renderer.h"
#include "OpenGLTexture.h"
#include "OpenGLUtils.h"

namespace highlo
{
	namespace utils
	{
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, HLRendererID *outID, uint32 count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, HLRendererID id)
		{
			glBindTexture(TextureTarget(multisampled), id);
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

		static Ref<Texture> CreateAndAttachColorAttachment(int32 samples, TextureFormat format, uint32 width, uint32 height, int32 index)
		{
			bool multisampled = samples > 1;
			Ref<Texture> texture;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, OpenGLTextureInternalFormat(format), width, height, GL_FALSE);
			}
			else
			{
				texture = Texture2D::Create(format, width, height, false);
			}

			Ref<OpenGLTexture2D> glTexture = texture.As<OpenGLTexture2D>();
			glTexture->CreateSampler(TextureProperties());
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), glTexture->GetRendererID(), 0);
			return texture;
		}

		static Ref<Texture> CreateAndAttachDepthTexture(int32 samples, TextureFormat format, uint32 width, uint32 height)
		{
			bool multisampled = samples > 1;
			Ref<Texture> texture;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, OpenGLTextureInternalFormat(format), width, height, GL_FALSE);
			}
			else
			{
				texture = Texture2D::Create(format, width, height, false);
			}

			Ref<OpenGLTexture2D> glTexture = texture.As<OpenGLTexture2D>();
			glTexture->CreateSampler(TextureProperties());
			glFramebufferTexture2D(GL_FRAMEBUFFER, utils::DepthAttachmentType(format), TextureTarget(multisampled), glTexture->GetRendererID(), 0);
			return texture;
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
		auto rendererID = m_RendererID;
		Renderer::Submit([rendererID]()
		{
			glDeleteFramebuffers(1, &rendererID);
		});
	}

	void OpenGLFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Specification.Width == width && m_Specification.Height == height) && m_Specification.NoResize)
			return;

		m_Specification.Width = width;
		m_Specification.Height = height;

		Ref<OpenGLFramebuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			if (instance->m_RendererID)
			{
				glDeleteFramebuffers(1, &instance->m_RendererID);
				instance->m_ColorAttachments.clear();
			}

			glGenFramebuffers(1, &instance->m_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, instance->m_RendererID);

			if (instance->m_ColorAttachmentFormats.size())
			{
				instance->m_ColorAttachments.resize(instance->m_ColorAttachmentFormats.size());

				// Create color attachments
				for (size_t i = 0; i < instance->m_ColorAttachments.size(); i++)
					instance->m_ColorAttachments[i] = utils::CreateAndAttachColorAttachment(instance->m_Specification.Samples, instance->m_ColorAttachmentFormats[i], instance->m_Specification.Width, instance->m_Specification.Height, int32(i));
			}

			if (instance->m_DepthAttachmentFormat != TextureFormat::None)
				instance->m_DepthAttachment = utils::CreateAndAttachDepthTexture(instance->m_Specification.Samples, instance->m_DepthAttachmentFormat, instance->m_Specification.Width, instance->m_Specification.Height);

			if (instance->m_ColorAttachments.size() > 1)
			{
				HL_ASSERT(instance->m_ColorAttachments.size() <= 4, "We only support 4 color passes for now");
				GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
				glDrawBuffers((GLsizei)instance->m_ColorAttachments.size(), buffers);
			}
			else if (instance->m_ColorAttachments.empty())
			{
				// Draw only depth-pass
				glDrawBuffer(GL_NONE);
			}
			
			HL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});
	}

	void OpenGLFramebuffer::Bind() const
	{
		Ref<const OpenGLFramebuffer> instance = this;
		Renderer::Submit([instance]()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, instance->m_RendererID);
			glViewport(0, 0, instance->m_Specification.Width, instance->m_Specification.Height);
		});
	}

	void OpenGLFramebuffer::Unbind() const
	{
		Renderer::Submit([]()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});
	}

	void OpenGLFramebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
	{
		Ref<const OpenGLFramebuffer> instance = this;
		Renderer::Submit([instance, slot, attachmentIndex]()
		{
			glBindTextureUnit(slot, instance->m_ColorAttachments[attachmentIndex]->GetRendererID());
		});
	}

	void OpenGLFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
		HL_ASSERT(attachmentIndex < m_ColorAttachments.size());
		auto &format = m_ColorAttachmentFormats[attachmentIndex];

		Ref<OpenGLFramebuffer> instance = this;
		Renderer::Submit([instance, format, attachmentIndex, value]()
		{
			glClearTexImage(instance->m_ColorAttachments[attachmentIndex]->GetRendererID(), 0, utils::HighLoFBTextureFormatToGL(format), GL_INT, &value);
		});
	}
	
	int32 OpenGLFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		HL_ASSERT(attachmentIndex < m_ColorAttachments.size());
		int32 pixelData;

		Renderer::Submit([attachmentIndex, x, y, pixelData]() mutable
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
			glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		});

		return pixelData;
	}
}

#endif // HIGHLO_API_OPENGL