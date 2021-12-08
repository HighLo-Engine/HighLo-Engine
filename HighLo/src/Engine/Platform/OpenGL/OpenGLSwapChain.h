// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-24) initial release
//

#pragma once

#include "Engine/Renderer/SwapChain.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	/// <summary>
	/// Even though OpenGL does not have the concept of a SwapChain, we use this class as a wrapper around the OpenGLRenderinContext,
	/// to keep the same API Usage as in Vulkan
	/// </summary>
	class OpenGLSwapChain : public SwapChain
	{
	public:

		// Inherited via SwapChain
		virtual void Init(const Ref<RenderingContext> &context) override;
		virtual void InitSurface(void *windowHandle) override;
		virtual void Create(uint32 *width, uint32 *height, bool vsync) override;
		virtual void Cleanup() override;

		virtual void OnResize(uint32 width, uint32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Present() override;

		virtual uint32 GetImageCount() const override { HL_ASSERT(false, "Not supported in OpenGL"); return -1; }
		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }
		virtual uint32 GetCurrentBufferIndex() const override { return 0; } // hard coded to 0 because OpenGL does not use multiple buffers
	
	private:

		Ref<RenderingContext> m_Context;
		uint32 m_Width = 0, m_Height = 0;
		bool m_VSync;
		void *m_NativeHandle = nullptr;

		friend class OpenGLContext;
	};
}

#endif // HIGHLO_API_OPENGL

