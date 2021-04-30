#pragma once
#include "RenderingAPI.h"

namespace highlo
{
	class Window;

	class Renderer
	{
	public:
		static HLAPI void ClearScreenColor(const glm::vec4& color);
		static HLAPI void ClearScreenBuffers();

		static HLAPI void SetWireframe(bool wf);
		static HLAPI void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static HLAPI void Init(Window* window);
		static HLAPI void Shutdown();

	private:
		static UniqueRef<RenderingAPI> s_RenderingAPI;

	private:
		/*friend class Renderer2D;
		friend class Renderer3D;*/
	};
}
