#pragma once

#include "Engine/Graphics/Framebuffer.h"

#ifdef HIGHLO_API_VULKAN

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

	private:

		HLRendererID m_RendererID = 0;
		FramebufferSpecification m_Specification;
	};
}

#endif // HIGHLO_API_VULKAN

