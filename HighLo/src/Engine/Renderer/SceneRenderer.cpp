// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

#include "HighLoPch.h"
#include "SceneRenderer.h"
#include "Renderer2D.h"

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	namespace utils
	{
		static uint32 GetAttachmentIndexFromCompositeFramebuffer(TextureFormat format)
		{
			uint32 attachmentIndex = 0;
			switch (format)
			{
				case TextureFormat::RGBA:
					attachmentIndex = 0;
					break;

				case TextureFormat::RED_INTEGER:
					attachmentIndex = 1;
					break;

				case TextureFormat::Depth:
					attachmentIndex = 2;
					break;
			}

			return attachmentIndex;
		}
	}

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

		// yea ... we have a lot of render passes to initialize in the future :)
		InitLightCullingCompute();
		InitShadowPass();
		InitPreDepthPass();
		InitGeometryPass();
		InitOutlinePass();
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
	}

	void SceneRenderer::SetScene(Ref<Scene> scene)
	{
		m_Scene = scene;
	}

	void SceneRenderer::SetViewportSize(uint32 width, uint32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}

	void SceneRenderer::SetClearColor(const glm::vec4 &color)
	{
		m_CompositeRenderPass->GetSpecification().Framebuffer->GetSpecification().ClearColor = color;
	}

	void SceneRenderer::BeginScene(const Camera &camera)
	{
		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance, camera]()
		{
			instance->m_CompositeRenderPass->GetSpecification().Framebuffer->Bind();

			Renderer::ClearScreenColor(instance->m_CompositeRenderPass->GetSpecification().Framebuffer->GetSpecification().ClearColor);
			Renderer::ClearScreenBuffers();
		});
	}

	void SceneRenderer::EndScene()
	{
		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance]()
		{
			instance->m_CompositeRenderPass->GetSpecification().Framebuffer->Unbind();
		});
	}

	void SceneRenderer::ClearPixelBuffer(TextureFormat format, int32 value)
	{
		Ref<Framebuffer> framebuffer = m_CompositeRenderPass->GetSpecification().Framebuffer;
		framebuffer->ClearAttachment(utils::GetAttachmentIndexFromCompositeFramebuffer(format), value);
	}

	int32 SceneRenderer::GetPixel(TextureFormat format, int32 x, int32 y) const
	{
		Ref<Framebuffer> framebuffer = m_CompositeRenderPass->GetSpecification().Framebuffer;
		int32 pixelData = framebuffer->ReadPixel(utils::GetAttachmentIndexFromCompositeFramebuffer(format), x, y);
		return pixelData;
	}

	bool SceneRenderer::PixelSelected(TextureFormat format, int32 x, int32 y) const
	{
		return GetPixel(format, x, y) != -1;
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
		return m_CompositeRenderPass->GetSpecification().Framebuffer->GetImage();
	}

	void SceneRenderer::OnUIRender()
	{
	}

	void SceneRenderer::WaitForThreads()
	{
	}

	void SceneRenderer::FlushDrawList()
	{
		if (m_ResourcesCreated && m_ViewportWidth > 0 && m_ViewportHeight > 0)
		{
			PreRender();

			m_CommandBuffer->Begin();

			// Main Render passes
			ShadowMapPass();
			PreDepthPass();
			LightCullingPass();
			GeometryPass();

			// HBAO
			DeinterleavingPass();
			HBAOPass();
			ReinterleavingPass();
			HBAOBlurPass();

			// Post-processing
			JumpFloodPass();
			BloomCompute();
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
	}

	void SceneRenderer::ClearPass()
	{
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
	}

	void SceneRenderer::JumpFloodPass()
	{
	}

	void SceneRenderer::BloomCompute()
	{
	}

	void SceneRenderer::CompositePass()
	{
	}

	void SceneRenderer::ClearPass(Ref<RenderPass> renderPass, bool explicitClear)
	{
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
			float shadowMapRes = 0.0f; // m_ShadowPassPipelines[0]->GetSpecification().RenderPass->GetSpecification().TargetFramebuffer->GetWidth();
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
	}

	void SceneRenderer::InitPreDepthPass()
	{
	}

	void SceneRenderer::InitGeometryPass()
	{
	}

	void SceneRenderer::InitOutlinePass()
	{
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
		m_GridShader = Renderer::GetShaderLibrary()->Get("GridShader");
		m_GridMaterial = Material::Create(m_GridShader, "GridMaterial");
		float gridScale = 16.025f;
		float gridSize = 0.025f;

	//	m_GridMaterial->Set("u_Scale", gridScale);
	//	m_GridMaterial->Set("u_Size", gridSize);
	}

	void SceneRenderer::InitSkybox()
	{

	}

	void SceneRenderer::InitCompositePass()
	{
		FramebufferSpecification framebufferSpec;
		framebufferSpec.DebugName = "SceneComposite";
		framebufferSpec.ClearColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		framebufferSpec.SwapChainTarget = m_Specification.SwapChain;

		if (m_Specification.SwapChain)
			framebufferSpec.Attachments = { TextureFormat::RGBA };
		else
			framebufferSpec.Attachments = { TextureFormat::RGBA, TextureFormat::RED_INTEGER, TextureFormat::Depth };

		Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);

		RenderPassSpecification renderPassSpec;
		renderPassSpec.Framebuffer = framebuffer;
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
