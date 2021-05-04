#pragma once

#include "RenderingAPI.h"
#include "CoreRenderer.h"

namespace highlo
{
	class Window;

	class Renderer
	{
	public:
		static HLAPI void ClearScreenColor(const glm::vec4& color);
		static HLAPI void ClearScreenBuffers();

		static HLAPI void SetWireframe(bool wf);
		static HLAPI void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);

		static HLAPI void Init(Window* window);
		static HLAPI void Shutdown();

	private:
		static UniqueRef<RenderingAPI> s_RenderingAPI;

	private:
		friend class CoreRenderer;
	};
}
