// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Renderer.h"

#include "Renderer2D.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLRenderingAPI.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11RenderingAPI.h"
#elif HIGHLO_API_DX12
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
		s_MainRendererData->EmptyEnvironment = Ref<Environment>::Create("", s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture, s_MainRendererData->BlackCubeTexture, s_MainRendererData->BRDFLut);

		// Load 3D Shaders
	//	Renderer::GetShaderLibrary()->Load("assets/shaders/HighLoPBRAnimated.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/HighLoPBR.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/Skybox.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/GridShader.glsl");

		// Load 2D Shaders
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DQuad.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DLine.glsl");
		Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DCircle.glsl");
		//Renderer::GetShaderLibrary()->Load("assets/shaders/2D/Renderer2DText.glsl");

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

	void Renderer::RenderDynamicModel(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va,
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<DynamicModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materialTable, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformOffset, 
		uint32 instanceCount)
	{
		s_RenderingAPI->RenderDynamicModel(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materialTable, transformBuffer, transformOffset, instanceCount);
	}

	void Renderer::RenderStaticModel(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<StaticModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materialTable, 
		const Transform &transform)
	{
		s_RenderingAPI->RenderStaticModel(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materialTable, transform);
	}

	void Renderer::RenderDynamicModelWithMaterial(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va,
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<DynamicModel> model, 
		uint32 submeshIndex, 
		Ref<VertexBuffer> transformBuffer, 
		uint32 transformOffset, 
		uint32 instanceCount, 
		Ref<Material> material, 
		Allocator additionalUniforms)
	{
		s_RenderingAPI->RenderDynamicModelWithMaterial(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, transformBuffer, transformOffset, instanceCount, material, additionalUniforms);
	}

	void Renderer::RenderStaticModelWithMaterial(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<StaticModel> model, 
		uint32 submeshIndex, 
		Ref<MaterialTable> materialTable, 
		const Transform &transform)
	{
		s_RenderingAPI->RenderStaticModelWithMaterial(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, model, submeshIndex, materialTable, transform);
	}

	void Renderer::RenderQuad(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<Material> material, 
		const Transform &transform)
	{
		s_RenderingAPI->RenderQuad(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, material, transform);
	}

	void Renderer::RenderGeometry(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<Material> material, 
		Ref<VertexBuffer> vertexBuffer, 
		Ref<IndexBuffer> indexBuffer, 
		const Transform &transform, 
		uint32 indexCount)
	{
		s_RenderingAPI->RenderGeometry(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, material, vertexBuffer, indexBuffer, transform, indexCount);
	}

	void Renderer::RenderGeometry(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const Transform &transform)
	{
		s_RenderingAPI->RenderGeometry(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, material, va->GetVertexBuffers()[0], va->GetIndexBuffer(), transform, va->GetIndexBuffer()->GetCount());
	}

	void Renderer::RenderGeometry(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<Material> material, const Transform &transform)
	{
		s_RenderingAPI->RenderGeometry(renderCommandBuffer, va, nullptr, nullptr, material, va->GetVertexBuffers()[0], va->GetIndexBuffer(), transform, va->GetIndexBuffer()->GetCount());
	}

	void Renderer::SubmitFullscreenQuad(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<Material> material)
	{
		s_RenderingAPI->SubmitFullscreenQuad(renderCommandBuffer, va, uniformBufferSet, material);
	}

	void Renderer::SubmitFullscreenQuad(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<Material> material)
	{
		s_RenderingAPI->SubmitFullscreenQuad(renderCommandBuffer, va, uniformBufferSet, storageBufferSet, material);
	}

	void Renderer::SubmitFullscreenQuadWithOverrides(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<VertexArray> va, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<Material> material, 
		Allocator vertexShaderOverrides, 
		Allocator fragmentShaderOverrides)
	{
		s_RenderingAPI->SubmitFullscreenQuadWithOverrides(renderCommandBuffer, va, uniformBufferSet, material, vertexShaderOverrides, fragmentShaderOverrides);
	}

	void Renderer::DispatchComputeShader(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<ComputePipeline> computePipeline, 
		Ref<UniformBufferSet> uniformBufferSet, 
		Ref<StorageBufferSet> storageBufferSet, 
		Ref<Material> material, 
		const glm::ivec3 &groups)
	{
		s_RenderingAPI->DispatchComputeShader(renderCommandBuffer, computePipeline, uniformBufferSet, storageBufferSet, material, groups);
	}

	void Renderer::ClearTexture(
		Ref<CommandBuffer> renderCommandBuffer, 
		Ref<Texture2D> texture)
	{
		s_RenderingAPI->ClearTexture(renderCommandBuffer, texture);
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