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
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/MaterialTable.h"
#include "Engine/Renderer/ComputePipeline.h"
#include "Engine/Renderer/CommandBuffer.h"
#include "Engine/Renderer/Shaders/UniformBufferSet.h"
#include "Engine/Renderer/Shaders/StorageBufferSet.h"
#include "Engine/Renderer/Meshes/DynamicModel.h"
#include "Engine/Renderer/Meshes/StaticModel.h"

namespace highlo
{
	struct SceneRendererOptions
	{
		bool ShowGrid = true;

		//HBAO
		bool EnableHBAO = true;
		float HBAOIntensity = 1.5f;
		float HBAORadius = 1.0f;
		float HBAOBias = 0.35f;
		float HBAOBlurSharpness = 1.0f;
	};

	struct SceneRendererSpecification
	{
		bool SwapChain = false; // where should we render? screen or framebuffer?
	};

	struct CascadeData
	{
		glm::mat4 ViewProjection;
		glm::mat4 View;
		float SplitDepth;
	};

	struct DynamicDrawCommand
	{
		Ref<DynamicModel> Model;
		uint32 SubmeshIndex;
		Ref<MaterialTable> Materials;
		Ref<Material> OverrideMaterial;

		uint32 InstanceCount = 0;
		uint32 InstanceOffset = 0;
	};

	struct StaticDrawCommand
	{
		Ref<StaticModel> Model;
		uint32 SubmeshIndex;
		Ref<MaterialTable> Materials;
		Ref<Material> OverrideMaterial;

		uint32 InstanceCount = 0;
		uint32 InstanceOffset = 0;
	};

	struct MeshKey
	{
		AssetHandle MeshHandle;
		AssetHandle MaterialHandle;
		uint32 SubmeshIndex;

		HLAPI MeshKey(AssetHandle meshHandle, AssetHandle materialHandle, uint32 index)
			: MeshHandle(meshHandle), MaterialHandle(materialHandle), SubmeshIndex(index)
		{
		}

		HLAPI bool operator<(const MeshKey &other) const
		{
			if (MeshHandle < other.MeshHandle)
				return true;

			if ((MeshHandle == other.MeshHandle) && (SubmeshIndex < other.SubmeshIndex))
				return true;

			return (MeshHandle == other.MeshHandle) && (SubmeshIndex == other.SubmeshIndex) && (MaterialHandle < other.MaterialHandle);
		}
	};

	struct TransformVertexData
	{
		glm::vec4 Rows[3];
	};

	struct TransformMapData
	{
		std::vector<TransformVertexData> Transforms;
		uint32 TransformOffset = 0;
	};

	struct DirLight
	{
		glm::vec3 Direction;
		float Padding = 0.0f;
		glm::vec3 Radiance;
		float Multiplier;
	};

	struct UniformBufferCamera
	{
		glm::mat4 ViewProjection;
		glm::mat4 InverseViewProjection;
		glm::mat4 Projection;
		glm::mat4 View;
	};

	struct UniformBufferHBAOData
	{
		glm::vec4 PerspectiveInfo;
		glm::vec2 InvQuarterResolution;
		float RadiusToScreen;
		float NegInvR2;
		float NDotVBias;
		float AOMultiplier;
		float PowExponent;
		bool IsOrtho;
		char Padding0[3] = { 0, 0, 0 };
		glm::vec4 Float2Offsets[16];
		glm::vec4 Jitters[16];
	};

	struct UniformBufferShadow
	{
		glm::mat4 ViewProjection[4];
	};

	struct UniformBufferScene
	{
		DirLight Lights;
		glm::vec3 u_CameraPosition;
		float EnvMapIntensity = 1.0f;
	};

	struct UniformBufferPointLights
	{
		uint32 Count = 0;
		glm::vec3 Padding{};
		PointLight PointLights[1024]{};
	};

	struct UniformBufferRendererData
	{
		glm::vec4 CascadeSplits;
		uint32 TilesCountX{ 0 };
		bool ShowCascades = false;
		char Padding0[3] = { 0, 0, 0 };
		bool SoftShadows = true;
		char Padding1[3] = { 0, 0, 0 };
		float LightSize = 0.5f;
		float MaxShadowDistance = 200.0f;
		float ShadowFade = 1.0f;
		bool CascadeFading = true;
		char Padding2[3] = { 0, 0, 0 };
		float CascadeTransitionFade = 1.0f;
		bool ShowLightComplexity = false;
		char Padding3[3] = { 0, 0, 0 };
	};

