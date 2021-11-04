// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Renderer.h"

#include "Renderer2D.h"

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
		RendererCapabilities Capabilities;
		Ref<Texture3D> BlackCubeTexture;
		Ref<Texture2D> WhiteTexture;
		Ref<Texture2D> BRDFLut;
		Ref<Environment> EmptyEnvironment;
		Ref<ShaderLibrary> ShaderLib;
		Ref<RenderPass> ActiveRenderPass;
	};

	static RendererData *s_MainRendererData = nullptr;
	static RenderCommandQueue *s_CommandQueue = nullptr;

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

	void Renderer::SetBlendMode(bool bEnabled)
	{
		s_RenderingAPI->SetBlendMode(bEnabled);
	}

	void Renderer::SetMultiSample(bool bEnabled)
	{
		s_RenderingAPI->SetMultiSample(bEnabled);
	}

	void Renderer::SetDepthTest(bool bEnabled)
	{
		s_RenderingAPI->SetDepthTest(bEnabled);
	}

	void Renderer::SetLineThickness(float thickness)
	{
		s_RenderingAPI->SetLineThickness(thickness);
	}

	void Renderer::Init(Window *window)
	{
		s_MainRendererData = new RendererData();
		s_CommandQueue = new RenderCommandQueue();
		s_MainRendererData->ShaderLib = Ref<ShaderLibrary>::Create();

		uint32 blackTextureData[6] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };
		s_MainRendererData->BlackCubeTexture = Texture3D::Create(TextureFormat::RGBA, 1, 1, &blackTextureData).As<Texture3D>();
		
		uint32 whiteTextureData[6] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
		s_MainRendererData->WhiteTexture = Texture2D::Create(TextureFormat::RGBA, 1, 1, &whiteTextureData).As<Texture2D>();

		s_MainRendererData->BRDFLut = Texture2D::LoadFromFile("assets/Resources/brdfMap.png").As<Texture2D>();
		s_MainRendererData->EmptyEnvironment = Ref<Environment>::Create(s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture);

		// Define Shader layouts
		BufferLayout staticShaderLayout = BufferLayout::GetStaticShaderLayout();
		BufferLayout animatedShaderLayout = BufferLayout::GetAnimatedShaderLayout();

		// Load 3D Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/DefaultShader.glsl", staticShaderLayout);
		Renderer::GetShaderLibrary()->Load("assets/shaders/DefaultAnimatedShader.glsl", animatedShaderLayout);
		Renderer::GetShaderLibrary()->Load("assets/shaders/DefaultShaderPBR.glsl", staticShaderLayout);
		Renderer::GetShaderLibrary()->Load("assets/shaders/DefaultAnimatedShaderPBR.glsl", animatedShaderLayout);
		Renderer::GetShaderLibrary()->Load("assets/shaders/SkyboxShader.glsl", BufferLayout::GetSkyboxLayout());
		Renderer::GetShaderLibrary()->Load("assets/shaders/GridShader.glsl", BufferLayout::GetGridLayout());

		// Load 2D Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/Renderer2DQuad.glsl", BufferLayout::GetTextureLayout());
		Renderer::GetShaderLibrary()->Load("assets/shaders/Renderer2DLine.glsl", BufferLayout::GetLineLayout());
		Renderer::GetShaderLibrary()->Load("assets/shaders/Renderer2DCircle.glsl", BufferLayout::GetCircleLayout());
		Renderer::GetShaderLibrary()->Load("assets/shaders/Renderer2DText.glsl", BufferLayout::GetTextLayout());

		// Load Compute Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/EquirectangularToCubeMap.glsl", BufferLayout::Empty, true);
		Renderer::GetShaderLibrary()->Load("assets/shaders/EnvironmentMipFilter.glsl", BufferLayout::Empty, true);
		Renderer::GetShaderLibrary()->Load("assets/shaders/EnvironmentIrradiance.glsl", BufferLayout::Empty, true);

		UI::InitImGui(window, UI::ImGuiWindowStyle::Dark);
		s_RenderingAPI->Init();

		WaitAndRender();

		CoreRenderer::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		CoreRenderer::Shutdown();
		s_RenderingAPI->Shutdown();
		UI::ShutdownImGui();

		delete s_CommandQueue;
		delete s_MainRendererData;
	}

	void Renderer::BeginFrame()
	{
		s_RenderingAPI->BeginFrame();
	}

	void Renderer::EndFrame()
	{
		s_RenderingAPI->EndFrame();
	}

	void Renderer::WaitAndRender()
	{
		HL_PROFILE_FUNCTION();
		s_CommandQueue->Execute();
	}

	void Renderer::BeginRenderPass(const Ref<RenderPass> &renderPass, bool clear)
	{
		HL_ASSERT(renderPass, "Renderpass can not be null!");
		s_MainRendererData->ActiveRenderPass = renderPass;

		renderPass->GetSpcification().Framebuffer->Bind();
		if (clear)
		{
			const glm::vec4 &clearColor = renderPass->GetSpcification().Framebuffer->GetSpecification().ClearColor;
			s_RenderingAPI->ClearScreenBuffers();
			s_RenderingAPI->ClearScreenColor(clearColor);
		}
	}

	void Renderer::EndRenderPass()
	{
		HL_ASSERT(s_MainRendererData->ActiveRenderPass, "No active Render pass! Have you called Renderer::EndRenderPass twice?");
		s_MainRendererData->ActiveRenderPass->GetSpcification().Framebuffer->Unbind();
		s_MainRendererData->ActiveRenderPass = nullptr;
	}

	void Renderer::DrawAABB(const Ref<Model> &model, const glm::mat4 &transform, const glm::vec4 &color)
	{
		DrawAABB(model->BoundingBox, transform, color);
	}

	void Renderer::DrawAABB(const AABB &aabb, const glm::mat4 &transform, const glm::vec4 &color)
	{
		glm::vec4 min = { aabb.Min.x, aabb.Min.y, aabb.Min.z, 1.0f };
		glm::vec4 max = { aabb.Max.x, aabb.Max.y, aabb.Max.z, 1.0f };

		glm::vec4 corners[8] =
		{
			transform * glm::vec4{ aabb.Min.x, aabb.Min.y, aabb.Max.z, 1.0f },
			transform * glm::vec4{ aabb.Min.x, aabb.Max.y, aabb.Max.z, 1.0f },
			transform * glm::vec4{ aabb.Max.x, aabb.Max.y, aabb.Max.z, 1.0f },
			transform * glm::vec4{ aabb.Max.x, aabb.Min.y, aabb.Max.z, 1.0f },

			transform * glm::vec4{ aabb.Min.x, aabb.Min.y, aabb.Min.z, 1.0f },
			transform * glm::vec4{ aabb.Min.x, aabb.Max.y, aabb.Min.z, 1.0f },
			transform * glm::vec4{ aabb.Max.x, aabb.Max.y, aabb.Min.z, 1.0f },
			transform * glm::vec4{ aabb.Max.x, aabb.Min.y, aabb.Min.z, 1.0f }
		};

		/*
		for (uint32 i = 0; i < 4; ++i)
			Renderer2D::DrawLine(corners[i], corners[(i + 1) % 4], color);

		for (uint32 i = 0; i < 4; ++i)
			Renderer2D::DrawLine(corners[i + 4], corners[((i + 1) % 4) + 4], color);

		for (uint32 i = 0; i < 4; ++i)
			Renderer2D::DrawLine(corners[i], corners[i + 4], color);
		*/
	}

	Ref<Texture3D> Renderer::GetBlackCubeTexture()
	{
		return s_MainRendererData->BlackCubeTexture;
	}

	Ref<Texture2D> Renderer::GetWhiteTexture()
	{
		return s_MainRendererData->WhiteTexture;
	}

	Ref<Environment> Renderer::GetEmptyEnvironment()
	{
		return s_MainRendererData->EmptyEnvironment;
	}

	RendererConfig &Renderer::GetConfig()
	{
		return s_MainRendererData->Config;
	}

	Ref<ShaderLibrary> Renderer::GetShaderLibrary()
	{
		return s_MainRendererData->ShaderLib;
	}

	RendererCapabilities &Renderer::GetCapabilities()
	{
		return s_MainRendererData->Capabilities;
	}

	Ref<Texture2D> &Renderer::GetBRDFLutTexture()
	{
		return s_MainRendererData->BRDFLut;
	}

	HLString Renderer::GetCurrentRenderingAPI()
	{
	#ifdef HIGHLO_API_OPENGL
		return "OpenGL";
	#elif HIGHLO_API_DX11
		return "DirectX11";
	#elif HIGHLO_API_DX12
		return "DirectX12";
	#elif HIGHLO_API_VULKAN
		return "Vulkan";
	#elif HIGHLO_API_METAL
		return "Metal";
	#else
		return "Unknown";
	#endif // HIGHLO_API_OPENGL
	}

	Ref<Environment> Renderer::CreateEnvironment(const HLString &path)
	{
		return s_RenderingAPI->CreateEnvironment(path);
	}
	
	RenderCommandQueue &Renderer::GetRenderCommandQueue()
	{
		return *s_CommandQueue;
	}
}