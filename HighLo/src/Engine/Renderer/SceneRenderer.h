// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-09-26) Added CompositeRenderPass
//     - 1.1 (2021-09-15) Added SetLineWidth method
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Scene/Scene.h"
#include "Engine/Camera/Camera.h"

#include "Engine/Graphics/RenderPass.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Material.h"
#include "Engine/Graphics/MaterialTable.h"
#include "Engine/Graphics/ComputePipeline.h"
#include "Engine/Graphics/CommandBuffer.h"
#include "Engine/Graphics/Shaders/UniformBufferSet.h"
#include "Engine/Graphics/Shaders/StorageBufferSet.h"
#include "Engine/Graphics/Shaders/UniformDefinitions.h"
#include "Engine/Graphics/Meshes/DynamicModel.h"
#include "Engine/Graphics/Meshes/StaticModel.h"

namespace highlo
{
	struct SceneRendererOptions
	{
		bool ShowGrid = true;

		// HBAO
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
		bool IsSelected;

		HLAPI MeshKey(AssetHandle meshHandle, AssetHandle materialHandle, uint32 index, bool isSelected)
			: MeshHandle(meshHandle), MaterialHandle(materialHandle), SubmeshIndex(index), IsSelected(isSelected)
		{
		}

		HLAPI bool operator<(const MeshKey &other) const
		{
			if (MeshHandle < other.MeshHandle)
				return true;
			
			if (MeshHandle > other.MeshHandle)
				return false;

			if (SubmeshIndex < other.SubmeshIndex)
				return true;
		
			if (SubmeshIndex > other.SubmeshIndex)
				return false;

			if (MaterialHandle < other.MaterialHandle)
				return true;
			
			if (MaterialHandle > other.MaterialHandle)
				return false;

			return IsSelected < other.IsSelected;
		}
	};

	struct TransformMapData
	{
		std::vector<TransformVertexData> Transforms;
		uint32 TransformOffset = 0;
	};

	/// <summary>
	/// Currently, the SceneRenderer is designed to be a Forward/Forward+ Renderer.
	/// This means, that all meshes and light objects are deeply connected with each other, 
	/// in pseudo code you will find something like:
	/// for mesh in meshes:
	///     for light in lights:
	///         render(mesh, light)
	/// 
	/// (actually in our real implementation, all lights are uploaded via a uniform buffer directly to the gpu, 
	///  but you will find the loop that iterates over the lights in the shader assets/shaders/Include/GLSL/Lighting.glslh)
	/// 
	/// We will provide another class in the future, that does the same job (rendering a scene) but in a more efficient way (deferred rendering).
	/// But this will also mean, that we will need to write new shaders in the future as well, while keeping all "old" shaders to still support forward rendering.
	/// 
	/// TODO: The addition of other renderers also means, that we should provide a general SceneRenderer interface, 
	///       that lets the user add models to the renderer and other standard methods, like Flush and Resize as well.
	///       In that case renderers would be exchangable as well, and a combination of multiple renderers could be used, 
	///       to gain back transparency in the case of the deferred renderer for example, like GTA V does it.
	/// 
	/// @See: For detailed information to deferred and forward rendering see https://www.youtube.com/watch?v=n5OiqJP2f7w
	/// </summary>
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

