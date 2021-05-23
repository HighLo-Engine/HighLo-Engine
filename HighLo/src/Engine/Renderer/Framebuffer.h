#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/HLCore.h"
#include "Engine/Core/HLLog.h"
#include "Engine/Core/DataTypes/HLString.h"
#include "Engine/Renderer/TextureFormat.h"
#include "Engine/Renderer/Texture.h"

namespace highlo
{
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(TextureFormat format)
			: Format(format) {}

		TextureFormat Format;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification> &attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
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

	class Framebuffer : public IsSharedReference
	{
	public:
		HLAPI virtual ~Framebuffer() = default;

		HLAPI virtual void Bind() const = 0;
		HLAPI virtual void Unbind() const = 0;

		HLAPI virtual void Resize(uint32 width, uint32 height, bool forceRecreate = false) = 0;
		HLAPI virtual void AddResizeCallback(const std::function<void(Ref<Framebuffer>)> &func) = 0;

		HLAPI virtual void BindTexture(uint32 attachmentIndex = 0, uint32 slot = 0) const = 0;
		HLAPI virtual uint32 GetWidth() const = 0;
		HLAPI virtual uint32 GetHeight() const = 0;

		HLAPI virtual void ClearAttachment(uint32 attachmentIndex, int32 value) = 0;
		HLAPI virtual int32 ReadPixel(uint32 attachmentIndex, int32 x, int32 y) = 0;

		HLAPI virtual HLRendererID GetRendererID() const = 0;

		HLAPI virtual Ref<Texture> GetImage(uint32 attachmentIndex = 0) const = 0;
		HLAPI virtual Ref<Texture> GetDepthImage() const = 0;

		HLAPI virtual FramebufferSpecification &GetSpecification() = 0;
		HLAPI virtual const FramebufferSpecification &GetSpecification() const = 0;

		HLAPI static Ref<Framebuffer> Create(const FramebufferSpecification &spec);
	};

	class FramebufferPool final
	{
	public:

		HLAPI FramebufferPool(uint32 maxFbs = 32);
		HLAPI ~FramebufferPool();

		HLAPI WeakRef<Framebuffer> AllocateBuffer();
		HLAPI void Add(const Ref<Framebuffer> &framebuffer);

		HLAPI inline std::vector<Ref<Framebuffer>> &GetAll() { return m_Pool; }
		HLAPI inline const std::vector<Ref<Framebuffer>> &GetAll() const { return m_Pool; }

		HLAPI inline static FramebufferPool *GetGlobal() { return s_Instance; }

	private:

		std::vector<Ref<Framebuffer>> m_Pool;
		static FramebufferPool *s_Instance;
		uint32 m_MaxFramebufferCount;
	};
}