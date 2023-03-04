// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanFramebuffer.h"

#include "Engine/Application/Application.h"
#include "Engine/Utils/ImageUtils.h"
#include "VulkanSwapChain.h"
#include "VulkanContext.h"
#include "VulkanAllocator.h"
#include "VulkanTexture2D.h"
#include "VulkanUtils.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification &specification)
		: m_Specification(specification)
	{
		if (specification.Width == 0)
		{
			m_Specification.Width = HLApplication::Get().GetWindow().GetWidth();
			m_Specification.Height = HLApplication::Get().GetWindow().GetHeight();
		}
		else
		{
			m_Specification.Width = (uint32)(m_Specification.Width * m_Specification.Scale);
			m_Specification.Height = (uint32)(m_Specification.Height * m_Specification.Scale);
		}

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
						m_DepthAttachment = m_Specification.ExistingImage;
					else
						m_ColorAttachments.emplace_back(m_Specification.ExistingImage);
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					if (!utils::IsDepthFormat(attachmentSpec.Format))
						m_ColorAttachments.emplace_back(); // This will be set later
				}
				else if (utils::IsDepthFormat(attachmentSpec.Format))
				{
					TextureSpecification spec;
					spec.Format = attachmentSpec.Format;
					spec.Usage = TextureUsage::Attachment;
					spec.Transfer = m_Specification.Transfer;
					spec.Width = (uint32)m_Specification.Width;
					spec.Height = (uint32)m_Specification.Height;
					spec.DebugName = fmt::format("{0}-DepthAttachment{1}", m_Specification.DebugName.IsEmpty() ? "Unnamed FB" : *m_Specification.DebugName, attachmentIndex);
					m_DepthAttachment = Texture2D::CreateFromSpecification(spec);
				}
				else
				{
					TextureSpecification spec;
					spec.Format = attachmentSpec.Format;
					spec.Usage = TextureUsage::Attachment;
					spec.Transfer = m_Specification.Transfer;
					spec.Width = (uint32)m_Specification.Width;
					spec.Height = (uint32)m_Specification.Height;
					spec.DebugName = fmt::format("{0}-ColorAttachment{1}", m_Specification.DebugName.IsEmpty() ? "Unnamed FB" : *m_Specification.DebugName, attachmentIndex);
					m_ColorAttachments.emplace_back(Texture2D::CreateFromSpecification(spec));
				}
				attachmentIndex++;
			}
		}

		HL_ASSERT(specification.Attachments.Attachments.size());
		Resize(m_Specification.Width, m_Specification.Height, true);
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		Release();
	}
	
	void VulkanFramebuffer::Invalidate()
	{
		auto device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

		Release();

		VulkanAllocator allocator("Framebuffer");

		std::vector<VkAttachmentDescription> attachmentDescriptions;

		std::vector<VkAttachmentReference> colorAttachmentReferences;
		VkAttachmentReference depthAttachmentReference;

		m_ClearValues.resize(m_Specification.Attachments.Attachments.size());

		bool createImages = m_ColorAttachments.empty();

		if (m_Specification.ExistingFramebuffer)
			m_ColorAttachments.clear();

		uint32 attachmentIndex = 0;
		for (auto attachmentSpec : m_Specification.Attachments.Attachments)
		{
			if (utils::IsDepthFormat(attachmentSpec.Format))
			{
				if (m_Specification.ExistingImage)
				{
					m_DepthAttachment = m_Specification.ExistingImage;
				}
				else if (m_Specification.ExistingFramebuffer)
				{
					Ref<VulkanFramebuffer> existingFramebuffer = m_Specification.ExistingFramebuffer.As<VulkanFramebuffer>();
					m_DepthAttachment = existingFramebuffer->GetDepthImage();
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					Ref<Texture2D> existingImage = m_Specification.ExistingImages.at(attachmentIndex);
					HL_ASSERT(utils::IsDepthFormat(existingImage->GetSpecification().Format), "Trying to attach non-depth image as depth attachment");
					m_DepthAttachment = existingImage;
				}
				else
				{
					auto &spec = m_DepthAttachment->GetSpecification();
					spec.Width = (uint32)(m_Specification.Width * m_Specification.Scale);
					spec.Height = (uint32)(m_Specification.Height * m_Specification.Scale);
					m_DepthAttachment->Invalidate(); // Create immediately
				}

				VkAttachmentDescription &attachmentDescription = attachmentDescriptions.emplace_back();
				attachmentDescription.flags = 0;
				attachmentDescription.format = utils::VulkanTextureFormat(attachmentSpec.Format);
				attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDescription.loadOp = m_Specification.ClearDepthOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // TODO: if sampling, needs to be store (otherwise DONT_CARE is fine)
				attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDescription.initialLayout = m_Specification.ClearDepthOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
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
				m_ClearValues[attachmentIndex].depthStencil = { m_Specification.DepthClearValue, 0 };
			}
			else
			{
				Ref<Texture2D> colorAttachment;
				if (m_Specification.ExistingFramebuffer)
				{
					Ref<VulkanFramebuffer> existingFramebuffer = m_Specification.ExistingFramebuffer.As<VulkanFramebuffer>();
					Ref<Texture2D> existingImage = existingFramebuffer->GetImage(attachmentIndex);
					colorAttachment = m_ColorAttachments.emplace_back(existingImage).As<Texture2D>();
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
				{
					Ref<Texture2D> existingImage = m_Specification.ExistingImages[attachmentIndex];
					HL_ASSERT(!utils::IsDepthFormat(existingImage->GetSpecification().Format), "Trying to attach depth image as color attachment");
					colorAttachment = existingImage;
					m_ColorAttachments[attachmentIndex] = existingImage;
				}
				else
				{
					if (createImages)
					{
						TextureSpecification spec;
						spec.Format = attachmentSpec.Format;
						spec.Usage = TextureUsage::Attachment;
						spec.Transfer = m_Specification.Transfer;
						spec.Width = (uint32)(m_Specification.Width * m_Specification.Scale);
						spec.Height = (uint32)(m_Specification.Height * m_Specification.Scale);
						colorAttachment = m_ColorAttachments.emplace_back(Texture2D::CreateFromSpecification(spec)).As<Texture2D>();
					}
					else
					{
						Ref<Texture2D> image = m_ColorAttachments[attachmentIndex];
						TextureSpecification &spec = image->GetSpecification();
						spec.Width = (uint32)(m_Specification.Width * m_Specification.Scale);
						spec.Height = (uint32)(m_Specification.Height * m_Specification.Scale);
						colorAttachment = image;

						if (colorAttachment->GetSpecification().Layers == 1)
						{
							colorAttachment->Invalidate(); // Create immediately
						}
						// Only invalidate the first layer from only the first framebuffer
						else if (attachmentIndex == 0 && m_Specification.ExistingImageLayers[0] == 0)
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
				attachmentDescription.format = utils::VulkanTextureFormat(attachmentSpec.Format);
				attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDescription.loadOp = m_Specification.ClearColorOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // TODO: if sampling, needs to be store (otherwise DONT_CARE is fine)
				attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDescription.initialLayout = m_Specification.ClearColorOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				const auto &clearColor = m_Specification.ClearColor;
				m_ClearValues[attachmentIndex].color = { {clearColor.r, clearColor.g, clearColor.b, clearColor.a } };
				colorAttachmentReferences.emplace_back(VkAttachmentReference{ attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
			}

			attachmentIndex++;
		}

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = uint32(colorAttachmentReferences.size());
		subpassDescription.pColorAttachments = colorAttachmentReferences.data();
		if (m_DepthAttachment)
			subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

		// Use subpass dependencies for layout transitions
		std::vector<VkSubpassDependency> dependencies;

		if (m_ColorAttachments.size())
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

		if (m_DepthAttachment)
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
		utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_RENDER_PASS, m_Specification.DebugName, m_RenderPass);

		std::vector<VkImageView> attachments(m_ColorAttachments.size());
		for (uint32 i = 0; i < m_ColorAttachments.size(); i++)
		{
			Ref<VulkanTexture2D> image = m_ColorAttachments[i].As<VulkanTexture2D>();
			if (image->GetSpecification().Layers > 1)
			{
				attachments[i] = image->GetLayerImageView(m_Specification.ExistingImageLayers[i]);
			}
			else
			{
				attachments[i] = image->GetTextureInfo().ImageView;
			}

			HL_ASSERT(attachments[i]);
		}

		if (m_DepthAttachment)
		{
			Ref<VulkanTexture2D> image = m_DepthAttachment.As<VulkanTexture2D>();
			if (m_Specification.ExistingImage)
			{
				HL_ASSERT(m_Specification.ExistingImageLayers.size() == 1, "Depth attachments do not support deinterleaving");
				attachments.emplace_back(image->GetLayerImageView(m_Specification.ExistingImageLayers[0]));
			}
			else
			{
				attachments.emplace_back(image->GetTextureInfo().ImageView);
			}

			HL_ASSERT(attachments.back());
		}

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = m_RenderPass;
		framebufferCreateInfo.attachmentCount = uint32(attachments.size());
		framebufferCreateInfo.pAttachments = attachments.data();
		framebufferCreateInfo.width = m_Specification.Width;
		framebufferCreateInfo.height = m_Specification.Height;
		framebufferCreateInfo.layers = 1;

		VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &m_Framebuffer));
		utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FRAMEBUFFER, m_Specification.DebugName, m_Framebuffer);
	}
	
	void VulkanFramebuffer::Release()
	{
		if (m_Framebuffer)
		{
			VkFramebuffer framebuffer = m_Framebuffer;
			Renderer::SubmitWithoutResources([framebuffer]()
			{
				const auto device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
				vkDestroyFramebuffer(device, framebuffer, nullptr);
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
	}
	
	void VulkanFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
	{
		if ((!forceRecreate && (m_Specification.Width == width && m_Specification.Height == height)) || m_Specification.NoResize)
			return;

		m_Specification.Width = width * (uint32)m_Specification.Scale;
		m_Specification.Height = height * (uint32)m_Specification.Scale;

		if (!m_Specification.SwapChainTarget)
		{
			Invalidate();
		}
		else
		{
			Ref<VulkanSwapChain> &swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();
			m_RenderPass = swapChain->GetRenderPass();

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
	
	void VulkanFramebuffer::ClearAttachment(uint32 attachmentIndex, int32 value)
	{
		// TODO
	}
	
	int32 VulkanFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
	{
		// TODO
		return 0;
	}
}

#endif // HIGHLO_API_VULKAN

