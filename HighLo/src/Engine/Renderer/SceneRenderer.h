#pragma once

#include "Engine/Scene/Scene.h"
#include "Engine/ECS/Components.h"
#include "Engine/Graphics/Meshes/Mesh.h"
#include "Engine/Graphics/RenderPass.h"

#include "Engine/Graphics/Shaders/UniformBufferSet.h"
#include "Engine/Graphics/CommandBuffer.h"
#include "Engine/Graphics/ComputePipeline.h"

#include "Engine/Graphics/Shaders/StorageBufferSet.h"

#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Graphics/Texture2D.h"

namespace highlo
{
	struct SceneRendererOptions
	{
		bool ShowGrid = true;
		bool ShowSelectedInWireframe = false;

		enum class PhysicsColliderView
		{
			SelectedEntity = 0, All = 1
		};

		bool ShowPhysicsColliders = false;
		PhysicsColliderView PhysicsColliderMode = PhysicsColliderView::SelectedEntity;
		bool ShowPhysicsCollidersOnTop = false;
		glm::vec4 SimplePhysicsCollidersColor = glm::vec4{ 0.2f, 1.0f, 0.2f, 1.0f };
		glm::vec4 ComplexPhysicsCollidersColor = glm::vec4{ 0.5f, 0.5f, 1.0f, 1.0f };

		// General AO
		float AOShadowTolerance = 0.15f;

		// HBAO
		bool EnableHBAO = false;
		float HBAOIntensity = 1.5f;
		float HBAORadius = 1.0f;
		float HBAOBias = 0.35f;
		float HBAOBlurSharpness = 1.0f;

		// GTAO
		bool EnableGTAO = true;
		bool GTAOBentNormals = false;
		int GTAODenoisePasses = 4;

		// SSR
		bool EnableSSR = false;
	//	ShaderDef::AOMethod ReflectionOcclusionMethod = ShaderDef::AOMethod::None;
	};

	struct SSROptionsUB
	{
		//SSR
		glm::vec2 HZBUvFactor;
		glm::vec2 FadeIn = { 0.1f, 0.15f };
		float Brightness = 0.7f;
		float DepthTolerance = 0.8f;
		float FacingReflectionsFading = 0.1f;
		int MaxSteps = 70;
		uint32 NumDepthMips;
		float RoughnessDepthTolerance = 1.0f;
		bool HalfRes = true;
		char Padding[3]{ 0, 0, 0 };
		bool EnableConeTracing = true;
		char Padding1[3]{ 0, 0, 0 };
		float LuminanceFactor = 1.0f;
	};

	struct SceneRendererCamera
	{
		Camera Camera;
		glm::mat4 ViewMatrix;
		float Near, Far; //Non-reversed
		float FOV;
	};

	struct BloomSettings
	{
		bool Enabled = true;
		float Threshold = 1.0f;
		float Knee = 0.1f;
		float UpsampleScale = 1.0f;
		float Intensity = 1.0f;
		float DirtIntensity = 1.0f;
	};

	struct DOFSettings
	{
		bool Enabled = false;
		float FocusDistance = 0.0f;
		float BlurSize = 1.0f;
	};

	struct SceneRendererSpecification
	{
	//	Tiering::Renderer::RendererTieringSettings Tiering;
		bool Swapchain = false;
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
		struct Statistics
		{
			uint32 DrawCalls = 0;
			uint32 Meshes = 0;
			uint32 Instances = 0;
			uint32 SavedDraws = 0;

			float TotalGPUTime = 0.0f;
		};
	public:

		SceneRenderer(const Ref<Scene> &scene, const SceneRendererSpecification &specification);
		virtual ~SceneRenderer();

		void Init();
		void InitMaterials();

		void Shutdown();

		// Should only be called at initialization.
		void InitOptions();

		void SetScene(const Ref<Scene> &scene);

		void SetViewportSize(uint32 width, uint32 height);

		void UpdateHBAOData();
		void UpdateGTAOData();

		void BeginScene(const Camera &camera);
		void EndScene();

