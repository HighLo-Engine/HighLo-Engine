// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLFramebuffer.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Utils/ImageUtils.h"

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
		if (spec.Width == 0)
		{
			m_Specification.Width = HLApplication::Get().GetWindow().GetWidth();
			m_Specification.Height = HLApplication::Get().GetWindow().GetHeight();
		}
		else
		{
			m_Specification.Width = m_Specification.Width * (uint32)m_Specification.Scale;
			m_Specification.Height = m_Specification.Height * (uint32)m_Specification.Height;
		}

		uint32 attachmentIndex = 0;
		if (!m_Specification.SwapChainTarget)
		{
			for (auto &attachmentSpec : m_Specification.Attachments.Attachments)
			{
				if (m_Specification.ExistingImage && m_Specification.ExistingImage->GetSpecification().Layers > 1)
				{
					if (utils::IsDepthFormat(attachmentSpec.Format))
					{
						m_DepthAttachment = m_Specification.ExistingImage;
					}
					else
					{
						m_ColorAttachments.emplace_back(m_Specification.ExistingImage);
					}
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					if (!utils::IsDepthFormat(attachmentSpec.Format))
					{
						m_ColorAttachments.emplace_back(); // This will be set later
					}
				}
				else if (utils::IsDepthFormat(attachmentSpec.Format))
				{
					TextureSpecification textureSpec;
					textureSpec.Format = attachmentSpec.Format;
					textureSpec.Usage = TextureUsage::Attachment;
					textureSpec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
					textureSpec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
					textureSpec.DebugName = fmt::format("{0}-DepthAttachment{1}", m_Specification.DebugName.IsEmpty() ? "unnamed FB" : *m_Specification.DebugName, attachmentIndex);
					m_DepthAttachment = Texture2D::CreateFromSpecification(textureSpec);
				}
				else
				{
					TextureSpecification textureSpec;
					textureSpec.Format = attachmentSpec.Format;
					textureSpec.Usage = TextureUsage::Attachment;
					textureSpec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
					textureSpec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
					textureSpec.DebugName = fmt::format("{0}-ColorAttachment{1}", m_Specification.DebugName.IsEmpty() ? "unnamed FB" : *m_Specification.DebugName, attachmentIndex);
					m_ColorAttachments.emplace_back(Texture2D::CreateFromSpecification(textureSpec));
				}

				++attachmentIndex;
			}
		}

		HL_ASSERT(spec.Attachments.Attachments.size());
		Resize(m_Specification.Width, m_Specification.Height, true);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		Release();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
		
			if (!m_Specification.ExistingFramebuffer)
			{
				uint32 attachmentIndex = 0;
				for (auto &texture : m_ColorAttachments)
				{
					if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
						continue;

					if (texture->GetSpecification().Layers > 1 || attachmentIndex == 0)
						texture->Release();

					++attachmentIndex;
				}

				if (m_DepthAttachment)
				{
					if (m_Specification.ExistingImages.find((uint32)m_Specification.Attachments.Attachments.size() - 1) == m_Specification.ExistingImages.end())
						m_DepthAttachment->Release();
				}
			}
		}

		if (m_Specification.ExistingFramebuffer)
			m_ColorAttachments.clear();

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		bool multisampled = m_Specification.Samples > 1;
		bool createImages = m_ColorAttachments.empty();

		uint32 attachmentIndex = 0;
		for (auto &attachmentSpec : m_Specification.Attachments.Attachments)
		{
			if (utils::IsDepthFormat(attachmentSpec.Format))
			{
				if (m_Specification.ExistingImage)
				{
					m_DepthAttachment = m_Specification.ExistingImage;
				}
				else if (m_Specification.ExistingFramebuffer)
				{
					Ref<Framebuffer> &existingFramebuffer = m_Specification.ExistingFramebuffer;
					m_DepthAttachment = existingFramebuffer->GetDepthImage();
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					auto &texture = m_Specification.ExistingImages.at(attachmentIndex);
					HL_ASSERT(utils::IsDepthFormat(texture->GetFormat()));
					m_DepthAttachment = texture;
				}
				else
				{
					auto &depthTextureSpec = m_DepthAttachment->GetSpecification();
					depthTextureSpec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
					depthTextureSpec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
					m_DepthAttachment->Invalidate();
				}

				glBindTexture(utils::TextureTarget(multisampled), m_DepthAttachment->GetRendererID());
				switch (m_DepthAttachmentFormat)
				{
					case TextureFormat::DEPTH24STENCIL8:
						utils::AttachDepthAttachment(m_DepthAttachment->GetRendererID(), m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
						break;

					case TextureFormat::DEPTH32F:
						utils::AttachDepthAttachment(m_DepthAttachment->GetRendererID(), m_Specification.Samples, GL_DEPTH_COMPONENT32F, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
						break;
				}
			}
			else
			{
				Ref<Texture> colorAttachment;
				if (m_Specification.ExistingFramebuffer)
				{
					auto &existingFramebuffer = m_Specification.ExistingFramebuffer;
					auto &existingTexture = existingFramebuffer->GetImage(attachmentIndex);
					colorAttachment = m_ColorAttachments.emplace_back(existingTexture);
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					auto &existingTexture = m_Specification.ExistingImages.at(attachmentIndex);
					HL_ASSERT(!utils::IsDepthFormat(existingTexture->GetFormat()));
					colorAttachment = existingTexture;
					m_ColorAttachments[attachmentIndex] = existingTexture;
				}
				else
				{
					if (createImages)
					{
						TextureSpecification textureSpec;
						textureSpec.Format = attachmentSpec.Format;
						textureSpec.Usage = TextureUsage::Attachment;
						textureSpec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
						textureSpec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
						textureSpec.DebugName = fmt::format("{0}-ColorAttachment{1}", m_Specification.DebugName.IsEmpty() ? "unnamed FB" : *m_Specification.DebugName, attachmentIndex);
						colorAttachment = Texture2D::CreateFromSpecification(textureSpec);
					}
					else
					{
						auto &texture = m_ColorAttachments[attachmentIndex];
						auto &spec = texture->GetSpecification();
						spec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
						spec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
						colorAttachment = texture;

						if (colorAttachment->GetSpecification().Layers > 1)
						{
							colorAttachment->Invalidate();
						}
						else if (attachmentIndex == 0 && m_Specification.ExistingImageLayers[0] == 0)
						{
							colorAttachment->Invalidate();
							//colorAttachment->CreatePerSpecificLayerImageViews(m_Specification.ExistingImageLayers);
						}
						else if (attachmentIndex == 0)
						{
							//colorAttachment->CreatePerSpecificLayerImageViews(m_Specification.ExistingImageLayers);
						}
					}
				}

				HL_ASSERT(m_ColorAttachmentFormats[attachmentIndex] != TextureFormat::None);
				switch (m_ColorAttachmentFormats[attachmentIndex])
				{
					case TextureFormat::SRGB:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_SRGB8, GL_SRGB, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RGB:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_RGB8, GL_RGB, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RGBA8:
					case TextureFormat::RGBA:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RG16F:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_RG16F, GL_RG, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RGBA16:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_RGBA16, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RGBA16F:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_RGBA16F, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RG32F:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_RG32F, GL_RG, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RGBA32:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_RGBA32I, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RGBA32F:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_RGBA32F, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RED32F:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_R32F, GL_RED, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;

					case TextureFormat::RED_INTEGER:
						utils::AttachColorAttachment(colorAttachment->GetRendererID(), m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, int32(attachmentIndex));
						break;
				}
			}

			++attachmentIndex;
		}

#if 0
		if (m_ColorAttachmentFormats.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentFormats.size());

			// Create color attachments
			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				m_ColorAttachments[i] = utils::CreateTexture(m_Specification.Samples, m_ColorAttachmentFormats[i], m_Specification.Width, m_Specification.Height, int32(i));
				switch (m_ColorAttachmentFormats[i])
				{
				case TextureFormat::SRGB:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_SRGB8, GL_SRGB, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RGB:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RGB8, GL_RGB, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RGBA8:
				case TextureFormat::RGBA:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RG16F:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RG16F, GL_RG, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RGBA16:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RGBA16, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RGBA16F:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RGBA16F, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RG32F:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RG32F, GL_RG, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RGBA32:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RGBA32I, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RGBA32F:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_RGBA32F, GL_RGBA, m_Specification.Width, m_Specification.Height, int32(i));
					break;

				case TextureFormat::RED32F:
					utils::AttachColorAttachment(m_ColorAttachments[i]->GetRendererID(), m_Specification.Samples, GL_R32F, GL_RED, m_Specification.Width, m_Specification.Height, int32(i));
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

			case TextureFormat::DEPTH32F:
				utils::AttachDepthAttachment(m_DepthAttachment->GetRendererID(), m_Specification.Samples, GL_DEPTH_COMPONENT32F, GL_DEPTH_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}

#endif

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

	void OpenGLFramebuffer::Release()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		m_RendererID = 0;
	}

	void OpenGLFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Specification.Width == width && m_Specification.Height == height) && m_Specification.NoResize)
			return;

		m_Specification.Width = width * (uint32)m_Specification.Scale;
		m_Specification.Height = height * (uint32)m_Specification.Scale;

		if (!m_Specification.SwapChainTarget)
		{
			Invalidate();
		}
		else
		{
			// we do not need to re-create the framebuffer
		}

		for (auto &callback : m_ResizeCallbacks)
			callback(this);
	}

	void OpenGLFramebuffer::AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func)
	{
		m_ResizeCallbacks.push_back(func);
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

