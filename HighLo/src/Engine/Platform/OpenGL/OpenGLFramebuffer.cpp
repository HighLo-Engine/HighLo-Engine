// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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

		static void AttachColorAttachment(HLRendererID id, int32 samples, GLenum internalFormat, GLenum format, uint32 width, uint32 height, int32 index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, id);
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
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, id);
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
				case TextureFormat::RGBA:        return GL_RGBA8;
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
		
		uint32 width = spec.Width;
		uint32 height = spec.Height;
		if (0 == width)
			width = HLApplication::Get().GetWindow().GetWidth();

		if (0 == height)
			height = HLApplication::Get().GetWindow().GetHeight();

		// Create all image objects immediately so we can start referencing them
		// elsewhere
		uint32 attachmentIndex = 0;
		if (!m_Specification.ExistingFramebuffer)
		{
			for (auto &attachmentSpec : m_Specification.Attachments.Attachments)
			{
				if (m_Specification.ExistingImage && m_Specification.ExistingImage->GetSpecification().Layers > 1)
				{
					if (utils::IsDepthFormat(attachmentSpec.Format))
					{
						m_DepthAttachment = m_Specification.ExistingImage;
						m_DepthAttachmentFormat = attachmentSpec.Format;
					}
					else
					{
						m_ColorAttachments.emplace_back(m_Specification.ExistingImage);
						m_ColorAttachmentFormats.emplace_back(attachmentSpec.Format);
					}
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					if (!utils::IsDepthFormat(attachmentSpec.Format))
					{
						m_ColorAttachments.emplace_back(); // This will be set later
						m_ColorAttachmentFormats.emplace_back(attachmentSpec.Format);
					}
				}
				else if (utils::IsDepthFormat(attachmentSpec.Format))
				{
					TextureSpecification spec;
					spec.Format = attachmentSpec.Format;
					spec.Usage = TextureUsage::Attachment;
					spec.Width = width * (uint32)m_Specification.Scale;
					spec.Height = height * (uint32)m_Specification.Scale;
					spec.DebugName = fmt::format("{0}-DepthAttachment{1}", m_Specification.DebugName.IsEmpty() ? "Unnamed FB" : m_Specification.DebugName, attachmentIndex);
					m_DepthAttachment = Texture2D::CreateFromSpecification(spec);
					m_DepthAttachmentFormat = attachmentSpec.Format;
				}
				else
				{
					TextureSpecification spec;
					spec.Format = attachmentSpec.Format;
					spec.Usage = TextureUsage::Attachment;
					spec.Width = width * (uint32)m_Specification.Scale;
					spec.Height = height * (uint32)m_Specification.Scale;
					spec.DebugName = fmt::format("{0}-ColorAttachment{1}", m_Specification.DebugName.IsEmpty() ? "Unnamed FB" : m_Specification.DebugName, attachmentIndex);
					m_ColorAttachments.emplace_back(Texture2D::CreateFromSpecification(spec));
					m_ColorAttachmentFormats.emplace_back(attachmentSpec.Format);
				}
				attachmentIndex++;
			}
		}

		Resize(width, height, true);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		Release();
	}

	void OpenGLFramebuffer::Invalidate()
	{
		Release();

		Ref<OpenGLFramebuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glGenFramebuffers(1, &instance->m_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, instance->m_RendererID);

			bool multisampled = instance->m_Specification.Samples > 1;
			bool createImages = instance->m_ColorAttachments.empty();

			if (instance->m_Specification.ExistingFramebuffer)
				instance->m_ColorAttachments.clear();

			uint32 attachmentIndex = 0;
			for (auto attachmentSpec : instance->m_Specification.Attachments.Attachments)
			{
				if (utils::IsDepthFormat(attachmentSpec.Format))
				{
					if (instance->m_Specification.ExistingImage)
					{
						instance->m_DepthAttachment = instance->m_Specification.ExistingImage;
					}
					else if (instance->m_Specification.ExistingFramebuffer)
					{
						Ref<Framebuffer> existingFramebuffer = instance->m_Specification.ExistingFramebuffer;
						instance->m_DepthAttachment = existingFramebuffer->GetDepthImage();
					}
					else if (instance->m_Specification.ExistingImages.find(attachmentIndex) != instance->m_Specification.ExistingImages.end())
					{
						Ref<Texture2D> existingImage = instance->m_Specification.ExistingImages.at(attachmentIndex);
						HL_ASSERT(utils::IsDepthFormat(existingImage->GetSpecification().Format), "Trying to attach non-depth image as depth attachment");
						instance->m_DepthAttachment = existingImage;
					}
					else
					{
						if (!instance->m_DepthAttachment)
						{
							TextureSpecification spec;
							spec.Format = attachmentSpec.Format;
							spec.Usage = TextureUsage::Attachment;
							spec.Width = instance->m_Specification.Width * (uint32)instance->m_Specification.Scale;
							spec.Height = instance->m_Specification.Height * (uint32)instance->m_Specification.Scale;
							spec.DebugName = fmt::format("{0}-DepthAttachment{1}", instance->m_Specification.DebugName.IsEmpty() ? "Unnamed FB" : instance->m_Specification.DebugName, attachmentIndex);
							instance->m_DepthAttachment = Texture2D::CreateFromSpecification(spec);
							instance->m_DepthAttachmentFormat = attachmentSpec.Format;
						}
						else
						{
							auto &spec = instance->m_DepthAttachment->GetSpecification();
							spec.Width = instance->m_Specification.Width * (uint32)instance->m_Specification.Scale;
							spec.Height = instance->m_Specification.Height * (uint32)instance->m_Specification.Scale;
							instance->m_DepthAttachment->RT_Invalidate(); // Create immediately
						}
					}

					glBindTexture(utils::TextureTarget(multisampled), instance->m_DepthAttachment->GetRendererID());
					switch (instance->m_DepthAttachmentFormat)
					{
						case TextureFormat::DEPTH24STENCIL8:
							utils::AttachDepthAttachment(instance->m_DepthAttachment->GetRendererID(), instance->m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, instance->m_Specification.Width, instance->m_Specification.Height);
							break;

						case TextureFormat::DEPTH32F:
							utils::AttachDepthAttachment(instance->m_DepthAttachment->GetRendererID(), instance->m_Specification.Samples, GL_DEPTH_COMPONENT32F, GL_DEPTH_ATTACHMENT, instance->m_Specification.Width, instance->m_Specification.Height);
							break;
					}
				}
				else
				{
					Ref<Texture2D> colorAttachment = nullptr;
					if (instance->m_Specification.ExistingFramebuffer)
					{
						Ref<Framebuffer> existingFramebuffer = instance->m_Specification.ExistingFramebuffer;
						Ref<Texture2D> existingImage = existingFramebuffer->GetImage(attachmentIndex);
						colorAttachment = instance->m_ColorAttachments.emplace_back(existingImage).As<Texture2D>();
					}
					else if (instance->m_Specification.ExistingImages.find(attachmentIndex) != instance->m_Specification.ExistingImages.end())
					{
						Ref<Texture2D> existingImage = instance->m_Specification.ExistingImages[attachmentIndex];
						HL_ASSERT(!utils::IsDepthFormat(existingImage->GetSpecification().Format), "Trying to attach depth image as color attachment");
						colorAttachment = existingImage;
						instance->m_ColorAttachments[attachmentIndex] = existingImage;
					}
					else
					{
						if (createImages)
						{
							TextureSpecification spec;
							spec.Format = attachmentSpec.Format;
							spec.Usage = TextureUsage::Attachment;
							spec.Width = instance->m_Specification.Width * (uint32)instance->m_Specification.Scale;
							spec.Height = instance->m_Specification.Height * (uint32)instance->m_Specification.Scale;
							colorAttachment = instance->m_ColorAttachments.emplace_back(Texture2D::CreateFromSpecification(spec)).As<Texture2D>();
						}
						else
						{
							Ref<Texture2D> image = instance->m_ColorAttachments[attachmentIndex];
							TextureSpecification &spec = image->GetSpecification();
							spec.Width = instance->m_Specification.Width * (uint32)instance->m_Specification.Scale;
							spec.Height = instance->m_Specification.Height * (uint32)instance->m_Specification.Scale;
							colorAttachment = image;

							if (colorAttachment->GetSpecification().Layers == 1)
								colorAttachment->RT_Invalidate(); // Create immediately
							else if (attachmentIndex == 0 && instance->m_Specification.ExistingImageLayers[0] == 0)// Only invalidate the first layer from only the first framebuffer
							{
								colorAttachment->RT_Invalidate(); // Create immediately
								colorAttachment->CreatePerSpecificLayerImageViews(instance->m_Specification.ExistingImageLayers);
							}
							else if (attachmentIndex == 0)
							{
								colorAttachment->CreatePerSpecificLayerImageViews(instance->m_Specification.ExistingImageLayers);
							}
						}
					}

					HL_ASSERT(colorAttachment);

					switch (instance->m_ColorAttachmentFormats[attachmentIndex])
					{
						case TextureFormat::SRGB:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_SRGB8, GL_SRGB, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RGB:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_RGB8, GL_RGB, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RGBA8:
						case TextureFormat::RGBA:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_RGBA8, GL_RGBA, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RG16F:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_RG16F, GL_RG, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RGBA16:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_RGBA16, GL_RGBA, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RGBA16F:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_RGBA16F, GL_RGBA, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RG32F:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_RG32F, GL_RG, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RGBA32:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_RGBA32I, GL_RGBA, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RGBA32F:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_RGBA32F, GL_RGBA, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RED32F:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_R32F, GL_RED, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;

						case TextureFormat::RED_INTEGER:
							utils::AttachColorAttachment(colorAttachment->GetRendererID(), instance->m_Specification.Samples, GL_R32I, GL_RED_INTEGER, instance->m_Specification.Width, instance->m_Specification.Height, attachmentIndex);
							break;
					}

				}

				++attachmentIndex;
			}

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

	void OpenGLFramebuffer::Release()
	{
		Ref<OpenGLFramebuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			glDeleteFramebuffers(1, &instance->m_RendererID);
			instance->m_RendererID = 0;
		});

		// Don't free the images if we don't own them
		if (!m_Specification.ExistingFramebuffer)
		{
			uint32 attachmentIndex = 0;
			for (Ref<Texture2D> image : m_ColorAttachments)
			{
				if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
					continue;

				// Only destroy deinterleaved image once and prevent clearing layer views on second framebuffer invalidation
				// TODO
			//	if (image->GetSpecification().Layers == 1 || attachmentIndex == 0 && !image->GetLayerImageView(0)) // TODO
			//		image->Release();
				attachmentIndex++;
			}

			if (m_DepthAttachment)
			{
				// Do we own the depth image?
				if (m_Specification.ExistingImages.find((uint32)m_Specification.Attachments.Attachments.size() - 1) == m_Specification.ExistingImages.end())
					m_DepthAttachment->Release();
			}
		}
	}

	void OpenGLFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if (!forceRecreate || (m_Specification.Width == width && m_Specification.Height == height) || m_Specification.NoResize)
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
		Renderer::Submit([instance, attachmentIndex, slot]()
		{
			glBindTextureUnit(slot, instance->m_ColorAttachments[attachmentIndex]->GetRendererID());
		});
	}

	void OpenGLFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
		HL_ASSERT(attachmentIndex < m_ColorAttachments.size());

		Ref<OpenGLFramebuffer> instance = this;
		Renderer::Submit([instance, attachmentIndex, value]()
		{
			auto &format = instance->m_ColorAttachmentFormats[attachmentIndex];
			glClearTexImage(instance->m_ColorAttachments[attachmentIndex]->GetRendererID(), 0, utils::HighLoFBTextureFormatToGL(format), GL_INT, &value);
		});
	}

	int32 OpenGLFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		HL_ASSERT(attachmentIndex < m_ColorAttachments.size());
		int32 pixelData = -1;

		Renderer::Submit([x, y, attachmentIndex, &pixelData]()
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
			glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		});

		return pixelData;
	}
}

#endif // HIGHLO_API_OPENGL

