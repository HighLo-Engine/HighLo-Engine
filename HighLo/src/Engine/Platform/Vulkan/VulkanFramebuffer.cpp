// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanFramebuffer.h"

#include "Engine/Application/Application.h"
#include "Engine/Utils/ImageUtils.h"

#include "VulkanSwapChain.h"
#include "VulkanContext.h"
#include "VulkanTexture.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification &spec)
		: m_Specification(spec)
	{
		if (spec.Width == 0)
		{
			m_Width = HLApplication::Get().GetWindow().GetWidth();
			m_Height = HLApplication::Get().GetWindow().GetHeight();
		}
		else
		{
			m_Width = spec.Width;
			m_Height = spec.Height;
		}

		// Create all image objects immediately so we can start referencing them
		// elsewhere
		uint32 attachmentIndex = 0;
		if (!m_Specification.ExistingFramebuffer)
		{
			for (auto &attachmentSpec : m_Specification.Attachments.Attachments)
			{
				if (m_Specification.ExistingImage && m_Specification.ExistingImage->GetSpecification().Deinterleaved)
				{
					HL_ASSERT(!utils::IsDepthFormat(attachmentSpec.Format), "Only supported for color attachments");
					m_AttachmentTextures.emplace_back(m_Specification.ExistingImage);
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					if (!utils::IsDepthFormat(attachmentSpec.Format))
						m_AttachmentTextures.emplace_back(); // This will be set later
				}
				else if (utils::IsDepthFormat(attachmentSpec.Format))
				{
					Ref<Texture2D> tex = Texture2D::Create(attachmentSpec.Format, m_Width * m_Specification.Scale, m_Height * m_Specification.Scale);
					tex->GetSpecification().Usage = TextureUsage::Attachment;
					tex->GetSpecification().DebugName = fmt::format("{0}-DepthAttachment{1}", m_Specification.DebugName.IsEmpty() ? "Unnamed FB" : m_Specification.DebugName, attachmentIndex);
					m_DepthTexture = tex;
				}
				else
				{
					Ref<Texture2D> tex = Texture2D::Create(attachmentSpec.Format, m_Width * m_Specification.Scale, m_Height * m_Specification.Height);
					tex->GetSpecification().Usage = TextureUsage::Attachment;
					tex->GetSpecification().DebugName = fmt::format("{0}-ColorAttachment{1}", m_Specification.DebugName.IsEmpty() ? "Unnamed FB" : m_Specification.DebugName, attachmentIndex);
					m_AttachmentTextures.emplace_back(tex);
				}
				attachmentIndex++;
			}
		}

		HL_ASSERT(spec.Attachments.Attachments.size());
		Resize(m_Width, m_Height, true);

	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
	}
	
	void VulkanFramebuffer::Bind() const
	{
	}
	
	void VulkanFramebuffer::Unbind() const
	{
	}
	
	void VulkanFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Width == width && m_Height == height))
			return;

		m_Width = width * m_Specification.Scale;
		m_Height = height * m_Specification.Scale;
		if (!m_Specification.SwapChainTarget)
		{
			Invalidate();
		}
		else
		{
			Ref<VulkanSwapChain> &swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();
			m_RenderPass = swapChain->GetNativeRenderPass();

			m_ClearValues.clear();
			m_ClearValues.emplace_back().color = { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		for (auto &callback : m_ResizeCallbacks)
			callback(this);

	}
	
	void VulkanFramebuffer::AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func)
	{
		m_ResizeCallbacks.push_back(func);
	}
	
	void VulkanFramebuffer::BindTexture(uint32 attachmentIndex, uint32 slot) const
	{
	}
	
	void VulkanFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
	}
	
	int32 VulkanFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		return 0;
	}
	
	Ref<Texture> VulkanFramebuffer::GetImage(uint32 attachmentIndex) const
	{
		HL_ASSERT(attachmentIndex < m_AttachmentTextures.size());
		return m_AttachmentTextures[attachmentIndex];
	}
	
	Ref<Texture> VulkanFramebuffer::GetDepthImage() const
	{
		return m_DepthTexture;
	}
	
	void VulkanFramebuffer::Invalidate()
	{
		auto device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		if (m_Framebuffer)
		{
			const auto device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
			vkDestroyFramebuffer(device, m_Framebuffer, nullptr);

			// Don't free the images if we don't own them
			if (!m_Specification.ExistingFramebuffer)
			{
				uint32 attachmentIndex = 0;
				for (Ref<Texture2D> image : m_AttachmentTextures)
				{
					if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
						continue;

					// Only destroy deinterleaved image once and prevent clearing layer views on second framebuffer invalidation
					if (!image->GetSpecification().Deinterleaved || attachmentIndex == 0 && !image.As<VulkanTexture2D>()->GetLayerImageView(0))
						image->Release();

					++attachmentIndex;
				}

				if (m_DepthTexture)
				{
					// Do we own the depth image?
					if (m_Specification.ExistingImages.find((uint32)m_Specification.Attachments.Attachments.size() - 1) == m_Specification.ExistingImages.end())
						m_DepthTexture->Release();
				}

			}
		}

		VulkanAllocator allocator("Framebuffer");

		std::vector<VkAttachmentDescription> attachmentDescriptions;
		std::vector<VkAttachmentReference> colorAttachmentReferences;
		VkAttachmentReference depthAttachmentReference;
		m_ClearValues.resize(m_Specification.Attachments.Attachments.size());

		bool createImages = m_AttachmentTextures.empty();

		if (m_Specification.ExistingFramebuffer)
			m_AttachmentTextures.clear();

		uint32 attachmentIndex = 0;
		for (auto attachmentSpec : m_Specification.Attachments.Attachments)
		{
			if (utils::IsDepthFormat(attachmentSpec.Format))
			{
				if (m_Specification.ExistingImage)
				{
					m_DepthTexture = m_Specification.ExistingImage;
				}
				else if (m_Specification.ExistingFramebuffer)
				{
					Ref<VulkanFramebuffer> existingFramebuffer = m_Specification.ExistingFramebuffer.As<VulkanFramebuffer>();
					m_DepthTexture = existingFramebuffer->GetDepthImage().As<Texture2D>();
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					Ref<Texture2D> existingImage = m_Specification.ExistingImages.at(attachmentIndex);
					HL_ASSERT(utils::IsDepthFormat(existingImage->GetSpecification().Format), "Trying to attach non-depth image as depth attachment");
					m_DepthTexture = existingImage;
				}
				else
				{
					Ref<VulkanTexture2D> depthAttachmentImage = m_DepthTexture.As<VulkanTexture2D>();
					auto &spec = depthAttachmentImage->GetSpecification();
					spec.Width = m_Width;
					spec.Height = m_Height;
					depthAttachmentImage->Invalidate(); // Create immediately
				}

				VkAttachmentDescription &attachmentDescription = attachmentDescriptions.emplace_back();
				attachmentDescription.flags = 0;
				attachmentDescription.format = utils::VulkanImageFormat(attachmentSpec.Format);
				attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDescription.loadOp = m_Specification.ClearOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // TODO: if sampling, needs to be store (otherwise DONT_CARE is fine)
				attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDescription.initialLayout = m_Specification.ClearOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
				if (attachmentSpec.Format == TextureFormat::DEPTH24STENCIL8 || true) // Separate layouts requires a "separate layouts" flag to be enabled
				{
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // TODO: if not sampling
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL; // TODO: if sampling
					depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
				}
				else
				{
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL; // TODO: if not sampling
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL; // TODO: if sampling
					depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL };
				}
				m_ClearValues[attachmentIndex].depthStencil = { 1.0f, 0 };
			}
			else
			{
				//HZ_CORE_ASSERT(!m_Specification.ExistingImage, "Not supported for color attachments");

				Ref<VulkanTexture2D> colorAttachment;
				if (m_Specification.ExistingFramebuffer)
				{
					Ref<VulkanFramebuffer> existingFramebuffer = m_Specification.ExistingFramebuffer.As<VulkanFramebuffer>();
					Ref<Texture2D> existingImage = existingFramebuffer->GetImage(attachmentIndex);
					colorAttachment = m_AttachmentTextures.emplace_back(existingImage).As<VulkanTexture2D>();
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					Ref<Texture2D> existingImage = m_Specification.ExistingImages[attachmentIndex];
					HL_ASSERT(!utils::IsDepthFormat(existingImage->GetSpecification().Format), "Trying to attach depth image as color attachment");
					colorAttachment = existingImage.As<VulkanTexture2D>();
					m_AttachmentTextures[attachmentIndex] = existingImage;
				}
				else
				{
					if (createImages)
					{
						Ref<Texture2D> tex = Texture2D::Create(attachmentSpec.Format, m_Width, m_Height);
						tex->GetSpecification().Usage = TextureUsage::Attachment;
						colorAttachment = m_AttachmentTextures.emplace_back(tex).As<VulkanTexture2D>();
						HL_ASSERT(false);
					}
					else
					{
						Ref<Texture2D> image = m_AttachmentTextures[attachmentIndex];
						TextureSpecification &spec = image->GetSpecification();
						spec.Width = m_Width;
						spec.Height = m_Height;
						colorAttachment = image.As<VulkanTexture2D>();
						if (!colorAttachment->GetSpecification().Deinterleaved)
							colorAttachment->Invalidate(); // Create immediately
						else if (attachmentIndex == 0 && m_Specification.ExistingImageLayers[0] == 0)// Only invalidate the first layer from only the first framebuffer
						{
							colorAttachment->Invalidate(); // Create immediately
							colorAttachment->CreatePerSpecificLayerImageViews(m_Specification.ExistingImageLayers);
						}
						else if (attachmentIndex == 0)
						{
							colorAttachment->CreatePerSpecificLayerImageViews(m_Specification.ExistingImageLayers);
						}
					}

				}

				VkAttachmentDescription &attachmentDescription = attachmentDescriptions.emplace_back();
				attachmentDescription.flags = 0;
				attachmentDescription.format = utils::VulkanImageFormat(attachmentSpec.Format);
				attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDescription.loadOp = m_Specification.ClearOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // TODO: if sampling, needs to be store (otherwise DONT_CARE is fine)
				attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDescription.initialLayout = m_Specification.ClearOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				const auto &clearColor = m_Specification.ClearColor;
				m_ClearValues[attachmentIndex].color = { {clearColor.r, clearColor.g, clearColor.b, clearColor.a} };
				colorAttachmentReferences.emplace_back(VkAttachmentReference{ attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
			}

			attachmentIndex++;
		}

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = uint32(colorAttachmentReferences.size());
		subpassDescription.pColorAttachments = colorAttachmentReferences.data();
		if (m_DepthTexture)
			subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

		// TODO: do we need these?
		// Use subpass dependencies for layout transitions
		std::vector<VkSubpassDependency> dependencies;

		if (m_AttachmentTextures.size())
		{
			{
				VkSubpassDependency &depedency = dependencies.emplace_back();
				depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
				depedency.dstSubpass = 0;
				depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				depedency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				depedency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
			{
				VkSubpassDependency &depedency = dependencies.emplace_back();
				depedency.srcSubpass = 0;
				depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
				depedency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				depedency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
		}

		if (m_DepthTexture)
		{
			{
				VkSubpassDependency &depedency = dependencies.emplace_back();
				depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
				depedency.dstSubpass = 0;
				depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				depedency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				depedency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}

			{
				VkSubpassDependency &depedency = dependencies.emplace_back();
				depedency.srcSubpass = 0;
				depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
				depedency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
				depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				depedency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
		}

		// Create the actual renderpass
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32>(attachmentDescriptions.size());
		renderPassInfo.pAttachments = attachmentDescriptions.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_RenderPass));

		std::vector<VkImageView> attachments(m_AttachmentTextures.size());
		for (uint32 i = 0; i < m_AttachmentTextures.size(); i++)
		{
			Ref<VulkanTexture2D> image = m_AttachmentTextures[i].As<VulkanTexture2D>();
			if (image->GetSpecification().Deinterleaved)
			{
				attachments[i] = image->GetLayerImageView(m_Specification.ExistingImageLayers[i]);
				HL_ASSERT(attachments[i]);
			}
			else
			{
				attachments[i] = image->GetTextureInfo().ImageView;
				HL_ASSERT(attachments[i]);
			}
		}

		if (m_DepthTexture)
		{
			Ref<VulkanTexture2D> image = m_DepthTexture.As<VulkanTexture2D>();
			if (m_Specification.ExistingImage)
			{
				HL_ASSERT(m_Specification.ExistingImageLayers.size() == 1, "Depth attachments do not support deinterleaving");
				attachments.emplace_back(image->GetLayerImageView(m_Specification.ExistingImageLayers[0]));
				HL_ASSERT(attachments.back());
			}
			else
			{
				attachments.emplace_back(image->GetTextureInfo().ImageView);
				HL_ASSERT(attachments.back());
			}
		}

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = m_RenderPass;
		framebufferCreateInfo.attachmentCount = uint32(attachments.size());
		framebufferCreateInfo.pAttachments = attachments.data();
		framebufferCreateInfo.width = m_Width;
		framebufferCreateInfo.height = m_Height;
		framebufferCreateInfo.layers = 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &m_Framebuffer));
	}
}

#endif // HIGHLO_API_VULKAN

