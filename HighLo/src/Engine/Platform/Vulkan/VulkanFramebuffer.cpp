// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanFramebuffer.h"

#ifdef HIGHLO_API_VULKAN

#include "Engine/Utils/ImageUtils.h"

#include "VulkanUtils.h"
#include "VulkanContext.h"
#include "VulkanTexture2D.h"

namespace highlo
{
    VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification &spec)
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
            m_Specification.Height = m_Specification.Height * (uint32)m_Specification.Scale;
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
                        m_DepthImage = m_Specification.ExistingImage;
                    }
                    else
                    {
                        m_AttachmentImages.emplace_back(m_Specification.ExistingImage);
                    }
                }
                else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
                {
                    if (!utils::IsDepthFormat(attachmentSpec.Format))
                        m_AttachmentImages.emplace_back(); // This will be set later
                }
                else if (utils::IsDepthFormat(attachmentSpec.Format))
                {
                    TextureSpecification spec;
                    spec.Format = attachmentSpec.Format;
                    spec.Usage = TextureUsage::Attachment;
                    spec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
                    spec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
                    spec.DebugName = fmt::format("{0}-DepthAttachment{1}", m_Specification.DebugName.IsEmpty() ? "unnamed FB" : *m_Specification.DebugName, attachmentIndex);
                    m_DepthImage = Texture2D::CreateFromSpecification(spec);
                }
                else
                {
                    TextureSpecification spec;
                    spec.Format = attachmentSpec.Format;
                    spec.Usage = TextureUsage::Attachment;
                    spec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
                    spec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
                    spec.DebugName = fmt::format("{0}-ColorAttachment{1}", m_Specification.DebugName.IsEmpty() ? "unnamed FB" : *m_Specification.DebugName, attachmentIndex);
                    m_AttachmentImages.emplace_back(Texture2D::CreateFromSpecification(spec));
                }

                ++attachmentIndex;
            }
        }

        HL_ASSERT(m_Specification.Attachments.Attachments.size());
        Resize(m_Specification.Width, m_Specification.Height, true);
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
        Release();
    }
    
    void VulkanFramebuffer::Resize(uint32 width, uint32 height, bool forceRecreate)
    {
        if (!forceRecreate && (m_Specification.Width == width && m_Specification.Height == height))
            return;

        m_Specification.Width = width * (uint32)m_Specification.Scale;
        m_Specification.Height = height * (uint32)m_Specification.Scale;

        if (!m_Specification.SwapChainTarget)
        {
            Invalidate();
        }
        else
        {
            Ref<VulkanSwapChain> swapchain = HLApplication::Get().GetWindow().GetSwapChain();
            HL_ASSERT(swapchain);

            m_RenderPass = swapchain->GetRenderPass();

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
        HL_ASSERT(attachmentIndex < m_AttachmentImages.size());
        Ref<Texture2D> texture = m_AttachmentImages[attachmentIndex];

        
    }
    
    int32 VulkanFramebuffer::ReadPixel(uint32 attachmentIndex, int32 x, int32 y)
    {
        HL_ASSERT(attachmentIndex < m_AttachmentImages.size());
        return 0;
    }
    
    Ref<Texture> VulkanFramebuffer::GetImage(uint32 attachmentIndex) const
    {
        HL_ASSERT(attachmentIndex < m_AttachmentImages.size());
        return m_AttachmentImages[attachmentIndex].As<Texture>();
    }
    
    Ref<Texture> VulkanFramebuffer::GetDepthImage() const
    {
        return m_DepthImage.As<Texture>();
    }

    void VulkanFramebuffer::Invalidate()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        std::vector<VkAttachmentDescription> attachmentDescriptions;
        std::vector<VkAttachmentReference> colorAttachmentReferences;
        VkAttachmentReference depthAttachmentReference;
        bool createImages = m_AttachmentImages.empty();

        if (m_Framebuffer)
        {
            vkDestroyFramebuffer(device, m_Framebuffer, nullptr);

            if (!m_Specification.ExistingFramebuffer)
            {
                uint32 attachmentIndex = 0;
                for (Ref<Texture2D> &texture : m_AttachmentImages)
                {
                    if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
                        continue;

                    if (texture->GetSpecification().Layers == 1 || attachmentIndex == 0 && !texture.As<VulkanTexture2D>()->GetLayerImageView(0))
                        texture->Release();

                    ++attachmentIndex;
                }

                if (m_DepthImage)
                {
                    if (m_Specification.ExistingImages.find((uint32)m_Specification.Attachments.Attachments.size() - 1) == m_Specification.ExistingImages.end())
                        m_DepthImage->Release();
                }
            }
        }

        if (m_Specification.ExistingFramebuffer)
            m_AttachmentImages.clear();

        m_ClearValues.resize(m_Specification.Attachments.Attachments.size());

        uint32 attachmentIndex = 0;
        for (auto &attachmentSpec : m_Specification.Attachments.Attachments)
        {
            if (utils::IsDepthFormat(attachmentSpec.Format))
            {
                if (m_Specification.ExistingImage)
                {
                    m_DepthImage = m_Specification.ExistingImage;
                }
                else if (m_Specification.ExistingFramebuffer)
                {
                    Ref<VulkanFramebuffer> existingFramebuffer = m_Specification.ExistingFramebuffer.As<VulkanFramebuffer>();
                    m_DepthImage = existingFramebuffer->GetDepthImage().As<Texture2D>();
                }
                else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
                {
                    Ref<Texture2D> existingTexture = m_Specification.ExistingImages.at(attachmentIndex);
                    HL_ASSERT(utils::IsDepthFormat(existingTexture->GetSpecification().Format));
                    m_DepthImage = existingTexture;
                }
                else
                {
                    Ref<VulkanTexture2D> depthAttachmentTexture = m_DepthImage.As<VulkanTexture2D>();
                    auto &spec = depthAttachmentTexture->GetSpecification();
                    spec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
                    spec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
                    depthAttachmentTexture->Invalidate();
                }

                VkAttachmentDescription &attachmentDescription = attachmentDescriptions.emplace_back();
                attachmentDescription.flags = 0;
                attachmentDescription.format = utils::VulkanTextureFormat(attachmentSpec.Format);
                attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                attachmentDescription.loadOp = m_Specification.ClearDepthOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
                attachmentDescription.storeOp = m_Specification.ShouldSample ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
                attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                attachmentDescription.initialLayout = m_Specification.ClearDepthOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
                
                if (attachmentSpec.Format == TextureFormat::DEPTH24STENCIL8)
                {
                    if (m_Specification.ShouldSample)
                        attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
                    else
                        attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                    depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
                }
                else
                {
                    if (m_Specification.ShouldSample)
                        attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
                    else
                        attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;

                    depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL };
                }

                m_ClearValues[attachmentIndex].depthStencil = { m_Specification.DepthClearValue, 0 };
            }
            else
            {
                Ref<VulkanTexture2D> colorAttachment;
                if (m_Specification.ExistingFramebuffer)
                {
                    Ref<VulkanFramebuffer> existingFramebuffer = m_Specification.ExistingFramebuffer.As<VulkanFramebuffer>();
                    Ref<Texture2D> existingTexture = existingFramebuffer->GetImage(attachmentIndex);
                    colorAttachment = m_AttachmentImages.emplace_back(existingTexture).As<VulkanTexture2D>();
                }
                else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
                {
                    Ref<Texture2D> existingTexture = m_Specification.ExistingImages.at(attachmentIndex);
                    HL_ASSERT(!utils::IsDepthFormat(existingTexture->GetSpecification().Format));
                    colorAttachment = existingTexture.As<VulkanTexture2D>();
                    m_AttachmentImages[attachmentIndex] = existingTexture;
                }
                else
                {
                    if (createImages)
                    {
                        TextureSpecification spec;
                        spec.Format = attachmentSpec.Format;
                        spec.Usage = TextureUsage::Attachment;
                        spec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
                        spec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
                        colorAttachment = m_AttachmentImages.emplace_back(Texture2D::CreateFromSpecification(spec)).As<VulkanTexture2D>();
                        HL_ASSERT(false);
                    }
                    else
                    {
                        Ref<Texture2D> texture = m_AttachmentImages[attachmentIndex];
                        TextureSpecification &spec = texture->GetSpecification();
                        spec.Width = m_Specification.Width * (uint32)m_Specification.Scale;
                        spec.Height = m_Specification.Height * (uint32)m_Specification.Scale;
                        colorAttachment = texture.As<VulkanTexture2D>();
                        if (colorAttachment->GetSpecification().Layers == 1)
                        {
                            colorAttachment->Invalidate();
                        }
                        else if (attachmentIndex == 0 && m_Specification.ExistingImageLayers[0] == 0)
                        {
                            colorAttachment->Invalidate();
                            colorAttachment->CreatePerSpecificLayerImageViews(m_Specification.ExistingImageLayers);
                        }
                        else if (attachmentIndex == 0)
                        {
                            colorAttachment->CreatePerSpecificLayerImageViews(m_Specification.ExistingImageLayers);
                        }
                    }

                    VkAttachmentDescription &attachmentDescription = attachmentDescriptions.emplace_back();
                    attachmentDescription.flags = 0;
                    attachmentDescription.format = utils::VulkanTextureFormat(attachmentSpec.Format);
                    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                    attachmentDescription.loadOp = m_Specification.ClearColorOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
                    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                    attachmentDescription.initialLayout = m_Specification.ClearColorOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

                    const auto &clearColor = m_Specification.ClearColor;
                    m_ClearValues[attachmentIndex].color = { { clearColor.r, clearColor.g, clearColor.b, clearColor.a } };
                    colorAttachmentReferences.emplace_back(VkAttachmentReference{ attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
                }
            }

            ++attachmentIndex;
        }

        VkSubpassDescription subpassDescription = {};
        subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.colorAttachmentCount = (uint32)colorAttachmentReferences.size();
        subpassDescription.pColorAttachments = colorAttachmentReferences.data();
        if (m_DepthImage)
            subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

        std::vector<VkSubpassDependency> dependencies;
        
        if (m_AttachmentImages.size())
        {
            VkSubpassDependency &fragmentDependency = dependencies.emplace_back();
            fragmentDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            fragmentDependency.dstSubpass = 0;
            fragmentDependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            fragmentDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            fragmentDependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            fragmentDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            fragmentDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

            VkSubpassDependency &colorDependency = dependencies.emplace_back();
            colorDependency.srcSubpass = 0;
            colorDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
            colorDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            colorDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            colorDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            colorDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            colorDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        }

        if (m_DepthImage)
        {
            VkSubpassDependency &fragmentDependency = dependencies.emplace_back();
            fragmentDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            fragmentDependency.dstSubpass = 0;
            fragmentDependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            fragmentDependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            fragmentDependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            fragmentDependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            fragmentDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

            VkSubpassDependency &colorDependency = dependencies.emplace_back();
            colorDependency.srcSubpass = 0;
            colorDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
            colorDependency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            colorDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            colorDependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            colorDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            colorDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        }

        // Create the actual render pass
        VkRenderPassCreateInfo renderPassCreate = {};
        renderPassCreate.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassCreate.pNext = nullptr;
        renderPassCreate.attachmentCount = (uint32)attachmentDescriptions.size();
        renderPassCreate.pAttachments = attachmentDescriptions.data();
        renderPassCreate.subpassCount = 1;
        renderPassCreate.pSubpasses = &subpassDescription;
        renderPassCreate.dependencyCount = (uint32)dependencies.size();
        renderPassCreate.pDependencies = dependencies.data();
        VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassCreate, nullptr, &m_RenderPass));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_RENDER_PASS, m_Specification.DebugName, m_RenderPass);

        std::vector<VkImageView> attachments(m_AttachmentImages.size());
        for (uint32 i = 0; i < m_AttachmentImages.size(); ++i)
        {
            Ref<VulkanTexture2D> texture = m_AttachmentImages[i].As<VulkanTexture2D>();
            if (texture->GetSpecification().Layers > 1)
            {
                attachments[i] = texture->GetLayerImageView(m_Specification.ExistingImageLayers[i]);
            }
            else
            {
                attachments[i] = texture->GetTextureInfo().ImageView;
            }

            HL_ASSERT(attachments[i]);
        }

        if (m_DepthImage)
        {
            Ref<VulkanTexture2D> texture = m_DepthImage.As<VulkanTexture2D>();
            if (m_Specification.ExistingImage)
            {
                HL_ASSERT(m_Specification.ExistingImageLayers.size() == 1, "Depth attachments do not support deinterleaving!");
                attachments.emplace_back(texture->GetLayerImageView(m_Specification.ExistingImageLayers[0]));
            }
            else
            {
                attachments.emplace_back(texture->GetTextureInfo().ImageView);
            }

            HL_ASSERT(attachments.back());
        }

        // Create the actual frame buffer
        VkFramebufferCreateInfo framebufferCreate = {};
        framebufferCreate.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreate.pNext = nullptr;
        framebufferCreate.renderPass = m_RenderPass;
        framebufferCreate.attachmentCount = (uint32)attachments.size();
        framebufferCreate.pAttachments = attachments.data();
        framebufferCreate.layers = 1;
        framebufferCreate.width = m_Specification.Width;
        framebufferCreate.height = m_Specification.Height;
        VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferCreate, nullptr, &m_Framebuffer));
        utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FRAMEBUFFER, m_Specification.DebugName, m_Framebuffer);
    }
    
    void VulkanFramebuffer::Release()
    {
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        vkDestroyFramebuffer(device, m_Framebuffer, nullptr);
        vkDestroyRenderPass(device, m_RenderPass, nullptr);
    }
}

#endif // HIGHLO_API_VULKAN

