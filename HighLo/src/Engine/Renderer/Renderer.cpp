// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Renderer.h"

#include "Renderer2D.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLRenderingAPI.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11RenderingAPI.h"
#elif HIGHLO_API_DX12
#include "Engine/Platform/DX12/DX12RenderingAPI.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanRenderingAPI.h"
#endif // HIGHLO_API_OPENGL

namespace highlo
{
#ifdef HIGHLO_API_OPENGL
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<OpenGLRenderingAPI>::Create();
#elif HIGHLO_API_DX11
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<DX11RenderingAPI>::Create();
#elif HIGHLO_API_DX12
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<DX12RenderingAPI>::Create();
#elif HIGHLO_API_METAL
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<MetalRenderingAPI>::Create();
#elif HIGHLO_API_VULKAN
	UniqueRef<RenderingAPI> Renderer::s_RenderingAPI = UniqueRef<VulkanRenderingAPI>::Create();
#endif // HIGHLO_API_OPENGL

	struct RendererData
	{
		RendererConfig Config;
		RendererCapabilities Capabilities;
		Ref<Texture3D> BlackCubeTexture;
		Ref<Texture2D> WhiteTexture;
		Ref<Texture2D> BRDFLut;
		Ref<Environment> EmptyEnvironment;
		Ref<ShaderLibrary> ShaderLib;
		float LineWidth = 2.0f;
	};

	struct ShaderDependencies
	{
		std::vector<Ref<ComputePipeline>> ComputePipelines;
		std::vector<Ref<VertexArray>> VertexArrays;
		std::vector<Ref<Material>> Materials;
	};

	static std::unordered_map<uint64, ShaderDependencies> s_ShaderDepedencies;

	static RendererData *s_MainRendererData = nullptr;
	static RenderCommandQueue *s_CommandQueue = nullptr;
	static RenderCommandQueue s_ResourceFreeQueue[3];

	void Renderer::Init(Window *window)
	{
		s_MainRendererData = new RendererData();
		s_CommandQueue = new RenderCommandQueue();
		s_MainRendererData->ShaderLib = Ref<ShaderLibrary>::Create();

		uint32 blackTextureData[6] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };
		s_MainRendererData->BlackCubeTexture = Texture3D::Create(TextureFormat::RGBA, 1, 1, &blackTextureData);
		
		uint32 whiteTextureData[6] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
		s_MainRendererData->WhiteTexture = Texture2D::Create(TextureFormat::RGBA, 1, 1, &whiteTextureData);

		s_MainRendererData->BRDFLut = Texture2D::LoadFromFile("assets/Resources/brdfMap.png");
		s_MainRendererData->EmptyEnvironment = Ref<Environment>::Create(s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture);

		// Load 3D Shaders
	//	Renderer::GetShaderLibrary()->Load("assets/shaders/HighLoPBRAnimated.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/HighLoPBR.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/Skybox.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/Grid.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/SelectedGeometry.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/ShadowMap.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/Wireframe.glsl");

		// Load 2D Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DQuad.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DLine.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DCircle.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DText.glsl");

		// Load Compute Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/hdr/EquirectangularToCubeMap.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/hdr/EnvironmentMipFilter.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/hdr/EnvironmentIrradiance.glsl");

		UI::InitImGui(window, UI::ImGuiWindowStyle::Dark);
		s_RenderingAPI->Init();

		// Make sure the queue is empty after the renderer is initialized
		Renderer::WaitAndRender();

		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
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

	void Renderer::RenderQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4 &transform)
	{
		s_RenderingAPI->DrawQuad(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, material, transform);
	}

	void Renderer::RenderInstancedStaticMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
		s_RenderingAPI->DrawInstancedStaticMesh(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materials, transformBuffer, transformBufferOffset, instanceCount);
	}

	void Renderer::OnShaderReloaded(uint64 hash)
	{
		if (s_ShaderDepedencies.find(hash) != s_ShaderDepedencies.end())
		{
			auto &dependencies = s_ShaderDepedencies.at(hash);

			for (auto &va : dependencies.VertexArrays)
			{
				va->Invalidate();
			}

			for (auto &computePipeline : dependencies.ComputePipelines)
			{
				computePipeline->Invalidate();
			}

			for (auto &material : dependencies.Materials)
			{
				material->Invalidate();
			}
		}
	}

	void Renderer::RegisterShaderDependency(Ref<Shader> shader, Ref<ComputePipeline> computePipeline)
	{
		s_ShaderDepedencies[shader->GetHash()].ComputePipelines.push_back(computePipeline);
	}

	void Renderer::RegisterShaderDependency(Ref<Shader> shader, Ref<VertexArray> va)
	{
		s_ShaderDepedencies[shader->GetHash()].VertexArrays.push_back(va);
	}

	void Renderer::RegisterShaderDependency(Ref<Shader> shader, Ref<Material> material)
	{
		s_ShaderDepedencies[shader->GetHash()].Materials.push_back(material);
	}

	void Renderer::BeginRenderPass(Ref<CommandBuffer> &renderCommandBuffer, Ref<RenderPass> &renderPass, bool shouldClear)
	{
		HL_ASSERT(renderPass, "Renderpass can not be null!");
		s_RenderingAPI->BeginRenderPass(renderCommandBuffer, renderPass, shouldClear);
	}

	void Renderer::EndRenderPass(Ref<CommandBuffer> &renderCommandBuffer)
	{
		s_RenderingAPI->EndRenderPass(renderCommandBuffer);
	}

	void Renderer::ClearScreenColor(const glm::vec4 &color)
	{
		s_RenderingAPI->ClearScreenColor(color);
	}

	void Renderer::ClearScreenColor(float r, float g, float b, float a)
	{
		s_RenderingAPI->ClearScreenColor({ r, g, b, a });
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

	void Renderer::SetLineWidth(float width)
	{
		s_MainRendererData->LineWidth = width;
	}

	float Renderer::GetCurrentLineWidth()
	{
		return s_MainRendererData->LineWidth;
	}

	Ref<Environment> Renderer::CreateEnvironment(const FileSystemPath &filePath)
	{
		return s_RenderingAPI->CreateEnvironment(filePath);
	}

	Ref<Texture3D> Renderer::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		// TODO
		return nullptr;
	}

	void Renderer::SetSceneEnvironment(Ref<SceneRenderer> sceneRenderer, Ref<Environment> environment, Ref<Texture2D> shadow, Ref<Texture2D> linearDepth)
	{
		// TODO
	}

	Ref<RenderingContext> Renderer::GetContext()
	{
		return HLApplication::Get().GetWindow().GetContext();
	}

	uint32 Renderer::GetCurrentFrameIndex()
	{
		return HLApplication::Get().GetWindow().GetSwapChain()->GetCurrentBufferIndex();
	}

	RenderCommandQueue &Renderer::GetRenderResourceReleaseQueue(uint32 index)
	{
		HL_ASSERT(index < 3);
		return s_ResourceFreeQueue[index];
	}
	
	RenderCommandQueue &Renderer::GetRenderCommandQueue()
	{
		return *s_CommandQueue;
	}
}