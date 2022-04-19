#pragma once

#include "Engine/Graphics/Framebuffer.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

namespace highlo
{
	class VulkanFramebuffer : public Framebuffer
	{
	public:

		VulkanFramebuffer(const FramebufferSpecification &spec);
		virtual ~VulkanFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32 width, uint32 height, bool forceRecreate = false) override;
		virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func) override;

		virtual void BindTexture(uint32 attachmentIndex = 0, uint32 slot = 0) const override;
		virtual uint32 GetWidth() const override { return m_Specification.Width; }
		virtual uint32 GetHeight() const override { return m_Specification.Height; }

		virtual void ClearAttachment(uint32 attachmentIndex, int32 value) override;
		virtual int32 ReadPixel(uint32 attachmentIndex, int32 x, int32 y) override;

		virtual HLRendererID GetRendererID() const override { return m_RendererID; }

		virtual Ref<Texture> GetImage(uint32 attachmentIndex = 0) const override;
		virtual Ref<Texture> GetDepthImage() const override;

		virtual FramebufferSpecification &GetSpecification() override { return m_Specification; }
		virtual const FramebufferSpecification &GetSpecification() const override { return m_Specification; }

		// TODO: make this global
		uint64 GetColorAttachmentCount() const { return m_Specification.SwapChainTarget ? 1 : m_AttachmentImages.size(); }
		bool HasDepthAttachment() const { return (bool)m_DepthImage; }

		// Vulkan-specific
		VkRenderPass GetRenderPass() const { return m_RenderPass; }
		VkFramebuffer GetFramebuffer() const { return m_Framebuffer; }
		const std::vector<VkClearValue> &GetClearValues() const { return m_ClearValues; }

	private:

		HLRendererID m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<Ref<Texture2D>> m_AttachmentImages;
		Ref<Texture2D> m_DepthImage;

		std::vector<VkClearValue> m_ClearValues;

		VkRenderPass m_RenderPass = nullptr;
		VkFramebuffer m_Framebuffer = nullptr;

		std::vector<std::function<void(Ref<Framebuffer>)>> m_ResizeCallbacks;
	};
}

#endif // HIGHLO_API_VULKAN