		HLAPI void SubmitStaticModel(const Ref<StaticModel> &model, const Ref<MaterialTable> &materials = nullptr, const glm::mat4 &transform = glm::mat4(1.0f), const Ref<Material> &overrideMaterial = nullptr);
		HLAPI void SubmitDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, Ref<MaterialTable> materials = nullptr, const glm::mat4 &transform = glm::mat4(1.0f), const Ref<Material> &overrideMaterial = nullptr);

		HLAPI void SubmitSelectedStaticModel(const Ref<StaticModel> &model, const Ref<MaterialTable> &materials = nullptr, const glm::mat4 &transform = glm::mat4(1.0f), const Ref<Material> &overrideMaterial = nullptr);
		HLAPI void SubmitSelectedDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials = nullptr, const glm::mat4 &transform = glm::mat4(1.0f), const Ref<Material> &overrideMaterial = nullptr);

		HLAPI void SubmitPhysicsDebugStaticModel(const Ref<StaticModel> &model, const glm::mat4 &transform = glm::mat4(1.0f));
		HLAPI void SubmitPhysicsDebugDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, const glm::mat4 &transform = glm::mat4(1.0f));

		HLAPI void ClearPass(const Ref<RenderPass> &renderPass, bool explicitClear = false);
		HLAPI void ClearPass();

		HLAPI void UpdateHBAOData();

		HLAPI SceneRendererOptions &GetOptions();

		HLAPI Ref<RenderPass> GetFinalRenderPass();
		HLAPI Ref<Texture2D> GetFinalRenderTexture();
		HLAPI const Ref<RenderPass> &GetExternalCompositeRenderPass() { return m_ExternalCompositingRenderPass; }

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
		std::map<MeshKey, DynamicDrawCommand> m_DynamicTransparentDrawList;

		std::map<MeshKey, StaticDrawCommand> m_StaticDrawList;
		std::map<MeshKey, StaticDrawCommand> m_StaticSelectedMeshDrawList;
		std::map<MeshKey, StaticDrawCommand> m_StaticShadowPassDrawList;
		std::map<MeshKey, StaticDrawCommand> m_StaticTransparentDrawList;

		// Debug draw lists
		std::map<MeshKey, StaticDrawCommand> m_StaticColliderDrawList;
		std::map<MeshKey, DynamicDrawCommand> m_DynamicColliderDrawList;
		std::map<MeshKey, TransformMapData> m_MeshTransformMap;

		TransformVertexData *m_TransformVertexData = nullptr;

		// Bloom
		Ref<Shader> m_BloomBlurShader = nullptr;
		Ref<Shader> m_BloomBlendShader = nullptr;

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
		Ref<Material> m_CompositeMaterial = nullptr;
		Ref<VertexArray> m_CompositeVertexArray = nullptr;

		// External compositing
		Ref<VertexArray> m_GeometryWireframeVertexArray = nullptr;
		Ref<VertexArray> m_GeometryWireframeOnTopVertexArray = nullptr;
		Ref<RenderPass> m_ExternalCompositingRenderPass = nullptr;

		// Light Culling
		glm::ivec3 m_LightCullingWorkGroups;
		Ref<Material> m_LightCullingMaterial = nullptr;
		Ref<ComputePipeline> m_LightCullingPipeline = nullptr;

		// Geometry
		Ref<VertexArray> m_GeometryVertexArray = nullptr;
		Ref<VertexArray> m_SelectedGeometryVertexArray = nullptr;
		Ref<Material> m_SelectedGeometryMaterial = nullptr;

		// Pre-Depth
		Ref<Material> m_PreDepthMaterial = nullptr;
		Ref<VertexArray> m_PreDepthVertexArray = nullptr;

		// HBAO
		Ref<Material> m_DeinterleavingMaterial = nullptr;
		Ref<Material> m_ReinterleavingMaterial = nullptr;
		Ref<Material> m_HBAOBlurMaterials[2];
		Ref<Material> m_HBAOMaterial = nullptr;
		Ref<VertexArray> m_DeinterleavingPipelines[2];
		Ref<VertexArray> m_ReinterleavingPipeline = nullptr;
		Ref<VertexArray> m_HBAOBlurPipelines[2];

		Ref<ComputePipeline> m_HBAOPipeline = nullptr;
		Ref<Texture2D> m_HBAOOutputImage = nullptr;
		Ref<RenderPass> m_HBAORenderPass = nullptr;
		glm::ivec3 m_HBAOWorkGroups{ 32.f, 32.f, 16.f };

		// Grid
		Ref<VertexArray> m_GridVertexArray = nullptr;
		Ref<Shader> m_GridShader = nullptr;
		Ref<Material> m_GridMaterial = nullptr;
		Ref<Material> m_WireframeMaterial = nullptr;
		Ref<Material> m_OutlineMaterial = nullptr, m_OutlineAnimMaterial = nullptr;
		Ref<Material> m_ColliderMaterial = nullptr;

		// Skybox
		Ref<VertexArray> m_SkyboxVertexArray = nullptr;
		Ref<Material> m_SkyboxMaterial = nullptr;

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

			Ref<Environment> SceneEnvironment = nullptr;
			float SkyboxLod = 0.0f;
			float EnvironmentIntensity;
			LightEnvironment SceneLightEnvironment;
			DirectionalLight ActiveLight;
		};

		SceneInfo m_SceneData;
	};
}
