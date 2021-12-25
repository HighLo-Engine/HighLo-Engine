// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

#include "HighLoPch.h"
#include "SceneRenderer.h"
#include "Renderer2D.h"

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
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
		InitGrid();
		InitSkybox();
		InitCompositePass();
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

		});
	}

	void SceneRenderer::EndScene()
	{
		Renderer::Submit([this]() {
			m_CompositeRenderPass->GetSpecification().Framebuffer->Unbind();
		});
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
			framebufferSpec.Attachments = { TextureFormat::RGBA, TextureFormat::Depth };

		Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);

		RenderPassSpecification renderPassSpec;
		renderPassSpec.Framebuffer = framebuffer;
		renderPassSpec.DebugName = "SceneComposite";
		m_CompositeRenderPass = RenderPass::Create(renderPassSpec);
	}


}
