#include "HighLoPch.h"
#include "Renderer.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLRenderingAPI.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11RenderingAPI.h"
#endif // HIGHLO_API_DX11

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<RenderingAPI>(new OpenGLRenderingAPI());
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<RenderingAPI>(new DX11RenderingAPI());
#endif // HIGHLO_API_DX11

	struct RendererData
	{
		RendererConfig Config;
		Ref<Texture3D> BlackCubeTexture;
		Ref<Environment> EmptyEnvironment;
	};

	static RendererData *s_Data = nullptr;

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

	void Renderer::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
		s_RenderingAPI->SetViewport(x, y, width, height);
	}

	void Renderer::Init(Window* window)
	{
		s_Data = new RendererData();

		uint32 blackTextureData[6] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };
		s_Data->BlackCubeTexture = Texture3D::Create(ImageFormat::RGBA, 1, 1, &blackTextureData).As<Texture3D>();
		s_Data->EmptyEnvironment = Ref<Environment>::Create(s_Data->BlackCubeTexture, s_Data->BlackCubeTexture, s_Data->BlackCubeTexture, s_Data->BlackCubeTexture);

		CoreRenderer::Init();
		ImGuiRenderer::Init(window);
	}

	void Renderer::Shutdown()
	{
		ImGuiRenderer::Shutdown();
		CoreRenderer::Shutdown();

		delete s_Data;
	}

	Ref<Texture3D> Renderer::GetBlackCubeTexture()
	{
		return s_Data->BlackCubeTexture;
	}

	Ref<Environment> Renderer::GetEmptyEnvironment()
	{
		return s_Data->EmptyEnvironment;
	}

	RendererConfig &Renderer::GetConfig()
	{
		return s_Data->Config;
	}

	Ref<Environment> Renderer::CreateEnvironment(const HLString &path)
	{
		return s_RenderingAPI->CreateEnvironment(path);
	}
}