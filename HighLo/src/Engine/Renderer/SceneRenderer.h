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

	class SceneRenderer : public IsSharedReference
	{
	public:

		HLAPI SceneRenderer(Ref<Scene> &scene, SceneRendererSpecification &specification = SceneRendererSpecification());
		HLAPI virtual ~SceneRenderer();

		HLAPI void Init();

		HLAPI void SetScene(Ref<Scene> scene);
		HLAPI void SetViewportSize(uint32 width, uint32 height);
		HLAPI void SetClearColor(const glm::vec4 &color);
		HLAPI void SetLineWidth(float width) { m_LineWidth = width; }

		HLAPI void BeginScene(const Camera &camera);
		HLAPI void EndScene();

		HLAPI void SubmitStaticModel(Ref<StaticModel> model, Ref<MaterialTable> materials, const glm::mat4 &transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);
		HLAPI void SubmitDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, const glm::mat4 &transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);

		HLAPI void SubmitSelectedStaticModel(Ref<StaticModel> model, Ref<MaterialTable> materials, const glm::mat4 &transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);
		HLAPI void SubmitSelectedDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, const glm::mat4 &transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);

		HLAPI void SubmitPhysicsDebugStaticModel(Ref<StaticModel> model, const glm::mat4 &transform = glm::mat4(1.0f));
		HLAPI void SubmitPhysicsDebugDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, const glm::mat4 &transform = glm::mat4(1.0f));

		HLAPI void ClearPixelBuffer(TextureFormat format, int32 value);
		HLAPI int32 GetPixel(TextureFormat format, int32 x, int32 y) const;
		HLAPI bool PixelSelected(TextureFormat format, int32 x, int32 y) const;

		HLAPI void ClearPass(Ref<RenderPass> renderPass, bool explicitClear = false);
		HLAPI void ClearPass();

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
		void InitOutlinePass();
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
		float m_LineWidth = 2.0f;
		bool m_ResourcesCreated = false;

		Ref<Scene> m_Scene;
		SceneRendererSpecification m_Specification;
		SceneRendererOptions m_RendererOptions;

		Ref<RenderPass> m_CompositeRenderPass;
		Ref<RenderPass> m_ExternalCompositeRenderPass;

		Ref<Shader> m_CompositeShader;
		Ref<Shader> m_BloomBlurShader;
		Ref<Shader> m_BloomBlendShader;

		glm::ivec3 m_HBAOWorkGroups{ 32.f, 32.f, 16.f };
		glm::ivec3 m_LightCullingWorkGroups;

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

		Ref<Material> m_CompositeMaterial;
		Ref<Material> m_LightCullingMaterial;

		// Geometry
		Ref<VertexArray> m_GeometryVertexArray;
		Ref<VertexArray> m_SelectedGeometryVertexArray;
		Ref<Material> m_SelectedGeometryMaterial;

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