	struct UniformBufferScreenData
	{
		glm::vec2 InvFullRes;
		glm::vec2 FullRes;
	};

	class SceneRenderer : public IsSharedReference
	{
	public:

		HLAPI SceneRenderer(Ref<Scene> &scene, SceneRendererSpecification &specification = SceneRendererSpecification());
		HLAPI virtual ~SceneRenderer();

		HLAPI void Init();

		HLAPI void SetScene(Ref<Scene> scene);
		HLAPI void SetViewportSize(uint32 width, uint32 height);
		HLAPI void SetClearColor(const glm::vec4 &color);

		HLAPI void BeginScene(const Camera &camera);
		HLAPI void EndScene();

		HLAPI void SubmitStaticModel(Ref<StaticModel> model, Ref<MaterialTable> materials, const glm::mat4 &transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);
		HLAPI void SubmitDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, const glm::mat4 &transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);

		HLAPI void SubmitSelectedStaticModel(Ref<StaticModel> model, Ref<MaterialTable> materials, const glm::mat4 &transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);
		HLAPI void SubmitSelectedDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, const glm::mat4 &transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);

		HLAPI void SubmitPhysicsDebugStaticModel(Ref<StaticModel> model, const glm::mat4 &transform = glm::mat4(1.0f));
		HLAPI void SubmitPhysicsDebugDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, const glm::mat4 &transform = glm::mat4(1.0f));

		HLAPI void ClearPass(Ref<RenderPass> renderPass, bool explicitClear = false);
		HLAPI void ClearPass();

		HLAPI void UpdateHBAOData();

		HLAPI SceneRendererOptions &GetOptions();

		HLAPI Ref<RenderPass> GetFinalRenderPass();
		HLAPI Ref<Texture2D> GetFinalRenderTexture();

		HLAPI void OnUIRender();
		HLAPI static void WaitForThreads();

	private:

		void FlushDrawList();
		void PreRender();
		void DeinterleavingPass();
		void HBAOPass();
		void ReinterleavingPass();
		void HBAOBlurPass();
		void ShadowMapPass();
		void PreDepthPass();
		void LightCullingPass();
		void GeometryPass();
		void JumpFloodPass();

		// Post-processing
		void BloomCompute();
		void CompositePass();

		void UpdateStatistics();

		void CalculateCascades(CascadeData *data, const Camera &sceneCamera, const glm::vec3 &lightDir) const;

	private:

		// Initializers
		void InitLightCullingCompute();
		void InitShadowPass();
		void InitPreDepthPass();
		void InitGeometryPass();
		void InitBloomCompute();
		void InitDeinterleaving();
		void InitHBAO();
		void InitReinterleaving();
		void InitBlurFirstPass();
		void InitBlurSecondPass();
		void InitCompositePass();
		void InitDOF();
		void InitExternalCompositePass();
		void InitJumpFlood();
		void InitGrid();
		void InitSkybox();

	private:

		uint32 m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_InvertedViewportWidth = 0.0f, m_InvertedViewportHeight = 0.0f;
		bool m_NeedsResize = false;
		bool m_Active = false;
		bool m_ResourcesCreated = false;

		Ref<Scene> m_Scene;
		SceneRendererSpecification m_Specification;
		SceneRendererOptions m_RendererOptions;

		Ref<UniformBufferSet> m_UniformBufferSet;
		Ref<StorageBufferSet> m_StorageBufferSet;
		Ref<CommandBuffer> m_CommandBuffer;

		// general drawing
		std::map<MeshKey, DynamicDrawCommand> m_DynamicDrawList;
		std::map<MeshKey, DynamicDrawCommand> m_DynamicSelectedMeshDrawList;
		std::map<MeshKey, DynamicDrawCommand> m_DynamicShadowPassDrawList;

		std::map<MeshKey, StaticDrawCommand> m_StaticDrawList;
		std::map<MeshKey, StaticDrawCommand> m_StaticSelectedMeshDrawList;
		std::map<MeshKey, StaticDrawCommand> m_StaticShadowPassDrawList;

		// Debug draw lists
		std::map<MeshKey, StaticDrawCommand> m_StaticColliderDrawList;
		std::map<MeshKey, DynamicDrawCommand> m_DynamicColliderDrawList;
		std::map<MeshKey, TransformMapData> m_MeshTransformMap;

		Ref<VertexBuffer> m_SubmeshTransformBuffer;
		TransformVertexData *m_TransformVertexData = nullptr;

		// Bloom
		Ref<Shader> m_BloomBlurShader;
		Ref<Shader> m_BloomBlendShader;

		// Shadows
		float m_LightDistance = 0.1f;
		float m_CascadeSplitLambda = 0.92f;
		glm::vec4 m_CascadeSplits;
		float m_CascadeFarPlaneOffset = 50.0f, m_CascadeNearPlaneOffset = -50.0f;

		Ref<VertexArray> m_ShadowPassVertexArrays[4];
		Ref<Material> m_ShadowPassMaterial;
		Ref<Shader> m_ShadowMapShader, m_ShadowMapAnimShader;

		bool m_EnableBloom = false;
		float m_BloomThreshold = 1.5f;

		glm::vec2 m_FocusPoint = { 0.5f, 0.5f };

		// Composite
		Ref<Material> m_CompositeMaterial;
		Ref<VertexArray> m_CompositeVertexArray;

		// External compositing
		Ref<VertexArray> m_GeometryWireframeVertexArray;
		Ref<VertexArray> m_GeometryWireframeOnTopVertexArray;
		Ref<RenderPass> m_ExternalCompositingRenderPass;

		// Light Culling
		glm::ivec3 m_LightCullingWorkGroups;
		Ref<Material> m_LightCullingMaterial;
		Ref<ComputePipeline> m_LightCullingPipeline;

		// Geometry
		Ref<VertexArray> m_GeometryVertexArray;
		Ref<VertexArray> m_SelectedGeometryVertexArray;
		Ref<Material> m_SelectedGeometryMaterial;

		// Pre-Depth
		Ref<Material> m_PreDepthMaterial;
		Ref<VertexArray> m_PreDepthVertexArray;

		// HBAO
		Ref<Material> m_DeinterleavingMaterial;
		Ref<Material> m_ReinterleavingMaterial;
		Ref<Material> m_HBAOBlurMaterials[2];
		Ref<Material> m_HBAOMaterial;
		Ref<VertexArray> m_DeinterleavingPipelines[2];
		Ref<VertexArray> m_ReinterleavingPipeline;
		Ref<VertexArray> m_HBAOBlurPipelines[2];

		Ref<ComputePipeline> m_HBAOPipeline;
		Ref<Texture2D> m_HBAOOutputImage;
		Ref<RenderPass> m_HBAORenderPass;
		glm::ivec3 m_HBAOWorkGroups{ 32.f, 32.f, 16.f };

		// Grid
		Ref<VertexArray> m_GridVertexArray;
		Ref<Shader> m_GridShader;
		Ref<Material> m_GridMaterial;
		Ref<Material> m_WireframeMaterial;
		Ref<Material> m_OutlineMaterial, m_OutlineAnimMaterial;
		Ref<Material> m_ColliderMaterial;

		// Skybox
		Ref<VertexArray> m_SkyboxVertexArray;
		Ref<Material> m_SkyboxMaterial;

		// Uniform buffer blocks
		UniformBufferCamera m_CameraUniformBuffer;
		UniformBufferShadow m_ShadowUniformBuffer;
		UniformBufferScene m_SceneUniformBuffer;
		UniformBufferPointLights m_PointLightsUniformBuffer;
		UniformBufferRendererData m_RendererDataUniformBuffer;
		UniformBufferScreenData m_ScreenDataUniformBuffer;
		UniformBufferHBAOData m_HBAOUniformBuffer;

		// Scene information
		struct SceneInfo
		{
			Camera SceneCamera;

			Ref<Environment> SceneEnvironment;
			float SkyboxLod = 0.0f;
			float EnvironmentIntensity;
			LightEnvironment SceneLightEnvironment;
			DirectionalLight ActiveLight;
		};

		SceneInfo m_SceneData;
	};
}
