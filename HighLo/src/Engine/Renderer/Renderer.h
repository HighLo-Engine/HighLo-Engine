// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-10-17) Added RenderCommandQueue
//     - 1.1 (2021-09-29) Added Renderer2DText Shader loading to init function
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Application/Application.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Core/Allocator.h"
#include "Engine/ImGui/ImGui.h"
#include "RenderingAPI.h"
#include "CoreRenderer.h"
#include "SceneRenderer.h"
#include "Texture.h"
#include "Environment.h"
#include "Shaders/ShaderLibrary.h"
#include "RendererCapabilities.h"
#include "RenderPass.h"
#include "Engine/Math/AABB.h"
#include "RenderCommandQueue.h"
#include "CommandBuffer.h"
#include "RenderingContext.h"
#include "CommandBuffer.h"
#include "ComputePipeline.h"
#include "Shaders/UniformBufferSet.h"
#include "Shaders/StorageBufferSet.h"

namespace highlo
{
	class Window;

	struct RendererConfig
	{
		bool ComputeEnvironmentMaps = true;
		uint32 FramesInFlight = 3;

		uint32 EnvironmentMapResolution = 1024; // TODO: implement this into the CreateEnvironment functions for each rendering api, because this should be controlable via the editor
		uint32 IrradianceMapComputeSamples = 512;
	};

	class Renderer
	{
	public:

		HLAPI static void Init(Window *window);
		HLAPI static void Shutdown();

		HLAPI static void BeginFrame();
		HLAPI static void EndFrame();

		HLAPI static void BeginRenderPass(Ref<CommandBuffer> &renderCommandBuffer, Ref<RenderPass> &renderPass, bool shouldClear = false);
		HLAPI static void EndRenderPass(Ref<CommandBuffer> &renderCommandBuffer);

		HLAPI static void ClearScreenColor(const glm::vec4 &color);
		HLAPI static void ClearScreenColor(float r, float g, float b, float a);
		HLAPI static void ClearScreenBuffers();

		HLAPI static void SetWireframe(bool wf);
		HLAPI static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
		HLAPI static void SetBlendMode(bool bEnabled);
		HLAPI static void SetMultiSample(bool bEnabled);
		HLAPI static void SetDepthTest(bool bEnabled);
		HLAPI static void SetLineThickness(float thickness);
	
		template<typename T>
		HLAPI static void Submit(T &&func)
		{
			if (HLApplication::Get().IsShuttingDown())
				return;

			auto renderCmd = [](void *ptr)
			{
				auto pFunc = (T*)ptr;
				(*pFunc)();
				pFunc->~T();
			};

			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) T(std::forward<T>(func));
		}

		template<typename T>
		HLAPI static void SubmitWithoutResources(T &&func)
		{
			auto renderCmd = [](void *ptr)
			{
				auto pFunc = (T*)ptr;
				(*pFunc)();
				pFunc->~T();
			};

			Submit([renderCmd, func]()
			{
				const uint32 index = Renderer::GetCurrentFrameIndex();
				auto storageBuffer = GetRenderResourceReleaseQueue(index).Allocate(renderCmd, sizeof(func));
				new (storageBuffer) T(std::forward<T>((T&&)func));
			});
		}

		HLAPI static void WaitAndRender();
		HLAPI static void RenderQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4 &transform = glm::mat4(1.0f));

		HLAPI static void RenderInstancedStaticMesh(
			Ref<CommandBuffer> renderCommandBuffer, 
			Ref<VertexArray> va,
			Ref<UniformBufferSet> uniformBufferSet, 
			Ref<StorageBufferSet> storageBufferSet, 
			Ref<StaticModel> model, 
			uint32 submeshIndex, 
			Ref<MaterialTable> materials, 
			Ref<VertexBuffer> transformBuffer, 
			uint32 transformBufferOffset, 
			uint32 instanceCount);

		HLAPI static void OnShaderReloaded(uint64 hash);
		HLAPI static void RegisterShaderDependency(Ref<Shader> shader, Ref<ComputePipeline> computePipeline);
		HLAPI static void RegisterShaderDependency(Ref<Shader> shader, Ref<VertexArray> va);
		HLAPI static void RegisterShaderDependency(Ref<Shader> shader, Ref<Material> material);

		HLAPI static Ref<Texture3D> GetBlackCubeTexture();
		HLAPI static Ref<Texture2D> GetWhiteTexture();
		HLAPI static Ref<Environment> GetEmptyEnvironment();
		HLAPI static RendererConfig &GetConfig();
		HLAPI static Ref<ShaderLibrary> GetShaderLibrary();
		HLAPI static RendererCapabilities &GetCapabilities();
		HLAPI static Ref<Texture2D> &GetBRDFLutTexture();
		HLAPI static HLString GetCurrentRenderingAPI();
		HLAPI static void SetLineWidth(float width);
		HLAPI static float GetCurrentLineWidth();

		HLAPI static Ref<Environment> CreateEnvironment(const FileSystemPath &filePath);
		HLAPI static Ref<Texture3D> CreatePreethamSky(float turbidity, float azimuth, float inclination);
		HLAPI static void SetSceneEnvironment(Ref<SceneRenderer> sceneRenderer, Ref<Environment> environment, Ref<Texture2D> shadow, Ref<Texture2D> linearDepth);

		HLAPI static Ref<RenderingContext> GetContext();

		HLAPI static uint32 GetCurrentFrameIndex();
		HLAPI static RenderCommandQueue &GetRenderResourceReleaseQueue(uint32 index);
		HLAPI static RenderCommandQueue &GetRenderCommandQueue();

	private:

		static UniqueRef<RenderingAPI> s_RenderingAPI;

		friend class CoreRenderer;
		friend class Renderer2D;
	};

	namespace utils
	{
		inline void DumpGPUInfos()
		{
			auto &caps = Renderer::GetCapabilities();
			HL_CORE_TRACE("GPU Info:");
			HL_CORE_TRACE("  Vendor: {0}", *caps.Vendor);
			HL_CORE_TRACE("  Device: {0}", *caps.Device);
			HL_CORE_TRACE("  Version: {0}", *caps.Version);
		}
	}
}

