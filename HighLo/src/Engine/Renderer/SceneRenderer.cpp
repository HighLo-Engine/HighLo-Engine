// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SceneRenderer.h"

#include "Engine/Renderer/Renderer.h"

#include "Engine/Graphics/RenderPassBuilder.h"
#include "Engine/Graphics/FramebufferBuilder.h"
#include "Engine/Graphics/VertexArrayBuilder.h"

namespace highlo
{
	struct SceneRendererData
	{

	};

	static SceneRendererData *s_RendererData = nullptr;

	SceneRenderer::SceneRenderer(Ref<Scene> &scene, SceneRendererSpecification &specification)
		: m_Specification(specification), m_Scene(scene)
	{
		Init();
	}

	SceneRenderer::~SceneRenderer()
	{
		Shutdown();
	}

	void SceneRenderer::Init()
	{
		if (m_Specification.SwapChain)
			m_CommandBuffer = CommandBuffer::CreateFromSwapChain("SceneRenderer");
		else
			m_CommandBuffer = CommandBuffer::Create("SceneRenderer");

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		m_UniformBufferSet = UniformBufferSet::Create(framesInFlight);
		m_UniformBufferSet->CreateUniform(sizeof(UniformBufferCamera), 0, UniformLayout::GetCameraLayout()); // Camera Uniform block
		m_UniformBufferSet->CreateUniform(sizeof(UniformBufferShadow), 1, UniformLayout::GetShadowDataLayout()); // Shadow Uniform block
		m_UniformBufferSet->CreateUniform(sizeof(UniformBufferScene), 2, UniformLayout::GetSceneDataLayout()); // Scene Uniform block
		m_UniformBufferSet->CreateUniform(sizeof(UniformBufferRendererData), 3, UniformLayout::GetRendererDataLayout()); // Renderer Data Uniform block
		m_UniformBufferSet->CreateUniform(sizeof(UniformBufferPointLights), 4, UniformLayout::GetPointLightDataLayout()); // PointLights Uniform block
		m_UniformBufferSet->CreateUniform(sizeof(UniformBufferScreenData), 17, UniformLayout::GetScreenDataLayout()); // Screen data Uniform block
		m_UniformBufferSet->CreateUniform(sizeof(UniformBufferHBAOData), 18, UniformLayout::GetHBAODataLayout()); // HBAO data Uniform block

		// Init geometry render pass
		{
			FramebufferSpecification framebufferSpec;
			framebufferSpec.DebugName = "Geometry";
			framebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::RGBA16F, TextureFormat::RGBA, TextureFormat::DEPTH32FSTENCIL8UINT };
			framebufferSpec.Samples = 1;
			framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

			RenderPassSpecification renderPassSpec;
			renderPassSpec.DebugName = "Geometry";
			renderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
			m_GeometryRenderPass = RenderPass::Create(renderPassSpec);

			auto &pbrShader = Renderer::GetShaderLibrary()->Get("HighLoPBR");
			m_GeometryMaterial = Material::Create(pbrShader, "GeometryMaterial");

			VertexArraySpecification vaoSpec;
			vaoSpec.DebugName = "Geometry";
			vaoSpec.LineWidth = Renderer::GetCurrentLineWidth();
			vaoSpec.Shader = pbrShader;
			vaoSpec.RenderPass = m_GeometryRenderPass;
			vaoSpec.Layout = BufferLayout::GetStaticShaderLayout();
			vaoSpec.InstanceLayout = BufferLayout::GetTransformBufferLayout();
			m_GeometryVertexArray = VertexArray::Create(vaoSpec);
		}

