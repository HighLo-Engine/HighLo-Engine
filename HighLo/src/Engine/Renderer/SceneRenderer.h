// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-09-02) initial release
//

#pragma once

#include "Engine/Scene/Scene.h"
#include "Engine/Graphics/RenderPass.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/CommandBuffer.h"
#include "Engine/Graphics/Material.h"
#include "Engine/Graphics/Shaders/UniformBufferSet.h"
#include "Engine/Graphics/Shaders/StorageBufferSet.h"
#include "Engine/Graphics/Shaders/UniformDefinitions.h"

namespace highlo
{
	struct SceneRendererSpecification
	{
		bool SwapChain = false;
	};

	class SceneRenderer : public IsSharedReference
	{
	public:

		HLAPI SceneRenderer(Ref<Scene> &scene, SceneRendererSpecification &specification = SceneRendererSpecification());
		HLAPI virtual ~SceneRenderer();
		
		HLAPI void Init();
		HLAPI void Shutdown();

		HLAPI void SetScene(const Ref<Scene> &scene);
		HLAPI void SetViewportSize(uint32 width, uint32 height);
		HLAPI void SetClearColor(const glm::vec4 &color);

		HLAPI void BeginScene(const Camera &camera);
		HLAPI void EndScene();

		HLAPI void ClearPass(const Ref<RenderPass> &renderPass);
		HLAPI void ClearPass();

		HLAPI Ref<RenderPass> GetFinalRenderPass();
		HLAPI Ref<Texture2D> GetFinalRenderTexture();
		HLAPI const Ref<RenderPass> &GetExternalCompositeRenderPass() { return m_ExternalCompositingRenderPass; }

		HLAPI void OnUIRender();
		HLAPI static void WaitForThreads();

	private:

		void FlushDrawList();

	private:

		Ref<RenderPass> m_GeometryRenderPass = nullptr;
		Ref<VertexArray> m_GeometryVertexArray = nullptr;
		Ref<VertexArray> m_GeometryWireframeVertexArray = nullptr;
		Ref<VertexArray> m_GeometryWireframeOnTopVertexArray = nullptr;
		Ref<Material> m_GeometryMaterial = nullptr;

		Ref<RenderPass> m_ExternalCompositingRenderPass = nullptr;
		Ref<VertexArray> m_ExternalVertexArray = nullptr;
		Ref<Material> m_WireframeMaterial = nullptr;

		Ref<RenderPass> m_FinalCompositeRenderPass = nullptr;
		Ref<VertexArray> m_FinalVertexArray = nullptr;
		Ref<Material> m_CompositeMaterial = nullptr;

		uint32 m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_InvertedViewportWidth = 0.0f, m_InvertedViewportHeight = 0.0f;
		bool m_NeedsResize = false;
		bool m_Active = false;
		bool m_ResourcesCreated = false;

		Ref<Scene> m_Scene = nullptr;
		SceneRendererSpecification m_Specification;
		
		Ref<UniformBufferSet> m_UniformBufferSet;
		Ref<StorageBufferSet> m_StorageBufferSet;
		Ref<CommandBuffer> m_CommandBuffer;

		// Scene information
		struct SceneInfo
		{
			Camera SceneCamera;

			Ref<Environment> SceneEnvironment = nullptr;
			float SkyboxLod = 0.0f;
			float EnvironmentIntensity = 1.0f;
			LightEnvironment SceneLightEnvironment;
			DirectionalLight ActiveLight;
		};

		SceneInfo m_SceneData;

		// Uniform buffer blocks
		UniformBufferCamera m_CameraUniformBuffer;
		UniformBufferShadow m_ShadowUniformBuffer;
		UniformBufferScene m_SceneUniformBuffer;
		UniformBufferPointLights m_PointLightsUniformBuffer;
		UniformBufferRendererData m_RendererDataUniformBuffer;
		UniformBufferScreenData m_ScreenDataUniformBuffer;
		UniformBufferHBAOData m_HBAOUniformBuffer;
	};
}

