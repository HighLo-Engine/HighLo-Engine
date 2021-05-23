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

	void Renderer::Init(Window* window)
	{
		s_Data = new RendererData();
		s_Data->ShaderLib = Ref<ShaderLibrary>::Create();

		uint32 blackTextureData[6] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };
		s_Data->BlackCubeTexture = Texture3D::Create(TextureFormat::RGBA, 1, 1, &blackTextureData).As<Texture3D>();
		
		uint32 whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture = Texture2D::Create(TextureFormat::RGBA, 1, 1).As<Texture2D>();
		s_Data->WhiteTexture->GetWriteableBuffer().Write(&whiteTextureData, sizeof(uint32));

		s_Data->BRDFLut = Texture2D::LoadFromFile("assets/textures/brdfMap.png").As<Texture2D>();
		s_Data->EmptyEnvironment = Ref<Environment>::Create(s_Data->BlackCubeTexture, s_Data->BlackCubeTexture, s_Data->BlackCubeTexture, s_Data->BlackCubeTexture);

		// Define Shader layouts
		BufferLayout textureLayout2D = {
			{ "in_Position", ShaderDataType::Float3 },
			{ "in_Color", ShaderDataType::Float4 },
			{ "in_TexCoord", ShaderDataType::Float2 },
			{ "in_TexIndex", ShaderDataType::Float },
			{ "in_TilingFactor", ShaderDataType::Float },
		};
		BufferLayout lineLayout2D = {
			{ "in_Position", ShaderDataType::Float3 },
			{ "in_Color", ShaderDataType::Float4 }
		};
		BufferLayout circleLayout2D = {
			{ "in_WorldPosition", ShaderDataType::Float3 },
			{ "in_Thickness", ShaderDataType::Float },
			{ "in_LocalPosition", ShaderDataType::Float2 },
			{ "in_Color", ShaderDataType::Float4 },
		};
		BufferLayout staticShaderLayout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 }
		};
		BufferLayout animatedShaderLayout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 },
			{ "TANGENT"	, ShaderDataType::Float3 },
			{ "BINORMAL", ShaderDataType::Float3 },
			{ "BONE_IDS", ShaderDataType::Int4 },
			{ "BONE_WEIGHTS", ShaderDataType::Float4 }
		};
		BufferLayout skyboxLayout = {
			{ "POSITION", ShaderDataType::Float3 },
		};

		// Load 3D Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/DefaultShader.glsl", staticShaderLayout);
		Renderer::GetShaderLibrary()->Load("assets/shaders/DefaultAnimatedShader.glsl", animatedShaderLayout);
		Renderer::GetShaderLibrary()->Load("assets/shaders/DefaultShaderPBR.glsl", staticShaderLayout);
		Renderer::GetShaderLibrary()->Load("assets/shaders/DefaultAnimatedShaderPBR.glsl", animatedShaderLayout);
		Renderer::GetShaderLibrary()->Load("assets/shaders/SkyboxShader.glsl", skyboxLayout);

		// Load 2D Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/Renderer2DQuad.glsl", textureLayout2D);
		Renderer::GetShaderLibrary()->Load("assets/shaders/Renderer2DLine.glsl", lineLayout2D);
		Renderer::GetShaderLibrary()->Load("assets/shaders/Renderer2DCircle.glsl", circleLayout2D);

		// Load Compute Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/EquirectangularToCubeMap.glsl", BufferLayout::Empty, true);
		Renderer::GetShaderLibrary()->Load("assets/shaders/EnvironmentMipFilter.glsl", BufferLayout::Empty, true);
		Renderer::GetShaderLibrary()->Load("assets/shaders/EnvironmentIrradiance.glsl", BufferLayout::Empty, true);

		s_RenderingAPI->Init();
		CoreRenderer::Init();
		Renderer2D::Init();
		ImGuiRenderer::Init(window, ImGuiWindowStyle::Dark);
	}

	void Renderer::Shutdown()
	{
		ImGuiRenderer::Shutdown();
		Renderer2D::Shutdown();
		CoreRenderer::Shutdown();
		s_RenderingAPI->Shutdown();

		delete s_Data;
	}

	Ref<Texture3D> Renderer::GetBlackCubeTexture()
	{
		return s_Data->BlackCubeTexture;
	}

	Ref<Texture2D> Renderer::GetWhiteTexture()
	{
		return s_Data->WhiteTexture;
	}

	Ref<Environment> Renderer::GetEmptyEnvironment()
	{
		return s_Data->EmptyEnvironment;
	}

	RendererConfig &Renderer::GetConfig()
	{
		return s_Data->Config;
	}

	Ref<ShaderLibrary> Renderer::GetShaderLibrary()
	{
		return s_Data->ShaderLib;
	}

	RendererCapabilities &Renderer::GetCapabilities()
	{
		return s_Data->Capabilities;
	}

	Ref<Texture2D> &Renderer::GetBRDFLutTexture()
	{
		return s_Data->BRDFLut;
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
}