#include "HighLoPch.h"
#include "OpenGLFramebuffer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Application/HLApplication.h"
#include "OpenGLTexture.h"

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
			glCreateTextures(TextureTarget(multisampled), 1, outID);
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

		static Ref<Texture> CreateAndAttachColorAttachment(int samples, TextureFormat format, uint32 width, uint32 height, int32 index)
		{
			bool multisampled = samples > 1;
			Ref<Texture> image;
			if (multisampled)
			{
				//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				image = Texture2D::Create(format, width, height);
				image->Invalidate();
			}

			Ref<OpenGLTexture2D> glImage = image.As<OpenGLTexture2D>();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), glImage->GetRendererID(), 0);
			return image;
		}

		static Ref<Texture> AttachDepthTexture(int samples, TextureFormat format, uint32 width, uint32 height)
		{
			bool multisampled = samples > 1;
			Ref<Texture> image;
			if (multisampled)
			{
				//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				image = Texture2D::Create(format, width, height);
				image->Invalidate();
			}

			Ref<OpenGLTexture2D> glImage = image.As<OpenGLTexture2D>();
			glFramebufferTexture2D(GL_FRAMEBUFFER, utils::DepthAttachmentType(format), TextureTarget(multisampled), glImage->GetRendererID(), 0);
			return image;
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

		static GLenum HihloFBTextureFormatToGL(TextureFormat format)
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
		: m_Specification(spec), m_Width(spec.Width), m_Height(spec.Height)
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
		{
			width = HLApplication::Get().GetWindow().GetWidth();
			height = HLApplication::Get().GetWindow().GetHeight();
		}

		Resize(width, height, true);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Width == width && m_Height == height))
			return;

		m_Width = width;
		m_Height = height;

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			m_ColorAttachments.clear();
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		if (m_ColorAttachmentFormats.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentFormats.size());

			// Create Color attachments
			for (int32 i = 0; i < m_ColorAttachments.size(); ++i)
				m_ColorAttachments[i] = utils::CreateAndAttachColorAttachment(m_Specification.Samples, m_ColorAttachmentFormats[i], m_Width, m_Height, i);
		}

		if (m_DepthAttachmentFormat != TextureFormat::None)
		{
			m_DepthAttachment = utils::AttachDepthTexture(m_Specification.Samples, m_DepthAttachmentFormat, m_Width, m_Height);
		}

		if (m_ColorAttachments.size() > 1)
		{
			HL_ASSERT(m_ColorAttachments.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Draw only the depth pass
			glDrawBuffer(GL_NONE);
		}

		HL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
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
		glClearTexImage(m_ColorAttachments[attachmentIndex]->GetRendererID(), 0, utils::HihloFBTextureFormatToGL(format), GL_INT, &value);
	}
	
	int32 OpenGLFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		HL_ASSERT(attachmentIndex < m_ColorAttachments.size());

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int32 pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}
}

#endif // HIGHLO_API_OPENGL