// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "SceneRenderer.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Graphics/Shaders/UniformDefinitions.h"

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

		// Geometry RenderPass
		FramebufferBuilder geometryFramebufferBuilder;
		Ref<Framebuffer> geometryFramebuffer = geometryFramebufferBuilder
			.DebugName("Geometry")
			.SetAttachments({ TextureFormat::RGBA32F, TextureFormat::RGBA16F, TextureFormat::RGBA, TextureFormat::DEPTH32FSTENCIL8UINT })
			.ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f })
			.Samples(1)
			.Build();

		RenderPassBuilder geometryRenderPassBuilder;
		m_GeometryRenderPass = geometryRenderPassBuilder
			.SetFramebuffer(geometryFramebuffer)
			.DebugName("Geometry")
			.Build();

		auto &pbrStaticShader = Renderer::GetShaderLibrary()->Get("HighLoPBR");
		m_GeometryMaterial = Material::Create(pbrStaticShader, "HighLoPBR");

		VertexArrayBuilder geometryVAOBuilder;
		m_GeometryVertexArray = geometryVAOBuilder
			.DebugName("Geometry")
			.LineWidth(Renderer::GetCurrentLineWidth())
			.Layout(BufferLayout::GetStaticShaderLayout())
			.InstanceLayout(BufferLayout::GetTransformBufferLayout())
			.SetShader(pbrStaticShader)
			.SetRenderPass(m_GeometryRenderPass)
			.Build();

		// Selected geometry RenderPass
		// TODO

		// Final Composite RenderPass
		FramebufferSpecification finalFramebufferSpec;
		finalFramebufferSpec.SwapChainTarget = m_Specification.SwapChain;
		
		FramebufferBuilder finalFramebufferBuilder = finalFramebufferBuilder
			.DebugName("FinalCompositeFramebuffer")
			.ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f })
			.SetAttachments({ TextureFormat::RGBA, TextureFormat::Depth });

		if (m_Specification.SwapChain)
		{
			finalFramebufferBuilder = finalFramebufferBuilder
				.WithSwapchainTarget()
				.SetAttachments({ TextureFormat::RGBA });
		}

		Ref<Framebuffer> finalFramebuffer = finalFramebufferBuilder.Build();

		RenderPassBuilder finalRenderPassBuilder;
		m_FinalCompositeRenderPass = finalRenderPassBuilder
			.DebugName("FinalRenderPass")
			.SetFramebuffer(finalFramebuffer)
			.Build();

		auto &compositeShader = Renderer::GetShaderLibrary()->Get("SceneComposite");
		m_FinalMaterial = Material::Create(compositeShader, "CompositeMaterial");
		
		VertexArrayBuilder finalVAOBuilder;
		m_FinalVertexArray = finalVAOBuilder
			.DebugName("FinalVertexArray")
			.SetRenderPass(m_FinalCompositeRenderPass)
			.SetShader(compositeShader)
			.Layout(BufferLayout::GetCompositeLayout())
			.Build();

		// External editing RenderPass
		FramebufferBuilder externalEditingFramebufferBuilder;
		Ref<Framebuffer> externalEditingFramebuffer = externalEditingFramebufferBuilder
			.DebugName("ExternalEditingFramebuffer")
			.SetAttachments({ TextureFormat::RGBA, TextureFormat::DEPTH32FSTENCIL8UINT })
			.ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f })
			.AddExistingImage(m_FinalCompositeRenderPass->GetSpecification().Framebuffer->GetImage().As<Texture2D>())
			.AddExistingImage(m_GeometryRenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>())
			.Build();

		RenderPassBuilder externalEditingRenderPassBuilder;
		m_ExternalCompositingRenderPass = externalEditingRenderPassBuilder
			.DebugName("ExternalEditingRenderPass")
			.SetFramebuffer(externalEditingFramebuffer)
			.Build();

		auto &wireframeShader = Renderer::GetShaderLibrary()->Get("Wireframe");
		m_ExternalMaterial = Material::Create(wireframeShader, "WireframeMaterial");

		VertexArrayBuilder externalEditingVAOBuilder;
		m_GeometryWireframeOnTopVertexArray = externalEditingVAOBuilder
			.DebugName("ExternalEditingVertexArray")
			.SetRenderPass(m_ExternalCompositingRenderPass)
			.SetShader(wireframeShader)
			.Layout(BufferLayout::GetStaticShaderLayout())
			.InstanceLayout(BufferLayout::GetTransformBufferLayout())
			.LineWidth(2.0f)
			.WithWireframe()
			.Build();

		m_ExternalVertexArray = externalEditingVAOBuilder
			.WithDepthTest()
			.Build();

		m_ResourcesCreated = true;
	}

	void SceneRenderer::Shutdown()
	{
		delete s_RendererData;
		s_RendererData = nullptr;
	}

	void SceneRenderer::SetScene(const Ref<Scene> &scene)
	{

	}

	void SceneRenderer::SetViewportSize(uint32 width, uint32 height)
	{

	}

	void SceneRenderer::SetClearColor(const glm::vec4 &color)
	{
	}

	void SceneRenderer::BeginScene(const Camera &camera)
	{

	}

	void SceneRenderer::EndScene()
	{

	}

	void SceneRenderer::ClearPass(const Ref<RenderPass> &renderPass, bool explicitClear)
	{

	}

	void SceneRenderer::ClearPass()
	{

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
}

