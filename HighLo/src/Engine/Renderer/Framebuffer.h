#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/HLCore.h"
#include "Engine/Core/HLLog.h"
#include "Engine/Core/DataTypes/HLString.h"
#include "Engine/Renderer/Image.h"

namespace highlo
{
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(ImageFormat format)
			: m_Format(format) {}

		ImageFormat m_Format;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> &attachments)
			: m_Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> m_Attachments;
	};

	struct FramebufferSpecification
	{
		float Scale = 1.0f;
		uint32 Width = 0;
		uint32 Height = 0;

		glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		FramebufferAttachmentSpecification Attachments;
		uint32 Samples = 1;

		bool NoResize = false;
		bool SwapChainTarget = false;

		HLString DebugName;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32 width, uint32 height, bool forceRecreate = false) = 0;
		virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func) = 0;

		virtual void BindTexture(uint32 attachmentIndex = 0, uint32 slot = 0) const = 0;
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual void ClearAttachment(uint32 attachmentIndex, int32 value) = 0;
		virtual int32 ReadPixel(uint32 attachmentIndex, int32 x, int32 y) = 0;

		virtual HLRendererID GetRendererID() const = 0;

		virtual Ref<Image2D> GetImage(uint32 attachmentIndex = 0) const = 0;
		virtual Ref<Image2D> GetDepthImage() const = 0;

		virtual const FramebufferSpecification &GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification &spec);
	};

	class FramebufferPool final
	{
	public:

		FramebufferPool(uint32 maxFbs = 32);
		~FramebufferPool();

		WeakRef<Framebuffer> AllocateBuffer();
		void Add(const Ref<Framebuffer> &framebuffer);

		inline std::vector<Ref<Framebuffer>> &GetAll() { return m_Pool; }
		inline const std::vector<Ref<Framebuffer>> &GetAll() const { return m_Pool; }

		inline static FramebufferPool *GetGlobal() { return s_Instance; }

	private:

		std::vector<Ref<Framebuffer>> m_Pool;
		static FramebufferPool *s_Instance;
		uint32 m_MaxFramebufferCount;
	};
}