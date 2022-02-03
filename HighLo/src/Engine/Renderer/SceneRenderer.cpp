// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

#include "HighLoPch.h"
#include "SceneRenderer.h"
#include "Renderer2D.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Threading/Thread.h"

#define USE_MULTI_THREADED_RENDERER 0

namespace highlo
{
	// Temp until we can use our own thread implementation
	static std::vector<std::thread> s_ThreadPool;

	struct UniformBufferCamera
	{
		glm::mat4 ViewProjection;
	};

	SceneRenderer::SceneRenderer(Ref<Scene> &scene, SceneRendererSpecification &specification)
		: m_Scene(scene), m_Specification(specification)
	{
		Init();
	}

	SceneRenderer::~SceneRenderer()
	{
	}

	void SceneRenderer::Init()
	{
		if (m_Specification.SwapChain)
			m_CommandBuffer = CommandBuffer::CreateFromSwapChain("SceneRenderer");
		else
			m_CommandBuffer = CommandBuffer::Create("SceneRenderer", 0);

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		m_UniformBufferSet = UniformBufferSet::Create(framesInFlight);
		m_UniformBufferSet->CreateUniform(sizeof(UniformBufferCamera), 0); // Camera Uniform block

		m_StorageBufferSet = StorageBufferSet::Create(framesInFlight);

		const size_t transformBufferCount = 100 * 1024; // 10240 transforms for now
		m_SubmeshTransformBuffer = VertexBuffer::Create(sizeof(TransformVertexData) * transformBufferCount);
		m_TransformVertexData = new TransformVertexData[transformBufferCount];

		// yea ... we have a lot of render passes to initialize in the future :)
		InitLightCullingCompute();
		InitShadowPass();
		InitPreDepthPass();
		InitGeometryPass();
		InitBloomCompute();
		InitDeinterleaving();
		InitHBAO();
		InitReinterleaving();
		InitBlurFirstPass();
		InitBlurSecondPass();
		InitCompositePass();
		InitDOF();
		InitExternalCompositePass();
		InitJumpFlood();
		InitGrid();
		InitSkybox();

		m_ResourcesCreated = true;
	}

	void SceneRenderer::SetScene(Ref<Scene> scene)
	{
		HL_ASSERT(!m_Active, "You can not set a scene while a scene is being rendered!");
		m_Scene = scene;
	}

