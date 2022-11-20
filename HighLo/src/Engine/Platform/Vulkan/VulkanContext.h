// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/RenderingContext.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanContext : public RenderingContext
	{
	public:

		VulkanContext(void *handle, WindowData &data);
		virtual ~VulkanContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void MakeCurrent() override;
		virtual void SetSwapInterval(bool bEnabled) override;

		virtual void *GetCurrentContext() override;

	private:

		void *m_NativeWindowHandle = nullptr;
	};
}