		// Init composite renderpass
		{
			FramebufferSpecification framebufferSpec;
			framebufferSpec.DebugName = "Composite";
			framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
			framebufferSpec.SwapChainTarget = m_Specification.SwapChain;
			framebufferSpec.Attachments = { TextureFormat::RGBA, TextureFormat::Depth };

			if (m_Specification.SwapChain)
				framebufferSpec.Attachments = { TextureFormat::RGBA };

			RenderPassSpecification renderPassSpec;
			renderPassSpec.DebugName = "Composite";
			renderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
			m_FinalCompositeRenderPass = RenderPass::Create(renderPassSpec);

			auto &shader = Renderer::GetShaderLibrary()->Get("SceneComposite");
			m_CompositeMaterial = Material::Create(shader, "CompositeMaterial");

			VertexArraySpecification vaoSpec;
			vaoSpec.DebugName = "Composite";
			vaoSpec.Layout = BufferLayout::GetCompositeLayout();
			vaoSpec.InstanceLayout = {};
			vaoSpec.Shader = shader;
			vaoSpec.RenderPass = m_FinalCompositeRenderPass;
			m_FinalVertexArray = VertexArray::Create(vaoSpec);
		}

		// Init external renderpass
		{
			FramebufferSpecification framebufferSpec;
			framebufferSpec.DebugName = "ExternalComposite";
			framebufferSpec.Attachments = { TextureFormat::RGBA, TextureFormat::DEPTH32FSTENCIL8UINT };
			framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
			framebufferSpec.ClearOnLoad = true;
			framebufferSpec.ExistingImages[0] = m_FinalCompositeRenderPass->GetSpecification().Framebuffer->GetImage().As<Texture2D>();
			framebufferSpec.ExistingImages[1] = m_GeometryRenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>();

			RenderPassSpecification renderPassSpec;
			renderPassSpec.DebugName = "ExternalComposite";
			renderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
			m_ExternalCompositingRenderPass = RenderPass::Create(renderPassSpec);

			auto &shader = Renderer::GetShaderLibrary()->Get("Wireframe");
			m_WireframeMaterial = Material::Create(shader, "WireframeMaterial");

			VertexArraySpecification vaoSpec;
			vaoSpec.DebugName = "ExternalComposite";
			vaoSpec.Shader = shader;
			vaoSpec.RenderPass = m_ExternalCompositingRenderPass;
			vaoSpec.Layout = BufferLayout::GetStaticShaderLayout();
			vaoSpec.InstanceLayout = BufferLayout::GetTransformBufferLayout();
			m_ExternalVertexArray = VertexArray::Create(vaoSpec);
			
			vaoSpec.Wireframe = true;
			vaoSpec.DepthTest = true;
			vaoSpec.LineWidth = 2.0f;
			m_GeometryWireframeVertexArray = VertexArray::Create(vaoSpec);
			vaoSpec.DepthTest = false;
			m_GeometryWireframeOnTopVertexArray = VertexArray::Create(vaoSpec);
		}

