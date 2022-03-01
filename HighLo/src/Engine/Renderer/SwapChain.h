// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Engine/Renderer/RenderingContext.h"

namespace highlo
{
	class SwapChain : public IsSharedReference
	{
	public:

		HLAPI virtual ~SwapChain() = default;

		HLAPI virtual void Init(const Ref<RenderingContext> &context) = 0;
		HLAPI virtual void InitSurface(void *windowHandle) = 0;
		HLAPI virtual void Create(uint32 *width, uint32 *height, bool vsync) = 0;
		HLAPI virtual void Cleanup() = 0;

		HLAPI virtual void OnResize(uint32 width, uint32 height) = 0;

		HLAPI virtual void BeginFrame() = 0;
		HLAPI virtual void EndFrame() = 0;
		HLAPI virtual void Present() = 0;

		HLAPI virtual uint32 GetImageCount() const = 0;
		HLAPI virtual uint32 GetWidth() const = 0;
		HLAPI virtual uint32 GetHeight() const = 0;
		HLAPI virtual uint32 GetCurrentBufferIndex() const = 0;

		HLAPI static Ref<SwapChain> Create();
	};
}

