// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

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
#elif HIGHLO_API_METAL
#include "Engine/Platform/Metal/MetalRenderingAPI.h"
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

	struct GlobalShaderInfo
	{
		std::unordered_map<HLString, std::unordered_map<uint64, WeakRef<Shader>>> GlobalMacros;
		std::vector<WeakRef<Shader>> DirtyShaders;
	};

	static GlobalShaderInfo s_GlobalShaderInfo;
	constexpr static uint32 s_RenderCommandQueueCount = 2;

	static RendererData *s_MainRendererData = nullptr;
	static RenderCommandQueue *s_CommandQueue[s_RenderCommandQueueCount];
	static RenderCommandQueue s_ResourceFreeQueue[3];

	static std::atomic<uint32> s_RenderCommandQueueSubmissionIndex = 0;

	void Renderer::PreInit()
	{
		s_MainRendererData = new RendererData();
		s_CommandQueue[0] = new RenderCommandQueue();
		s_CommandQueue[1] = new RenderCommandQueue();
		s_MainRendererData->ShaderLib = Ref<ShaderLibrary>::Create();
	}

	void Renderer::Init(Window *window)
	{
		// Load 3D Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/HighLoPBRAnimated.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/HighLoPBR.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/Collider.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/Skybox.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/Grid.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/SelectedGeometry.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/ShadowMap.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/ShadowMapAnimated.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/Wireframe.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/SceneComposite.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/PreDepth.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/PreDepthAnimated.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/LightCulling.glsl");
	
		// Load 2D Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DQuad.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DLine.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DCircle.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DText.glsl");
	
		// Load Compute Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/hdr/EquirectangularToCubeMap.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/hdr/EnvironmentMipFilter.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/hdr/EnvironmentIrradiance.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/hdr/PreethamSky.glsl");

		s_RenderingAPI->Init();
		UI::InitImGui(window, UI::ImGuiWindowStyle::Dark);

		uint32 blackTextureData[6] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };
		s_MainRendererData->BlackCubeTexture = Texture3D::Create(TextureFormat::RGBA, 1, 1, &blackTextureData);
		s_MainRendererData->BlackCubeTexture->GetSpecification().DebugName = "BlackCubeTexture";

		uint32 whiteTextureData[6] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
		s_MainRendererData->WhiteTexture = Texture2D::Create(TextureFormat::RGBA, 1, 1, &whiteTextureData);
		s_MainRendererData->WhiteTexture->GetSpecification().DebugName = "WhiteTexture";

		s_MainRendererData->BRDFLut = Texture2D::LoadFromFile("assets/Resources/brdfMap.png");
		s_MainRendererData->BRDFLut->GetSpecification().DebugName = "BRDFLutTexture";

		s_MainRendererData->EmptyEnvironment = Ref<Environment>::Create(s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture);

		// Make sure the queue is empty after the renderer is initialized
		HLApplication::Get().GetRenderThread().Pump();

		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		s_RenderingAPI->Shutdown();
		UI::ShutdownImGui();

		for (uint32 i = 0; i < Renderer::GetConfig().FramesInFlight; ++i)
		{
			auto &queue = Renderer::GetRenderResourceReleaseQueue(i);
			queue.Execute();
		}

		delete s_CommandQueue[0];
		delete s_CommandQueue[1];

		delete s_MainRendererData;
		s_MainRendererData = nullptr;
	}

	void Renderer::BeginFrame()
	{
		s_RenderingAPI->BeginFrame();
	}

	void Renderer::EndFrame()
	{
		s_RenderingAPI->EndFrame();
	}

	bool Renderer::UpdateDirtyShaders()
	{
		const bool updatedAnyShaders = s_GlobalShaderInfo.DirtyShaders.size();

		for (WeakRef<Shader> shader : s_GlobalShaderInfo.DirtyShaders)
			shader->Reload();

		s_GlobalShaderInfo.DirtyShaders.clear();

		return updatedAnyShaders;
	}

	void Renderer::AcknowledgeParsedGlobalMacros(const std::unordered_set<HLString> &macros, const Ref<Shader> &shader)
	{
		for (const HLString &macro : macros)
			s_GlobalShaderInfo.GlobalMacros[macro][shader->GetHash()] = shader;
	}

	void Renderer::SetMacroInShader(Ref<Shader> &shader, const HLString &name, const HLString &value)
	{
		shader->SetMacro(name, value);

		auto &it = std::find(s_GlobalShaderInfo.DirtyShaders.begin(), s_GlobalShaderInfo.DirtyShaders.end(), shader);
		if (it != s_GlobalShaderInfo.DirtyShaders.end())
		{
			// Shader is already part of the dirty shaders, so we can safely skip the next step
			return;
		}

		s_GlobalShaderInfo.DirtyShaders.push_back(shader);
	}

	void Renderer::SetGlobalMacroInShaders(const HLString &name, const HLString &value)
	{
		HL_ASSERT(s_GlobalShaderInfo.GlobalMacros.find(name) != s_GlobalShaderInfo.GlobalMacros.end(), "Macro has not been passed from any shader!");
		for (auto &[hash, shader] : s_GlobalShaderInfo.GlobalMacros.at(name))
		{
			HL_ASSERT(shader.IsValid(), "Shader was deleted!");
			shader->SetMacro(name, value);

			// First check if the shader is already part of the dirty shaders
			auto &it = std::find(s_GlobalShaderInfo.DirtyShaders.begin(), s_GlobalShaderInfo.DirtyShaders.end(), shader);
			if (it != s_GlobalShaderInfo.DirtyShaders.end())
			{
				// Shader is already part of the dirty shaders, so we can safely skip the next step
				continue;
			}

			s_GlobalShaderInfo.DirtyShaders.push_back(shader);
		}
	}

	void Renderer::WaitAndRender(RenderThread *renderThread)
	{
		// Wait for kick, then set render thread to busy
		renderThread->WaitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

		s_CommandQueue[Renderer::GetRenderQueueIndex()]->Execute();

		// Rendering has completed, set state to idle
		renderThread->Set(RenderThread::State::Idle);
	}

	void Renderer::SwapQueues()
	{
		s_RenderCommandQueueSubmissionIndex = (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
	}

	uint32 Renderer::GetRenderQueueIndex()
	{
		return (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
	}

	uint32 Renderer::GetRenderQueueSubmissionIndex()
	{
		return s_RenderCommandQueueSubmissionIndex;
	}

	void Renderer::RenderThreadFunc(RenderThread *renderThread)
	{
		while (renderThread->IsRunning())
		{
			WaitAndRender(renderThread);
		}
	}

	void Renderer::RenderFullscreenQuad(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<Material> &material, 
		const glm::mat4 &transform)
	{
		s_RenderingAPI->DrawFullscreenQuad(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, material, transform);
	}

	void Renderer::RenderDynamicMesh(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va,
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<DynamicModel> &model, 
		uint32 submeshIndex, 
		const Ref<MaterialTable> &materials, 
		const TransformVertexData *transformBuffer,
		uint32 transformBufferOffset)
	{
		s_RenderingAPI->DrawDynamicMesh(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materials, transformBuffer, transformBufferOffset);
	}

	void Renderer::RenderStaticMesh(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<StaticModel> &model, 
		uint32 submeshIndex, 
		const Ref<MaterialTable> &materials, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset)
	{
		s_RenderingAPI->DrawStaticMesh(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materials, transformBuffer, transformBufferOffset);
	}

	void Renderer::RenderInstancedDynamicMesh(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va,
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<DynamicModel> &model, 
		uint32 submeshIndex, 
		const Ref<MaterialTable> &materials, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount)
	{
		s_RenderingAPI->DrawInstancedDynamicMesh(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materials, transformBuffer, transformBufferOffset, instanceCount);
	}

	void Renderer::RenderInstancedStaticMesh(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<StaticModel> &model, 
		uint32 submeshIndex, 
		const Ref<MaterialTable> &materials, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount)
	{
		s_RenderingAPI->DrawInstancedStaticMesh(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materials, transformBuffer, transformBufferOffset, instanceCount);
	}

	void Renderer::RenderInstancedStaticMeshWithMaterial(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va,
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<StaticModel> &model,
		uint32 submeshIndex, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount, 
		Ref<Material> &overrideMaterial)
	{
		s_RenderingAPI->DrawInstancedStaticMeshWithMaterial(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, transformBuffer, transformBufferOffset, instanceCount, overrideMaterial);
	}

	void Renderer::RenderInstancedDynamicMeshWithMaterial(
		const Ref<CommandBuffer> &renderCommandBuffer, 
		const Ref<VertexArray> &va, 
		const Ref<UniformBufferSet> &uniformBufferSet, 
		const Ref<StorageBufferSet> &storageBufferSet, 
		Ref<DynamicModel> &model, 
		uint32 submeshIndex, 
		const TransformVertexData *transformBuffer, 
		uint32 transformBufferOffset, 
		uint32 instanceCount, 
		Ref<Material> &overrideMaterial)
	{
		s_RenderingAPI->DrawInstancedDynamicMeshWithMaterial(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, transformBuffer, transformBufferOffset, instanceCount, overrideMaterial);
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

	void Renderer::BeginRenderPass(Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
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

	Ref<Environment> Renderer::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		return s_RenderingAPI->CreateEnvironment(filePath, cubemapSize, irradianceMapSize);
	}

	Ref<Texture3D> Renderer::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		return s_RenderingAPI->CreatePreethamSky(turbidity, azimuth, inclination);
	}

	void Renderer::SetSceneEnvironment(
		const Ref<SceneRenderer> &sceneRenderer, 
		Ref<Environment> &environment, 
		const Ref<Texture2D> &shadow)
	{
		s_RenderingAPI->SetSceneEnvironment(sceneRenderer, environment, shadow);
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
		return *s_CommandQueue[s_RenderCommandQueueSubmissionIndex];
	}
}
