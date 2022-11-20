// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/SwapChain.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanSwapChain : public SwapChain
	{
	public:

		VulkanSwapChain();
		virtual ~VulkanSwapChain();

		virtual void Init(const Ref<RenderingContext> &context) override;
		virtual void InitSurface(void *windowHandle) override;
		virtual void Create(uint32 *width, uint32 *height, bool vsync) override;
		virtual void Cleanup() override;

		virtual void OnResize(uint32 width, uint32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Present() override;

		virtual uint32 GetImageCount() const override { return m_ImageCount; }
		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }
		virtual uint32 GetCurrentBufferIndex() const override { return m_CurrentBufferInsertIndex; }

	private:

		uint32 m_Width = 0;
		uint32 m_Height = 0;
		uint32 m_CurrentBufferInsertIndex = 0;
		uint32 m_ImageCount = 0;
	};
}

