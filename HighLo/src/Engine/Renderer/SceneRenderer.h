// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-09-26) Added CompositeRenderPass
//     - 1.1 (2021-09-15) Added SetLineWidth method
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Scene/Scene.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Renderer/RenderPass.h"
#include "Engine/Renderer/VertexArray.h"

namespace highlo
{
	struct SceneRendererOptions
	{
		bool ShowGrid = true;
	};

	struct SceneRendererSpecification
	{
		bool SwapChain = false; // where should we render? screen or framebuffer?
	};

	class SceneRenderer : public IsSharedReference
	{
	public:

		HLAPI SceneRenderer(Ref<Scene> &scene, SceneRendererSpecification &specification = SceneRendererSpecification());
		HLAPI ~SceneRenderer();

		HLAPI void Init();

		HLAPI void SetScene(Ref<Scene> scene);
		HLAPI void SetViewportSize(uint32 width, uint32 height);
		HLAPI void SetClearColor(const glm::vec4 &color);
		HLAPI void SetLineWidth(float width) { m_LineWidth = width; }

		HLAPI void BeginScene(const Camera &camera);
		HLAPI void EndScene();

		HLAPI SceneRendererOptions &GetOptions();

		HLAPI Ref<RenderPass> GetFinalRenderPass();
		HLAPI Ref<Texture2D> GetFinalRenderTexture();

	private:

		void InitGrid();
		void InitSkybox();
		void InitCompositePass();

		uint32 m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_InvertedViewportWidth = 0.0f, m_InvertedViewportHeight = 0.0f;
		bool m_NeedsResize = false;
		bool m_Active = false;
		float m_LineWidth = 2.0f;

		Ref<Scene> m_Scene;
		SceneRendererSpecification m_Specification;
		SceneRendererOptions m_RendererOptions;
		
		Ref<RenderPass> m_CompositeRenderPass;

		// Grid
		Ref<VertexArray> m_GridVertexArray;
		Ref<Shader> m_GridShader;
		Ref<Material> m_GridMaterial;
		Ref<Material> m_WireframeMaterial;
		Ref<Material> m_OutlineMaterial, m_OutlineAnimMaterial;
		Ref<Material> m_ColliderMaterial;

		// Scene information
		struct SceneInfo
		{
			Camera SceneCamera;

			Ref<Environment> SceneEnvironment;
			float SkyboxLod = 0.0f;
			float EnvironmentIntensity;

		};
	};
}

