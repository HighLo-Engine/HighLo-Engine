#pragma once

#include "Engine/Graphics/RenderingContext.h"

#ifdef HIGHLO_API_VULKAN

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

		void *m_VulkanWindowHandle;
	};
}

#endif // HIGHLO_API_VULKAN