		static void InsertGPUPerfMarker(Ref<CommandBuffer> renderCommandBuffer, const std::string &label, const glm::vec4 &markerColor = {});
		static void BeginGPUPerfMarker(Ref<CommandBuffer> renderCommandBuffer, const std::string &label, const glm::vec4 &markerColor = {});
		static void EndGPUPerfMarker(Ref<CommandBuffer> renderCommandBuffer);

		void SubmitStaticModel(const Ref<StaticModel> &model, const Ref<MaterialTable> &materialTable, const glm::mat4 &transform = glm::mat4(1.0f), const Ref<Material> &overrideMaterial = nullptr);
		void SubmitDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials = nullptr, const glm::mat4 &transform = glm::mat4(1.0f), const Ref<Material> &overrideMaterial = nullptr, const std::vector<glm::mat4> &boneTransforms = {});

		void SubmitSelectedStaticModel(const Ref<StaticModel> &model, const Ref<MaterialTable> &materialTable, const glm::mat4 &transform = glm::mat4(1.0f), const Ref<Material> &overrideMaterial = nullptr);
		void SubmitSelectedDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materialTable, const glm::mat4 &transform = glm::mat4(1.0f), const Ref<Material> &overrideMaterial = nullptr, const std::vector<glm::mat4> &boneTransforms = {});

		void SubmitPhysicsDebugStaticModel(const Ref<StaticModel> &model, const glm::mat4 &transform = glm::mat4(1.0f), const bool isPrimitiveCollider = true);
		void SubmitPhysicsDebugDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, const glm::mat4 &transform = glm::mat4(1.0f));

		HLAPI const Ref<VertexArray> &GetFinalPipeline();
		HLAPI const Ref<RenderPass> &GetFinalRenderPass();
		HLAPI const Ref<RenderPass> &GetCompositeRenderPass() { return m_CompositePipeline->GetSpecification().RenderPass; }
		HLAPI const Ref<RenderPass> &GetExternalCompositeRenderPass() { return m_ExternalCompositeRenderPass; }
		HLAPI Ref<Texture2D> GetFinalPassImage();
		HLAPI const SceneRendererOptions &GetOptions();

		void SetShadowSettings(float nearPlane, float farPlane, float lambda, float scaleShadowToOrigin = 0.0f)
		{
			CascadeNearPlaneOffset = nearPlane;
			CascadeFarPlaneOffset = farPlane;
			CascadeSplitLambda = lambda;
			m_ScaleShadowCascadesToOrigin = scaleShadowToOrigin;
		}

		void SetShadowCascades(float a, float b, float c, float d)
		{
			m_UseManualCascadeSplits = true;
			m_ShadowCascadeSplits[0] = a;
			m_ShadowCascadeSplits[1] = b;
			m_ShadowCascadeSplits[2] = c;
			m_ShadowCascadeSplits[3] = d;
		}

		BloomSettings &GetBloomSettings() { return m_BloomSettings; }
		DOFSettings &GetDOFSettings() { return m_DOFSettings; }

		void SetLineWidth(float width);

		static void WaitForThreads();

		uint32 GetViewportWidth() const { return m_ViewportWidth; }
		uint32 GetViewportHeight() const { return m_ViewportHeight; }

		float GetOpacity() const { return m_Opacity; }
		void SetOpacity(float opacity) { m_Opacity = opacity; }

		const Statistics &GetStatistics() const { return m_Statistics; }

		HLAPI static Ref<SceneRenderer> Create(const Ref<Scene> &scene, const SceneRendererSpecification &specification);

	private:

		void FlushDrawList();

		void PreRender();

		struct MeshKey
		{
			AssetHandle MeshHandle;
			AssetHandle MaterialHandle;
			uint32 SubmeshIndex;
			bool IsSelected;

			MeshKey(AssetHandle meshHandle, AssetHandle materialHandle, uint32 submeshIndex, bool isSelected)
				: MeshHandle(meshHandle), MaterialHandle(materialHandle), SubmeshIndex(submeshIndex), IsSelected(isSelected)
			{
			}

			bool operator<(const MeshKey &other) const
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

		void CopyToBoneTransformStorage(const MeshKey &meshKey, const Ref<MeshFile> &meshSource, const std::vector<glm::mat4> &boneTransforms);

		void ClearPass();
		void ClearPass(Ref<RenderPass> renderPass, bool explicitClear = false);
		void DeinterleavingPass();
		void HBAOCompute();
		void GTAOCompute();

		void AOComposite();

		void GTAODenoiseCompute();

		void ReinterleavingPass();
		void HBAOBlurPass();
		void ShadowMapPass();
		void SpotShadowMapPass();
		void PreDepthPass();
		void HZBCompute();
		void PreIntegration();
		void LightCullingPass();
		void GeometryPass();
		void PreConvolutionCompute();
		void JumpFloodPass();

		// Post-processing
		void BloomCompute();
		void SSRCompute();
		void SSRCompositePass();
		void EdgeDetectionPass();
		void CompositePass();

		struct CascadeData
		{
			glm::mat4 ViewProj;
			glm::mat4 View;
			float SplitDepth;
		};
		void CalculateCascades(CascadeData *cascades, const SceneRendererCamera &sceneCamera, const glm::vec3 &lightDirection) const;
		void CalculateCascadesManualSplit(CascadeData *cascades, const SceneRendererCamera &sceneCamera, const glm::vec3 &lightDirection) const;

		void UpdateStatistics();
	
	private:

		Ref<Scene> m_Scene;
		SceneRendererSpecification m_Specification;
		Ref<CommandBuffer> m_CommandBuffer;

		struct SceneInfo
		{
			SceneRendererCamera SceneCamera;

			// Resources
			Ref<Environment> SceneEnvironment;
			float SkyboxLod = 0.0f;
			float SceneEnvironmentIntensity;
			LightEnvironment SceneLightEnvironment;
			DirectionalLight ActiveLight;
		} m_SceneData;

		struct UBCamera
		{
			// projection with near and far inverted
			glm::mat4 ViewProjection;
			glm::mat4 InverseViewProjection;
			glm::mat4 Projection;
			glm::mat4 InverseProjection;
			glm::mat4 View;
			glm::mat4 InverseView;
			glm::vec2 NDCToViewMul;
			glm::vec2 NDCToViewAdd;
			glm::vec2 DepthUnpackConsts;
			glm::vec2 CameraTanHalfFOV;
		} CameraDataUB;

		struct UBHBAOData
		{
			glm::vec4 PerspectiveInfo;
			glm::vec2 InvQuarterResolution;
			float RadiusToScreen;
			float NegInvR2;

			float NDotVBias;
			float AOMultiplier;
			float PowExponent;
			bool IsOrtho;
			char Padding0[3]{ 0, 0, 0 };

			glm::vec4 Float2Offsets[16];
			glm::vec4 Jitters[16];

			glm::vec3 Padding;
			float ShadowTolerance;
		} HBAODataUB;

		struct CBGTAOData
		{
			glm::vec2 NDCToViewMul_x_PixelSize;
			float EffectRadius = 0.5f;
			float EffectFalloffRange = 0.62f;

			float RadiusMultiplier = 1.46f;
			float FinalValuePower = 2.2f;
			float DenoiseBlurBeta = 1.2f;
			bool HalfRes = false;
			char Padding0[3]{ 0, 0, 0 };

			float SampleDistributionPower = 2.0f;
			float ThinOccluderCompensation = 0.0f;
			float DepthMIPSamplingOffset = 3.3f;
			int NoiseIndex = 0;

			glm::vec2 HZBUVFactor;
			float ShadowTolerance;
			float Padding;
		} GTAODataCB;

		struct UBScreenData
		{
			glm::vec2 InvFullResolution;
			glm::vec2 FullResolution;
			glm::vec2 InvHalfResolution;
			glm::vec2 HalfResolution;
		} ScreenDataUB;

		struct UBShadow
		{
			glm::mat4 ViewProjection[4];
		} ShadowData;

		struct DirLight
		{
			glm::vec3 Direction;
			float ShadowAmount;
			glm::vec3 Radiance;
			float Intensity;
		};

		struct UBPointLights
		{
			uint32 Count{ 0 };
			glm::vec3 Padding {};
			PointLight PointLights[1024]{};
		} PointLightsUB;

		struct UBSpotLights
		{
			uint32 Count{ 0 };
			glm::vec3 Padding{};
			SpotLight SpotLights[1000]{};
		} SpotLightUB;

		struct UBSpotShadowData
		{
			glm::mat4 ShadowMatrices[1000]{};
		} SpotShadowDataUB;

		struct UBScene
		{
			DirLight Lights;
			glm::vec3 CameraPosition;
			float EnvironmentMapIntensity = 1.0f;
		} SceneDataUB;

		struct UBRendererData
		{
			glm::vec4 CascadeSplits;
			uint32 TilesCountX{ 0 };
			bool ShowCascades = false;
			char Padding0[3] = { 0,0,0 }; // Bools are 4-bytes in GLSL
			bool SoftShadows = true;
			char Padding1[3] = { 0,0,0 };
			float Range = 0.5f;
			float MaxShadowDistance = 200.0f;
			float ShadowFade = 1.0f;
			bool CascadeFading = true;
			char Padding2[3] = { 0,0,0 };
			float CascadeTransitionFade = 1.0f;
			bool ShowLightComplexity = false;
			char Padding3[3] = { 0,0,0 };
		} RendererDataUB;

		// GTAO
		Ref<Texture2D> m_GTAOOutputImage;
		Ref<Texture2D> m_GTAODenoiseImage;
		// Points to m_GTAOOutputImage or m_GTAODenoiseImage!
		Ref<Texture2D> m_GTAOFinalImage; //TODO: WeakRef!
		Ref<Texture2D> m_GTAOEdgesOutputImage;
		Ref<ComputePipeline> m_GTAOPipeline;
		Ref<Material> m_GTAOMaterial;
		glm::uvec3 m_GTAOWorkGroups{ 1 };
		Ref<ComputePipeline> m_GTAODenoisePipeline;
		Ref<Material> m_GTAODenoiseMaterial[2]; //Ping, Pong
		Ref<Material> m_AOCompositeMaterial;
		glm::uvec3 m_GTAODenoiseWorkGroups{ 1 };
		Ref<VertexArray> m_AOCompositePipeline;
		Ref<RenderPass> m_AOCompositeRenderPass;


		//HBAO
		glm::uvec3 m_HBAOWorkGroups { 1 };
		Ref<Material> m_DeinterleavingMaterial;
		Ref<Material> m_ReinterleavingMaterial;
		Ref<Material> m_HBAOBlurMaterials[2];
		Ref<Material> m_HBAOMaterial;
		Ref<VertexArray> m_DeinterleavingPipelines[2];
		Ref<VertexArray> m_ReinterleavingPipeline;
		Ref<VertexArray> m_HBAOBlurPipelines[2];

		Ref<ComputePipeline> m_HBAOPipeline;
		Ref<Texture2D> m_HBAOOutputImage;

		Ref<Shader> m_CompositeShader;

		// Shadows
		Ref<VertexArray> m_SpotShadowPassPipeline;
		Ref<VertexArray> m_SpotShadowPassAnimPipeline;
		Ref<Material> m_SpotShadowPassMaterial;

		glm::uvec3 m_LightCullingWorkGroups;

		Ref<UniformBufferSet> m_UniformBufferSet;
		Ref<StorageBufferSet> m_StorageBufferSet;

		float LightDistance = 0.1f;
		float CascadeSplitLambda = 0.92f;
		glm::vec4 CascadeSplits;
		float CascadeFarPlaneOffset = 50.0f, CascadeNearPlaneOffset = -50.0f;
		float m_ScaleShadowCascadesToOrigin = 0.0f;
		float m_ShadowCascadeSplits[4];
		bool m_UseManualCascadeSplits = false;

		//SSR
		Ref<VertexArray> m_SSRCompositePipeline;
		Ref<ComputePipeline> m_SSRPipeline;
		Ref<ComputePipeline> m_HierarchicalDepthPipeline;
		Ref<ComputePipeline> m_GaussianBlurPipeline;
		Ref<ComputePipeline> m_PreIntegrationPipeline;
		Ref<ComputePipeline> m_SSRUpscalePipeline;
		Ref<Texture2D> m_SSRImage;
		Ref<Texture2D> m_HierarchicalDepthTexture;
		Ref<Texture2D> m_PreConvolutedTexture;
		Ref<Texture2D> m_VisibilityTexture;
		Ref<Material> m_SSRCompositeMaterial;
		Ref<Material> m_SSRMaterial;
		Ref<Material> m_GaussianBlurMaterial;
		Ref<Material> m_HierarchicalDepthMaterial;
		Ref<Material> m_PreIntegrationMaterial;
		glm::uvec3 m_SSRWorkGroups { 1 };


		glm::vec2 FocusPoint = { 0.5f, 0.5f };

		Ref<Material> m_CompositeMaterial;
		Ref<Material> m_LightCullingMaterial;

		Ref<VertexArray> m_GeometryPipeline;
		Ref<VertexArray> m_TransparentGeometryPipeline;
		Ref<VertexArray> m_GeometryPipelineAnim;

		Ref<VertexArray> m_SelectedGeometryPipeline;
		Ref<VertexArray> m_SelectedGeometryPipelineAnim;
		Ref<Material> m_SelectedGeometryMaterial;
		Ref<Material> m_SelectedGeometryMaterialAnim;

		Ref<VertexArray> m_GeometryWireframePipeline;
		Ref<VertexArray> m_GeometryWireframePipelineAnim;
		Ref<VertexArray> m_GeometryWireframeOnTopPipeline;
		Ref<VertexArray> m_GeometryWireframeOnTopPipelineAnim;
		Ref<Material> m_WireframeMaterial;

		Ref<VertexArray> m_PreDepthPipeline;
		Ref<VertexArray> m_PreDepthTransparentPipeline;
		Ref<VertexArray> m_PreDepthPipelineAnim;
		Ref<Material> m_PreDepthMaterial;

		Ref<VertexArray> m_CompositePipeline;

		Ref<VertexArray> m_ShadowPassPipelines[4];
		Ref<VertexArray> m_ShadowPassPipelinesAnim[4];

		Ref<VertexArray> m_EdgeDetectionPipeline;
		Ref<Material> m_EdgeDetectionMaterial;

		Ref<Material> m_ShadowPassMaterial;

		Ref<VertexArray> m_SkyboxPipeline;
		Ref<Material> m_SkyboxMaterial;

		Ref<VertexArray> m_DOFPipeline;
		Ref<Material> m_DOFMaterial;

		Ref<ComputePipeline> m_LightCullingPipeline;

		// Jump Flood Pass
		Ref<VertexArray> m_JumpFloodInitPipeline;
		Ref<VertexArray> m_JumpFloodPassPipeline[2];
		Ref<VertexArray> m_JumpFloodCompositePipeline;
		Ref<Material> m_JumpFloodInitMaterial, m_JumpFloodPassMaterial[2];
		Ref<Material> m_JumpFloodCompositeMaterial;

		// Bloom compute
		uint32 m_BloomComputeWorkgroupSize = 4;
		Ref<ComputePipeline> m_BloomComputePipeline;
		std::vector<Ref<Texture2D>> m_BloomComputeTextures{ 3 };
		Ref<Material> m_BloomComputeMaterial;

		struct TransformVertexData
		{
			glm::vec4 MRow[3];
		};

		struct TransformBuffer
		{
			Ref<VertexBuffer> Buffer;
			TransformVertexData *Data = nullptr;
		};

		std::vector<TransformBuffer> m_SubmeshTransformBuffers;
		using BoneTransforms = std::array<glm::mat4, 100>; // Note: 100 == MAX_BONES from the shaders
		std::vector<Ref<StorageBuffer>> m_BoneTransformStorageBuffers;
		BoneTransforms *m_BoneTransformsData = nullptr;

		std::vector<Ref<Framebuffer>> m_TempFramebuffers;

		Ref<RenderPass> m_ExternalCompositeRenderPass;

		struct DynamicDrawCommand
		{
			Ref<DynamicModel> Model;
			uint32 SubmeshIndex;
			Ref<MaterialTable> MaterialTable;
			Ref<Material> OverrideMaterial;

			uint32 InstanceCount = 0;
			uint32 InstanceOffset = 0;
			bool IsRigged = false;
		};

		struct StaticDrawCommand
		{
			Ref<StaticModel> Model;
			uint32 SubmeshIndex;
			Ref<MaterialTable> MaterialTable;
			Ref<Material> OverrideMaterial;

			uint32 InstanceCount = 0;
			uint32 InstanceOffset = 0;
		};

		struct TransformMapData
		{
			std::vector<TransformVertexData> Transforms;
			uint32 TransformOffset = 0;
		};

		struct BoneTransformsMapData
		{
			std::vector<BoneTransforms> BoneTransformsData;
			uint32 BoneTransformsBaseIndex = 0;
		};

		std::map<MeshKey, TransformMapData> m_MeshTransformMap;
		std::map<MeshKey, BoneTransformsMapData> m_MeshBoneTransformsMap;

		std::map<MeshKey, DynamicDrawCommand> m_DrawList;
		std::map<MeshKey, DynamicDrawCommand> m_TransparentDrawList;
		std::map<MeshKey, DynamicDrawCommand> m_SelectedMeshDrawList;
		std::map<MeshKey, DynamicDrawCommand> m_ShadowPassDrawList;

		std::map<MeshKey, StaticDrawCommand> m_StaticMeshDrawList;
		std::map<MeshKey, StaticDrawCommand> m_TransparentStaticMeshDrawList;
		std::map<MeshKey, StaticDrawCommand> m_SelectedStaticMeshDrawList;
		std::map<MeshKey, StaticDrawCommand> m_StaticMeshShadowPassDrawList;

		// Debug
		std::map<MeshKey, StaticDrawCommand> m_StaticColliderDrawList;
		std::map<MeshKey, DynamicDrawCommand> m_ColliderDrawList;

		// Grid
		Ref<VertexArray> m_GridPipeline;
		Ref<Material> m_GridMaterial;

		Ref<Material> m_OutlineMaterial;
		Ref<Material> m_SimpleColliderMaterial;
		Ref<Material> m_ComplexColliderMaterial;

		SceneRendererOptions m_Options;
		SSROptionsUB m_SSROptions;

		uint32 m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_InvViewportWidth = 0.f, m_InvViewportHeight = 0.f;
		bool m_NeedsResize = false;
		bool m_Active = false;
		bool m_ResourcesCreatedGPU = false;
		bool m_ResourcesCreated = false;

		float m_LineWidth = 2.0f;

		BloomSettings m_BloomSettings;
		DOFSettings m_DOFSettings;
		Ref<Texture2D> m_BloomDirtTexture;

		Ref<Texture2D> m_ReadBackImage;
		glm::vec4 *m_ReadBackBuffer = nullptr;

		float m_Opacity = 1.0f;

		struct GPUTimeQueries
		{
			uint32 DirShadowMapPassQuery = 0;
			uint32 SpotShadowMapPassQuery = 0;
			uint32 DepthPrePassQuery = 0;
			uint32 HierarchicalDepthQuery = 0;
			uint32 PreIntegrationQuery = 0;
			uint32 LightCullingPassQuery = 0;
			uint32 GeometryPassQuery = 0;
			uint32 PreConvolutionQuery = 0;
			uint32 HBAOPassQuery = 0;
			uint32 GTAOPassQuery = 0;
			uint32 GTAODenoisePassQuery = 0;
			uint32 AOCompositePassQuery = 0;
			uint32 SSRQuery = 0;
			uint32 SSRCompositeQuery = 0;
			uint32 BloomComputePassQuery = 0;
			uint32 JumpFloodPassQuery = 0;
			uint32 CompositePassQuery = 0;
		} m_GPUTimeQueries;

		Statistics m_Statistics;
	};
}