	void SceneRenderer::SetViewportSize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}

	void SceneRenderer::SetClearColor(const glm::vec4 &color)
	{
	}

	void SceneRenderer::BeginScene(const Camera &camera)
	{
		HL_ASSERT(m_Scene);
		HL_ASSERT(!m_Active);
		m_Active = true;

		glm::mat4 viewProj = camera.GetProjection() * camera.GetViewMatrix();

		/*
		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance]()
		{
			instance->m_CompositeRenderPass->GetSpecification().Framebuffer->Bind();

			Renderer::ClearScreenColor(instance->m_CompositeRenderPass->GetSpecification().Framebuffer->GetSpecification().ClearColor);
			Renderer::ClearScreenBuffers();
		});
		*/

		// Load Camera Projection into Uniform Buffer block
		/*
		Renderer::Submit([&viewProj, &ub = m_UniformBufferSet]() mutable
		{
			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			ub->GetUniform(0, 0, frameIndex)->SetData(&viewProj, sizeof(UniformBufferCamera));
		});
		*/

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		m_UniformBufferSet->GetUniform(0, 0, frameIndex)->SetData(&viewProj, sizeof(UniformBufferCamera));

		/*
		const auto &dirLight = m_SceneData.ActiveLight;

		// calculate cascades
		CascadeData cascades[4];
		CalculateCascades(cascades, m_SceneData.SceneCamera, dirLight.Direction);

		for (uint32 i = 0; i < 4; ++i)
		{
			m_CascadeSplits[i] = cascades[i].SplitDepth;
			// TODO: Add Shadow Data structure data here
		}

		// TODO: Add shadow data structure into uniform buffer struct on CPU side
		*/
	}

	void SceneRenderer::EndScene()
	{
		HL_ASSERT(m_Active);
		m_Active = false;

	#if USE_MULTI_THREADED_RENDERER
		Ref<SceneRenderer> instance = this;
		s_ThreadPool.emplace_back([instance]() mutable
		{
			instance->FlushDrawList();
		});
	#else
		/*
		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->FlushDrawList();
		});
		*/

		FlushDrawList();
	#endif

		/*
		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance]()
		{
			instance->m_CompositeRenderPass->GetSpecification().Framebuffer->Unbind();
		});
		*/
	}

	void SceneRenderer::SubmitStaticModel(Ref<StaticModel> model, Ref<MaterialTable> materials, const glm::mat4 &transform, Ref<Material> overrideMaterial)
	{
		for (uint32 submeshIndex : model->GetSubmeshIndices())
		{
			const auto &submeshes = model->Get()->GetSubmeshes();
			uint32 materialIndex = submeshes[submeshIndex].MaterialIndex;
			glm::mat4 submeshTransform = transform * submeshes[submeshIndex].LocalTransform.GetTransform();

			// If no asset handle is available, the model probably does not have the needed material
			AssetHandle materialHandle = materials->HasMaterial(materialIndex) ? materials->GetMaterial(materialIndex)->Handle : AssetHandle();

			MeshKey key = { model->Handle, materialHandle, submeshIndex };
			auto &transformStorage = m_MeshTransformMap[key].Transforms.emplace_back();

			transformStorage.Rows[0] = { submeshTransform[0][0], submeshTransform[1][0], submeshTransform[2][0], submeshTransform[3][0] };
			transformStorage.Rows[1] = { submeshTransform[0][1], submeshTransform[1][1], submeshTransform[2][1], submeshTransform[3][1] };
			transformStorage.Rows[2] = { submeshTransform[0][2], submeshTransform[1][2], submeshTransform[2][2], submeshTransform[3][2] };

			{
				// Main geometry
				auto &dc = m_StaticDrawList[key];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.Materials = materials;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
			}

			{
				// Shadow pass
				auto &dc = m_StaticShadowPassDrawList[key];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.Materials = materials;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
			}
		}
	}

	void SceneRenderer::SubmitDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, const glm::mat4 &transform, Ref<Material> overrideMaterial)
	{
		const auto &submeshes = model->Get()->GetSubmeshes();
		uint32 materialIndex = submeshes[submeshIndex].MaterialIndex;
		AssetHandle materialHandle = materials->HasMaterial(materialIndex) ? materials->GetMaterial(materialIndex)->Handle : model->GetMaterials()->GetMaterial(materialIndex)->Handle;

		MeshKey key = { model->Handle, materialHandle, submeshIndex };
		auto &transformStorage = m_MeshTransformMap[key].Transforms.emplace_back();

		transformStorage.Rows[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
		transformStorage.Rows[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
		transformStorage.Rows[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

		{
			// Main geometry pass
			auto &dc = m_DynamicDrawList[key];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.Materials = materials;
			dc.OverrideMaterial = overrideMaterial;
			dc.InstanceCount++;
		}

		{
			// Shadow pass
			auto &dc = m_DynamicShadowPassDrawList[key];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.Materials = materials;
			dc.OverrideMaterial = overrideMaterial;
			dc.InstanceCount++;
		}
	}

	void SceneRenderer::SubmitSelectedStaticModel(Ref<StaticModel> model, Ref<MaterialTable> materials, const glm::mat4 &transform, Ref<Material> overrideMaterial)
	{
		const auto &submeshData = model->Get()->GetSubmeshes();
		for (uint32 submeshIndex : model->GetSubmeshIndices())
		{
			glm::mat4 submeshTransform = transform * submeshData[submeshIndex].LocalTransform.GetTransform();

			const auto &submeshes = model->Get()->GetSubmeshes();
			uint32 materialIndex = submeshes[submeshIndex].MaterialIndex;
			AssetHandle materialHandle = materials->HasMaterial(materialIndex) ? materials->GetMaterial(materialIndex)->Handle : model->GetMaterials()->GetMaterial(materialIndex)->Handle;

			MeshKey key = { model->Handle, materialHandle, submeshIndex };
			auto &transformStorage = m_MeshTransformMap[key].Transforms.emplace_back();

			transformStorage.Rows[0] = { submeshTransform[0][0], submeshTransform[1][0], submeshTransform[2][0], submeshTransform[3][0] };
			transformStorage.Rows[1] = { submeshTransform[0][1], submeshTransform[1][1], submeshTransform[2][1], submeshTransform[3][1] };
			transformStorage.Rows[2] = { submeshTransform[0][2], submeshTransform[1][2], submeshTransform[2][2], submeshTransform[3][2] };

			uint32 instanceIndex = 0;

			{
				// Main Geometry pass
				auto &dc = m_StaticDrawList[key];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.Materials = materials;
				dc.OverrideMaterial = overrideMaterial;
				instanceIndex = dc.InstanceCount;
				dc.InstanceCount++;
			}

			{
				// Selected geometry pass
				auto &dc = m_StaticSelectedMeshDrawList[key];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.Materials = materials;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
				dc.InstanceOffset = instanceIndex;
			}

			{
				// Shadow pass
				auto &dc = m_StaticShadowPassDrawList[key];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.Materials = materials;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
			}
		}
	}

	void SceneRenderer::SubmitSelectedDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, const glm::mat4 &transform, Ref<Material> overrideMaterial)
	{
		const auto &submeshes = model->Get()->GetSubmeshes();
		uint32 materialIndex = submeshes[submeshIndex].MaterialIndex;
		AssetHandle materialHandle = materials->HasMaterial(materialIndex) ? materials->GetMaterial(materialIndex)->Handle : model->GetMaterials()->GetMaterial(materialIndex)->Handle;

		MeshKey key = { model->Handle, materialHandle, submeshIndex };
		auto &transformStorage = m_MeshTransformMap[key].Transforms.emplace_back();

		transformStorage.Rows[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
		transformStorage.Rows[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
		transformStorage.Rows[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

		uint32 instanceIndex = 0;

		{
			// Main Geometry Pass
			auto &dc = m_DynamicDrawList[key];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.Materials = materials;
			dc.OverrideMaterial = overrideMaterial;

			instanceIndex = dc.InstanceCount;
			dc.InstanceCount++;
		}

		{
			// Selected mesh list
			auto &dc = m_DynamicSelectedMeshDrawList[key];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.Materials = materials;
			dc.OverrideMaterial = overrideMaterial;
			dc.InstanceCount++;
			dc.InstanceOffset = instanceIndex;
		}

		{
			// Shadow pass
			auto &dc = m_DynamicShadowPassDrawList[key];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.Materials = materials;
			dc.OverrideMaterial = overrideMaterial;
			dc.InstanceCount++;
		}
	}

	void SceneRenderer::SubmitPhysicsDebugStaticModel(Ref<StaticModel> model, const glm::mat4 &transform)
	{
		const auto &submeshData = model->Get()->GetSubmeshes();
		for (uint32 submeshIndex : model->GetSubmeshIndices())
		{
			glm::mat4 submeshTransform = transform * submeshData[submeshIndex].LocalTransform.GetTransform();

			// TODO: material index 42 does not exist yet
			MeshKey key = { model->Handle, 42, submeshIndex };
			auto &transformStorage = m_MeshTransformMap[key].Transforms.emplace_back();

			transformStorage.Rows[0] = { submeshTransform[0][0], submeshTransform[1][0], submeshTransform[2][0], submeshTransform[3][0] };
			transformStorage.Rows[1] = { submeshTransform[0][1], submeshTransform[1][1], submeshTransform[2][1], submeshTransform[3][1] };
			transformStorage.Rows[2] = { submeshTransform[0][2], submeshTransform[1][2], submeshTransform[2][2], submeshTransform[3][2] };

			auto &dc = m_StaticColliderDrawList[key];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.InstanceCount++;
		}
	}

	void SceneRenderer::SubmitPhysicsDebugDynamicModel(Ref<DynamicModel> model, uint32 submeshIndex, const glm::mat4 &transform)
	{
		// TODO: material index 42 does not exist yet
		MeshKey key = { model->Handle, 42, submeshIndex };
		auto &transformStorage = m_MeshTransformMap[key].Transforms.emplace_back();

		transformStorage.Rows[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
		transformStorage.Rows[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
		transformStorage.Rows[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

		auto &dc = m_DynamicColliderDrawList[key];
		dc.Model = model;
		dc.SubmeshIndex = submeshIndex;
		dc.InstanceCount++;
	}

	SceneRendererOptions &SceneRenderer::GetOptions()
	{
		return m_RendererOptions;
	}

	Ref<RenderPass> SceneRenderer::GetFinalRenderPass()
	{
		return m_CompositeRenderPass;
	}

	Ref<Texture2D> SceneRenderer::GetFinalRenderTexture()
	{
		return GetFinalRenderPass()->GetSpecification().Framebuffer->GetImage();
	}

	void SceneRenderer::OnUIRender()
	{
	}

	void SceneRenderer::WaitForThreads()
	{
		for (uint32 i = 0; i < s_ThreadPool.size(); ++i)
			s_ThreadPool[i].join();

		s_ThreadPool.clear();
	}

	void SceneRenderer::FlushDrawList()
	{
		if (m_ResourcesCreated && m_ViewportWidth > 0 && m_ViewportHeight > 0)
		{
			PreRender();

			m_CommandBuffer->Begin();

			// Main Render passes
		//	ShadowMapPass();
		//	PreDepthPass();
		//	LightCullingPass();
			GeometryPass();

			// HBAO
		//	DeinterleavingPass();
		//	HBAOPass();
		//	ReinterleavingPass();
		//	HBAOBlurPass();

			// Post-processing
		//	JumpFloodPass();
		//	BloomCompute();
			CompositePass();

			m_CommandBuffer->End();
			m_CommandBuffer->Submit();
		}
		else
		{
			// Draw empty pass
			m_CommandBuffer->Begin();
			ClearPass();
			m_CommandBuffer->End();
			m_CommandBuffer->Submit();
		}

		UpdateStatistics();

		m_DynamicDrawList.clear();
		m_DynamicSelectedMeshDrawList.clear();
		m_DynamicShadowPassDrawList.clear();

		m_StaticDrawList.clear();
		m_StaticSelectedMeshDrawList.clear();
		m_StaticShadowPassDrawList.clear();

		m_DynamicColliderDrawList.clear();
		m_StaticColliderDrawList.clear();

		m_SceneData = {};
		m_MeshTransformMap.clear();
	}

	void SceneRenderer::PreRender()
	{
		uint32 offset = 0;
		for (auto &[key, transformData] : m_MeshTransformMap)
		{
			transformData.TransformOffset = offset * sizeof(TransformVertexData);
			for (const auto &transform : transformData.Transforms)
			{
				m_TransformVertexData[offset] = transform;
				++offset;
			}
		}

		m_SubmeshTransformBuffer->UpdateContents(m_TransformVertexData, offset * sizeof(TransformVertexData));
	}

	void SceneRenderer::ClearPass()
	{
		Renderer::BeginRenderPass(m_CommandBuffer, m_CompositeRenderPass, true);
		Renderer::EndRenderPass(m_CommandBuffer);
	}

	void SceneRenderer::ClearPass(Ref<RenderPass> renderPass, bool explicitClear)
	{
		Renderer::BeginRenderPass(m_CommandBuffer, renderPass, explicitClear);
		Renderer::EndRenderPass(m_CommandBuffer);
	}

	void SceneRenderer::DeinterleavingPass()
	{
	}

	void SceneRenderer::HBAOPass()
	{
	}

	void SceneRenderer::ReinterleavingPass()
	{
	}

	void SceneRenderer::HBAOBlurPass()
	{
	}

	void SceneRenderer::ShadowMapPass()
	{
	}

	void SceneRenderer::PreDepthPass()
	{
	}

	void SceneRenderer::LightCullingPass()
	{
	}

	void SceneRenderer::GeometryPass()
	{
		// Render selected geometry
		Renderer::BeginRenderPass(m_CommandBuffer, m_SelectedGeometryVertexArray->GetSpecification().RenderPass);

		for (auto &[mk, dc] : m_StaticSelectedMeshDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			//	Renderer::RenderInstancedStaticMeshWithMaterial(m_CommandBuffer, m_SelectedGeometryVertexArray, m_UniformBufferSet, nullptr, dc.Model, dc.SubmeshIndex, m_SubmeshTransformBuffer, transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData), dc.InstanceCount, m_SelectedGeometryMaterial);
		}

		for (auto &[mk, dc] : m_DynamicSelectedMeshDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			//	Renderer::RenderInstancedDynamicMeshWithMaterial(m_CommandBuffer, m_SelectedGeometryVertexArray, m_UniformBufferSet, nullptr, dc.Model, dc.SubmeshIndex, transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData), dc.InstanceCount, m_SelectedGeometryMaterial);
		}

		Renderer::EndRenderPass(m_CommandBuffer);

		// Render normal geometry
		Renderer::BeginRenderPass(m_CommandBuffer, m_GeometryVertexArray->GetSpecification().RenderPass);

		// First render skybox
	//	m_SkyboxMaterial->Set("u_Uniforms.TextureLod", m_SceneData.SkyboxLod);
	//	m_SkyboxMaterial->Set("u_Uniforms.Intensity", m_SceneData.EnvironmentIntensity);

	//	const Ref<Texture3D> radianceMap = m_SceneData.SceneEnvironment ? m_SceneData.SceneEnvironment->GetRadianceMap() : Renderer::GetBlackCubeTexture();
	//	m_SkyboxMaterial->Set("u_Texture", radianceMap);
	//	Renderer::RenderQuad(m_CommandBuffer, m_SkyboxVertexArray, m_UniformBufferSet, nullptr, m_SkyboxMaterial);

		// Now render static and dynamic meshes
		for (auto &[mk, dc] : m_StaticDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			Renderer::RenderInstancedStaticMesh(m_CommandBuffer, m_GeometryVertexArray, m_UniformBufferSet, m_StorageBufferSet, dc.Model, dc.SubmeshIndex, dc.Materials ? dc.Materials : dc.Model->GetMaterials(), m_SubmeshTransformBuffer, transformData.TransformOffset, dc.InstanceCount);
		}

		for (auto &[mk, dc] : m_DynamicDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			//	Renderer::RenderInstancedDynamicMesh(m_CommandBuffer, m_GeometryVertexArray, m_UniformBufferSet, m_StorageBufferSet, dc.Model, dc.SubmeshIndex, dc.Materials ? dc.Materials : dc.Model->GetMaterials(), m_SubmeshTransformBuffer, transformData.TransformOffset, dc.InstanceCount);
		}

		// Grid
		if (GetOptions().ShowGrid)
		{
			const glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), { 1.0f, 0.0f, 0.0f })
				* glm::scale(glm::mat4(1.0f), glm::vec3(8.0f));

			Renderer::RenderQuad(m_CommandBuffer, m_GridVertexArray, m_UniformBufferSet, nullptr, m_GridMaterial, transform);
		}

		Renderer::EndRenderPass(m_CommandBuffer);
	}

	void SceneRenderer::JumpFloodPass()
	{
	}

	void SceneRenderer::BloomCompute()
	{
	}

	void SceneRenderer::CompositePass()
	{
		Renderer::BeginRenderPass(m_CommandBuffer, m_CompositeRenderPass, true);
		Renderer::EndRenderPass(m_CommandBuffer);
	}

	void SceneRenderer::UpdateStatistics()
	{
	}

	void SceneRenderer::CalculateCascades(CascadeData *data, const Camera &sceneCamera, const glm::vec3 &lightDir) const
	{
		glm::mat4 viewProjection = sceneCamera.GetProjection() * sceneCamera.GetViewMatrix();
		const int32 SHADOW_MAP_CASCADE_COUNT = 4;
		float cascadeSplits[SHADOW_MAP_CASCADE_COUNT];

		glm::vec3 frustumCorners[8] =
		{
			glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(1.0f,  1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(-1.0f,  1.0f,  1.0f),
			glm::vec3(1.0f,  1.0f,  1.0f),
			glm::vec3(1.0f, -1.0f,  1.0f),
			glm::vec3(-1.0f, -1.0f,  1.0f),
		};

		float nearClip = 0.0f;
		float farClip = 0.0f;

		if (sceneCamera.IsPerspective())
		{
			nearClip = sceneCamera.GetPerspectiveNearPlane();
			farClip = sceneCamera.GetPerspectiveFarPlane();
		}
		else
		{
			nearClip = sceneCamera.GetOrthographicNearPlane();
			farClip = sceneCamera.GetOrthographicFarPlane();
		}

		float clipRange = farClip - nearClip;
		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

		// Calculate split depths based on view camera frustum (@see https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html)
		for (uint32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; ++i)
		{
			float p = (i + 1) / static_cast<float>(SHADOW_MAP_CASCADE_COUNT);
			float log = minZ * std::pow(ratio, p);
			float uniform = minZ + range * p;
			float d = m_CascadeSplitLambda * (log - uniform) + uniform;
			cascadeSplits[i] = (d - nearClip) / clipRange;
		}
		cascadeSplits[3] = 0.3f;

		// Calculate orthographic projection matrix for each cascade
		float lastSplitDistance = 0.0f;
		for (uint32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; ++i)
		{
			float splitDist = cascadeSplits[i];

			// Project frustum corners into world space
			glm::mat4 inverseCameraProj = glm::inverse(viewProjection);
			for (uint32 j = 0; j < 8; ++j)
			{
				glm::vec4 inverseCorner = inverseCameraProj * glm::vec4(frustumCorners[j], 1.0f);
				frustumCorners[j] = inverseCorner / inverseCorner.w;
			}

			for (uint32 j = 0; j < 4; ++j)
			{
				glm::vec3 dist = frustumCorners[j + 4] - frustumCorners[j];
				frustumCorners[j + 4] = frustumCorners[j] + (dist * splitDist);
				frustumCorners[j] += dist * lastSplitDistance;
			}

			// Get frustum center
			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (uint32 j = 0; j < 8; ++j)
				frustumCenter += frustumCorners[j];

			frustumCenter /= 8.0f;

			float radius = 0.0f;
			for (uint32 j = 0; j < 8; ++j)
			{
				float distance = glm::length(frustumCorners[j] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = glm::ceil(radius * 16.0f) / 16.0f;

			glm::vec3 maxExtents = glm::vec3(radius);
			glm::vec3 minExtents = -maxExtents;

			glm::vec3 lightDirection = -lightDir;
			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDirection * -minExtents.z, frustumCenter, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, m_CascadeNearPlaneOffset, maxExtents.z - minExtents.z + m_CascadeFarPlaneOffset);

			// Offset to texel space to avoid shimmering (@see https://stackoverflow.com/questions/33499053/cascaded-shadow-map-shimmering)
			glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
			float shadowMapRes = (float)m_ShadowPassVertexArrays[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetWidth();
			glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * shadowMapRes / 2.0f;
			glm::vec4 roundedOrigin = glm::round(shadowOrigin);
			glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset = roundOffset * 2.0f / shadowMapRes;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;

			lightOrthoMatrix[3] += roundOffset;

			data[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
			data[i].ViewProjection = lightOrthoMatrix * lightViewMatrix;
			data[i].View = lightViewMatrix;

			lastSplitDistance = cascadeSplits[i];
		}
	}

	void SceneRenderer::InitLightCullingCompute()
	{
	}

	void SceneRenderer::InitShadowPass()
	{
		/*
		TextureSpecification imageSpec;
		imageSpec.Format = TextureFormat::DEPTH32F;
		imageSpec.Usage = TextureUsage::Attachment;
		imageSpec.Width = 4096;
		imageSpec.Height = 4096;
		imageSpec.Layers = 4; // because we will use 4 cascades
		Ref<Texture2D> cascadeDepthImage = Texture2D::CreateFromSpecification(imageSpec);
		cascadeDepthImage->Invalidate();
		cascadeDepthImage->CreatePerLayerImageViews();

		FramebufferSpecification shadowMapFramebufferSpec;
		shadowMapFramebufferSpec.DebugName = "ShadowMap";
		shadowMapFramebufferSpec.Width = 4096;
		shadowMapFramebufferSpec.Height = 4096;
		shadowMapFramebufferSpec.Attachments = { TextureFormat::DEPTH32F };
		shadowMapFramebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		shadowMapFramebufferSpec.NoResize = true;
		shadowMapFramebufferSpec.ExistingImage = cascadeDepthImage;

		auto shader = Renderer::GetShaderLibrary()->Get("ShadowMap");
		VertexArraySpecification spec;
		spec.DebugName = "ShadowPass";
		spec.Shader = shader;
		spec.Layout = BufferLayout::GetShadowMapLayout();
		spec.InstanceLayout = {};

		for (int32 i = 0; i < 4; ++i)
		{
			shadowMapFramebufferSpec.ExistingImageLayers.clear();
			shadowMapFramebufferSpec.ExistingImageLayers.emplace_back(i);

			RenderPassSpecification shadowMapRenderPassSpec;
			shadowMapRenderPassSpec.DebugName = "ShadowMap";
			shadowMapRenderPassSpec.Framebuffer = Framebuffer::Create(shadowMapFramebufferSpec);
			spec.RenderPass = RenderPass::Create(shadowMapRenderPassSpec);
			m_ShadowPassVertexArrays[i] = VertexArray::Create(spec);
		}
		m_ShadowPassMaterial = Material::Create(shader, "ShadowPassMaterial");
		*/
	}

	void SceneRenderer::InitPreDepthPass()
	{
	}

	void SceneRenderer::InitGeometryPass()
	{
		FramebufferSpecification frameBufferSpec;
		frameBufferSpec.DebugName = "Geometry";
		frameBufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::RGBA16F, TextureFormat::RGBA16F, TextureFormat::Depth };
		frameBufferSpec.Samples = 1;
		frameBufferSpec.ClearColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		RenderPassSpecification renderPassSpec;
		renderPassSpec.DebugName = "Geometry";
		renderPassSpec.Framebuffer = Framebuffer::Create(frameBufferSpec);

		VertexArraySpecification spec;
		spec.DebugName = "PBR-Static";
		spec.LineWidth = Renderer::GetCurrentLineWidth();
		spec.Layout = BufferLayout::GetStaticShaderLayout();
		spec.InstanceLayout = {};
		spec.Shader = Renderer::GetShaderLibrary()->Get("HighLoPBR");
		spec.RenderPass = RenderPass::Create(renderPassSpec);
		m_GeometryVertexArray = VertexArray::Create(spec);

		// Selected Geometry pipeline
		FramebufferSpecification selectedFrameBufferSpec;
		selectedFrameBufferSpec.DebugName = frameBufferSpec.DebugName;
		selectedFrameBufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::Depth };
		selectedFrameBufferSpec.Samples = 1;
		selectedFrameBufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };

		RenderPassSpecification selectedRenderPassSpec;
		selectedRenderPassSpec.DebugName = renderPassSpec.DebugName;
		selectedRenderPassSpec.Framebuffer = Framebuffer::Create(selectedFrameBufferSpec);

		VertexArraySpecification selectedSpec;
		selectedSpec.DebugName = "SelectedGeometry";
		selectedSpec.Layout = BufferLayout::GetStaticShaderLayout();
		selectedSpec.InstanceLayout = {};
		selectedSpec.RenderPass = RenderPass::Create(selectedRenderPassSpec);
		selectedSpec.Shader = Renderer::GetShaderLibrary()->Get("SelectedGeometry");
		m_SelectedGeometryVertexArray = VertexArray::Create(selectedSpec);
		m_SelectedGeometryMaterial = Material::Create(selectedSpec.Shader, "SelectedGeometryMaterial");
	}

	void SceneRenderer::InitBloomCompute()
	{
	}

	void SceneRenderer::InitDeinterleaving()
	{
	}

	void SceneRenderer::InitHBAO()
	{
	}

	void SceneRenderer::InitReinterleaving()
	{
	}

	void SceneRenderer::InitBlurFirstPass()
	{
	}

	void SceneRenderer::InitBlurSecondPass()
	{
	}

	void SceneRenderer::InitGrid()
	{
		m_GridShader = Renderer::GetShaderLibrary()->Get("Grid");
		m_GridMaterial = Material::Create(m_GridShader, "GridMaterial");
		float gridScale = 16.025f;
		float gridSize = 0.025f;

		m_GridMaterial->Set("u_Settings.Scale", gridScale);
		m_GridMaterial->Set("u_Settings.Size", gridSize);

		VertexArraySpecification spec;
		spec.DebugName = "Grid";
		spec.Layout = BufferLayout::GetGridLayout();
		spec.Shader = m_GridShader;
		spec.BackfaceCulling = false;
		spec.RenderPass = m_GeometryVertexArray->GetSpecification().RenderPass;
		m_GridVertexArray = VertexArray::Create(spec);
	}

	void SceneRenderer::InitSkybox()
	{
		auto shader = Renderer::GetShaderLibrary()->Get("Skybox");

		VertexArraySpecification spec;
		spec.DebugName = "Skybox";
		spec.Layout = BufferLayout::GetSkyboxLayout();
		spec.InstanceLayout = {};
		spec.Shader = shader;
		spec.RenderPass = m_GeometryVertexArray->GetSpecification().RenderPass;
		m_SkyboxVertexArray = VertexArray::Create(spec);

		m_SkyboxMaterial = Material::Create(shader, "SkyboxMaterial");
		m_SkyboxMaterial->SetFlag(MaterialFlag::DepthTest, false);
	}

	void SceneRenderer::InitCompositePass()
	{
		FramebufferSpecification framebufferSpec;
		framebufferSpec.DebugName = "SceneComposite";
		framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		framebufferSpec.SwapChainTarget = m_Specification.SwapChain;

		if (m_Specification.SwapChain)
			framebufferSpec.Attachments = { TextureFormat::RGBA };
		else
			framebufferSpec.Attachments = { TextureFormat::RGBA, TextureFormat::Depth };

		RenderPassSpecification renderPassSpec;
		renderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
		renderPassSpec.DebugName = "SceneComposite";
		m_CompositeRenderPass = RenderPass::Create(renderPassSpec);
	}

	void SceneRenderer::InitDOF()
	{
	}

	void SceneRenderer::InitExternalCompositePass()
	{
	}

	void SceneRenderer::InitJumpFlood()
	{
	}


}