		m_ResourcesCreated = true;
	}

	void SceneRenderer::Shutdown()
	{
		delete s_RendererData;
		s_RendererData = nullptr;
	}

	void SceneRenderer::SetScene(const Ref<Scene> &scene)
	{
		HL_ASSERT(!m_Active, "You can not set a scene while a scene is being rendered!");
		m_Scene = scene;
	}

	void SceneRenderer::SetViewportSize(uint32 width, uint32 height)
	{
		if (m_ViewportWidth != width || m_ViewportHeight != height)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			m_InvertedViewportWidth = 1.0f / (float)width;
			m_InvertedViewportHeight = 1.0f / (float)height;
			m_NeedsResize = true;
		}
	}

	void SceneRenderer::SetClearColor(const glm::vec4 &color)
	{
	}

	void SceneRenderer::BeginScene(const Camera &camera)
	{
		HL_ASSERT(m_Scene);
		HL_ASSERT(!m_Active);

		if (!m_ResourcesCreated)
			return;

		m_Active = true;
		if (m_NeedsResize)
		{
			m_NeedsResize = false;
			m_GeometryVertexArray->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			m_FinalVertexArray->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			m_ExternalCompositingRenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

			if (m_Specification.SwapChain)
				m_CommandBuffer = CommandBuffer::CreateFromSwapChain("SceneRenderer");
		}

		auto &sceneCamera = m_SceneData.SceneCamera;
		const auto viewProjection = sceneCamera.GetViewProjectionMatrix();
		const glm::vec3 cameraPosition = glm::inverse(sceneCamera.GetViewMatrix())[3];

		UniformBufferCamera &cameraData = m_CameraUniformBuffer;
		cameraData.ViewProjection = sceneCamera.GetProjection() * sceneCamera.GetViewMatrix();
		cameraData.InverseViewProjection = glm::inverse(viewProjection);
		cameraData.Projection = sceneCamera.GetProjection();
		cameraData.View = sceneCamera.GetViewMatrix();

		// Load Camera Projection into Uniform Buffer block
		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance, cameraData]() mutable
		{
			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			instance->m_UniformBufferSet->GetUniform(0, 0, frameIndex)->SetData(&cameraData, sizeof(cameraData));
		});

		UniformBufferScreenData &screenData = m_ScreenDataUniformBuffer;
		screenData.FullResolution = { m_ViewportWidth, m_ViewportHeight };
		screenData.HalfResolution = { m_ViewportWidth / 2, m_ViewportHeight / 2 };
		screenData.InvFullResolution = { m_InvertedViewportWidth, m_InvertedViewportHeight };
		screenData.InvHalfResolution = { m_InvertedViewportWidth / 2, m_InvertedViewportHeight / 2 };

		Renderer::Submit([instance, screenData]() mutable
		{
			uint32 frameIndex = Renderer::GetCurrentFrameIndex();
			instance->m_UniformBufferSet->GetUniform(17, 0, frameIndex)->SetData(&screenData, sizeof(screenData));
		});
	}

	void SceneRenderer::EndScene()
	{
		HL_ASSERT(m_Active);
		m_Active = false;

		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->FlushDrawList();
		});
	}

	void SceneRenderer::ClearPass(const Ref<RenderPass> &renderPass)
	{
		Renderer::BeginRenderPass(m_CommandBuffer, renderPass, true);
		Renderer::EndRenderPass(m_CommandBuffer);
	}

	void SceneRenderer::ClearPass()
	{
		ClearPass(m_FinalVertexArray->GetSpecification().RenderPass);
	}

	Ref<RenderPass> SceneRenderer::GetFinalRenderPass()
	{
		return m_FinalCompositeRenderPass;
	}

	Ref<Texture2D> SceneRenderer::GetFinalRenderTexture()
	{
		return GetFinalRenderPass()->GetSpecification().Framebuffer->GetImage().As<Texture2D>();
	}

	void SceneRenderer::OnUIRender()
	{

	}

	void SceneRenderer::WaitForThreads()
	{

	}
	
	void SceneRenderer::PreRender()
	{
		m_UniformBufferSet->ForEach([](const Ref<UniformBuffer> &uniformBuffer)
		{
			uniformBuffer->Bind();
		});
	}

	void SceneRenderer::FlushDrawList()
	{
		if (m_ResourcesCreated && m_ViewportWidth > 0 && m_ViewportHeight > 0)
		{
			PreRender();

			m_CommandBuffer->Begin();

			// Geometry pass
		//	Renderer::BeginRenderPass(m_CommandBuffer, m_GeometryRenderPass);
		//	Renderer::EndRenderPass(m_CommandBuffer);

			// Composite pass
			Renderer::BeginRenderPass(m_CommandBuffer, m_FinalCompositeRenderPass, true);

		//	auto &geometryFramebuffer = m_GeometryVertexArray->GetSpecification().RenderPass->GetSpecification().Framebuffer;
			auto &externalFramebuffer = m_ExternalVertexArray->GetSpecification().RenderPass->GetSpecification().Framebuffer;
			float cameraExposure = m_SceneData.SceneCamera.GetExposure();
		//	int32 textureSamples = geometryFramebuffer->GetSpecification().Samples;

		//	m_CompositeMaterial->Set("u_Uniforms.Exposure", cameraExposure);
			m_CompositeMaterial->Set("u_Texture", externalFramebuffer->GetImage().As<Texture2D>());

			Renderer::RenderFullscreenQuad(m_CommandBuffer, m_FinalVertexArray, m_UniformBufferSet, nullptr, m_CompositeMaterial);
			Renderer::EndRenderPass(m_CommandBuffer);

			m_CommandBuffer->End();
			m_CommandBuffer->Submit();
		}
		else
		{
			m_CommandBuffer->Begin();
			ClearPass();
			m_CommandBuffer->End();
			m_CommandBuffer->Submit();
		}
	}
}

