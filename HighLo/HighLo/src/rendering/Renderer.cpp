#include "Renderer.h"

#ifdef HIGHLO_API_OPENGL
#include <api/opengl/rendering/OpenGLRenderingAPI.h>
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include <api/dx11/rendering/DX11RenderingAPI.h>
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<RenderingAPI>(new OpenGLRenderingAPI());
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<RenderingAPI>(new DX11RenderingAPI());
#endif // HIGHLO_API_DX11

	void Renderer::ClearScreenColor(const glm::vec4& color)
	{
		s_RenderingAPI->ClearScreenColor(color);
	}

	void Renderer::ClearScreenBuffers()
	{
		s_RenderingAPI->ClearScreenBuffers();
	}

	void Renderer::SetWireframe(bool wf)
	{
		s_RenderingAPI->SetWireframe(wf);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RenderingAPI->SetViewport(x, y, width, height);
	}

	void Renderer::Init(Window* window)
	{
	}

	void Renderer::Shutdown()
	{
	}
}