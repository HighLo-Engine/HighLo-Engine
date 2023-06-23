#include "HighLoPch.h"
#include "SceneRenderer.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Environment.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

#include "Engine/Graphics/Shaders/UniformBuffer.h"

#include "Engine/Assets/AssetManager.h"

namespace highlo
{
	// MUST AGREE WITH WHAT IS IN THE SHADERS
	// (conversely, shader bindings must agree with this...)
	enum Binding : uint32
	{
		CameraData = 0,
		ShadowData = 1,
		SceneData = 2,
		RendererData = 3,
		PointLightData = 4,

		VisiblePointLightIndicesBuffer = 14,
		VisibleSpotLightIndicesBuffer = 23,

		ScreenData = 17,
		HBAOData = 18
	};

	static std::vector<std::thread> s_ThreadPool;

	SceneRenderer::SceneRenderer(const Ref<Scene> &scene, const SceneRendererSpecification &specification)
		: m_Scene(scene), m_Specification(specification)
	{
		Init();
	}

	SceneRenderer::~SceneRenderer()
	{
		Shutdown();
	}

	void SceneRenderer::Init()
	{
		m_ShadowCascadeSplits[0] = 0.1f;
		m_ShadowCascadeSplits[1] = 0.2f;
		m_ShadowCascadeSplits[2] = 0.3f;
		m_ShadowCascadeSplits[3] = 1.0f;

		// Tiering
	//	{
	//		const auto &tiering = m_Specification.Tiering;
	//
	//		RendererDataUB.SoftShadows = tiering.ShadowQuality == ShadowQualitySetting::High;
	//
	//		m_Options.EnableGTAO = false;
	//		m_Options.EnableHBAO = false;
	//
	//		if (tiering.EnableAO)
	//		{
	//			switch (tiering.AOType)
	//			{
	//			case Tiering::Renderer::AmbientOcclusionTypeSetting::HBAO:
	//				m_Options.EnableHBAO = true;
	//				break;
	//			case Tiering::Renderer::AmbientOcclusionTypeSetting::GTAO:
	//				m_Options.EnableGTAO = true;
	//				break;
	//			}
	//		}
	//	}

		if (m_Specification.Swapchain)
			m_CommandBuffer = CommandBuffer::CreateFromSwapChain("SceneRenderer");
		else
			m_CommandBuffer = CommandBuffer::Create("SceneRenderer");

		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		m_UniformBufferSet = UniformBufferSet::Create(framesInFlight);
		m_UniformBufferSet->CreateUniform(sizeof(UBCamera), 0);
		m_UniformBufferSet->CreateUniform(sizeof(UBShadow), 1);
		m_UniformBufferSet->CreateUniform(sizeof(UBScene), 2);
		m_UniformBufferSet->CreateUniform(sizeof(UBRendererData), 3);
		m_UniformBufferSet->CreateUniform(sizeof(UBPointLights), 4);
		m_UniformBufferSet->CreateUniform(sizeof(UBScreenData), 17);
		m_UniformBufferSet->CreateUniform(sizeof(UBSpotLights), 19);
		m_UniformBufferSet->CreateUniform(sizeof(UBHBAOData), 18);
		m_UniformBufferSet->CreateUniform(sizeof(UBSpotShadowData), 20);


		m_CompositeShader = Renderer::GetShaderLibrary()->Get("SceneComposite");
		m_CompositeMaterial = Material::Create(m_CompositeShader);

		// Light culling compute pipeline
		{
			m_StorageBufferSet = StorageBufferSet::Create(framesInFlight);
			m_StorageBufferSet->CreateStorage(1, Binding::VisiblePointLightIndicesBuffer); //Can't allocate 0 bytes.. Resized later
			m_StorageBufferSet->CreateStorage(1, Binding::VisibleSpotLightIndicesBuffer); //Can't allocate 0 bytes.. Resized later

			m_LightCullingMaterial = Material::Create(Renderer::GetShaderLibrary()->Get("LightCulling"), "LightCulling");
			Ref<Shader> lightCullingShader = Renderer::GetShaderLibrary()->Get("LightCulling");
			m_LightCullingPipeline = ComputePipeline::Create(lightCullingShader);
		}

		BufferLayout vertexLayout = BufferLayout::GetStaticShaderLayout();
		BufferLayout boneInfluenceLayout = BufferLayout::GetBoneInfluenceLayout();
		BufferLayout instanceLayout =
		{
			{ "a_MRow0", ShaderDataType::Float4 },
			{ "a_MRow1", ShaderDataType::Float4 },
			{ "a_MRow2", ShaderDataType::Float4 },
		};

		uint32 shadowMapResolution = 4096;
	//	switch (m_Specification.Tiering.ShadowResolution)
	//	{
	//	case Tiering::Renderer::ShadowResolutionSetting::Low:
	//		shadowMapResolution = 1024;
	//		break;
	//	case Tiering::Renderer::ShadowResolutionSetting::Medium:
	//		shadowMapResolution = 2048;
	//		break;
	//	case Tiering::Renderer::ShadowResolutionSetting::High:
	//		shadowMapResolution = 4096;
	//		break;
	//	}

		// Directional Shadow pass
		{
			TextureSpecification spec;
			spec.Format = TextureFormat::DEPTH32F;
			spec.Usage = TextureUsage::Attachment;
			spec.Width = shadowMapResolution;
			spec.Height = shadowMapResolution;
			spec.Layers = 4; // 4 cascades
			spec.DebugName = "ShadowCascades";
			Ref<Texture2D> cascadedDepthImage = Texture2D::CreateFromSpecification(spec);
			cascadedDepthImage->Invalidate();
			cascadedDepthImage->CreatePerLayerImageViews();

			FramebufferSpecification shadowMapFramebufferSpec;
			shadowMapFramebufferSpec.DebugName = "Dir Shadow Map";
			shadowMapFramebufferSpec.Width = shadowMapResolution;
			shadowMapFramebufferSpec.Height = shadowMapResolution;
			shadowMapFramebufferSpec.Attachments = { TextureFormat::DEPTH32F };
			shadowMapFramebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			shadowMapFramebufferSpec.DepthClearValue = 1.0f;
			shadowMapFramebufferSpec.NoResize = true;
			shadowMapFramebufferSpec.ExistingImage = cascadedDepthImage;

			auto shadowPassShader = Renderer::GetShaderLibrary()->Get("DirShadowMap");
			auto shadowPassShaderAnim = Renderer::GetShaderLibrary()->Get("DirShadowMap_Anim");

			VertexArraySpecification pipelineSpec;
			pipelineSpec.DebugName = "DirShadowPass";
			pipelineSpec.Shader = shadowPassShader;
			pipelineSpec.DepthCompareOp = DepthCompareOperator::LessOrEqual;
			pipelineSpec.Layout = vertexLayout;
			pipelineSpec.InstanceLayout = instanceLayout;

			VertexArraySpecification pipelineSpecAnim = pipelineSpec;
			pipelineSpecAnim.DebugName = "DirShadowPass-Anim";
			pipelineSpecAnim.Shader = shadowPassShaderAnim;
			pipelineSpecAnim.BoneInfluenceLayout = boneInfluenceLayout;

			// 4 cascades
			for (int i = 0; i < 4; i++)
			{
				shadowMapFramebufferSpec.ExistingImageLayers.clear();
				shadowMapFramebufferSpec.ExistingImageLayers.emplace_back(i);

				RenderPassSpecification shadowMapRenderPassSpec;
				shadowMapRenderPassSpec.DebugName = shadowMapFramebufferSpec.DebugName;
				shadowMapRenderPassSpec.Framebuffer = Framebuffer::Create(shadowMapFramebufferSpec);
				auto renderpass = RenderPass::Create(shadowMapRenderPassSpec);

				pipelineSpec.RenderPass = renderpass;
				m_ShadowPassPipelines[i] = VertexArray::Create(pipelineSpec);

				pipelineSpecAnim.RenderPass = renderpass;
				m_ShadowPassPipelinesAnim[i] = VertexArray::Create(pipelineSpecAnim);
			}
			m_ShadowPassMaterial = Material::Create(shadowPassShader, "DirShadowPass");
		}

		// Non-directional shadow mapping pass
		{
			FramebufferSpecification framebufferSpec;
			framebufferSpec.Width = shadowMapResolution; // TODO: could probably halve these
			framebufferSpec.Height = shadowMapResolution;
			framebufferSpec.Attachments = { TextureFormat::DEPTH32F };
			framebufferSpec.DepthClearValue = 1.0f;
			framebufferSpec.NoResize = true;
			framebufferSpec.DebugName = "Spot Shadow Map";

			auto shadowPassShader = Renderer::GetShaderLibrary()->Get("SpotShadowMap");
			auto shadowPassShaderAnim = Renderer::GetShaderLibrary()->Get("SpotShadowMap_Anim");

			VertexArraySpecification pipelineSpec;
			pipelineSpec.DebugName = "SpotShadowPass";
			pipelineSpec.Shader = shadowPassShader;
			pipelineSpec.DepthCompareOp = DepthCompareOperator::LessOrEqual;

			pipelineSpec.Layout = BufferLayout::GetStaticShaderLayout();
			pipelineSpec.InstanceLayout = {
				{ "a_MRow0", ShaderDataType::Float4 },
				{ "a_MRow1", ShaderDataType::Float4 },
				{ "a_MRow2", ShaderDataType::Float4 },
			};

			RenderPassSpecification shadowMapRenderPassSpec;
			shadowMapRenderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
			shadowMapRenderPassSpec.DebugName = "SpotShadowMap";
			pipelineSpec.RenderPass = RenderPass::Create(shadowMapRenderPassSpec);

			VertexArraySpecification pipelineSpecAnim = pipelineSpec;
			pipelineSpecAnim.DebugName = "SpotShadowPass-Anim";
			pipelineSpecAnim.Shader = shadowPassShaderAnim;
			pipelineSpecAnim.BoneInfluenceLayout = boneInfluenceLayout;

			m_SpotShadowPassPipeline = VertexArray::Create(pipelineSpec);
			m_SpotShadowPassAnimPipeline = VertexArray::Create(pipelineSpecAnim);

			m_SpotShadowPassMaterial = Material::Create(shadowPassShader, "SpotShadowPass");
		}

		// PreDepth
		{
			FramebufferSpecification preDepthFramebufferSpec;
			preDepthFramebufferSpec.DebugName = "PreDepth-Opaque";
			//Linear depth, reversed device depth
			preDepthFramebufferSpec.Attachments = { /*TextureFormat::RED32F, */ TextureFormat::DEPTH32FSTENCIL8UINT };
			preDepthFramebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			preDepthFramebufferSpec.DepthClearValue = 0.0f;

			RenderPassSpecification preDepthRenderPassSpec;
			preDepthRenderPassSpec.DebugName = preDepthFramebufferSpec.DebugName;
			preDepthRenderPassSpec.Framebuffer = Framebuffer::Create(preDepthFramebufferSpec);

			VertexArraySpecification pipelineSpec;
			pipelineSpec.DebugName = preDepthFramebufferSpec.DebugName;

			pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("PreDepth");
			pipelineSpec.Layout = vertexLayout;
			pipelineSpec.InstanceLayout = instanceLayout;
			pipelineSpec.RenderPass = RenderPass::Create(preDepthRenderPassSpec);
			m_PreDepthPipeline = VertexArray::Create(pipelineSpec);
			m_PreDepthMaterial = Material::Create(pipelineSpec.Shader, pipelineSpec.DebugName);

			pipelineSpec.DebugName = "PreDepth-Anim";
			pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("PreDepth_Anim");
			pipelineSpec.BoneInfluenceLayout = boneInfluenceLayout;
			m_PreDepthPipelineAnim = VertexArray::Create(pipelineSpec);  // same renderpass as Predepth-Opaque

			pipelineSpec.DebugName = "PreDepth-Transparent";
			pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("PreDepth");
			preDepthFramebufferSpec.DebugName = pipelineSpec.DebugName;
			preDepthRenderPassSpec.Framebuffer = Framebuffer::Create(preDepthFramebufferSpec);
			preDepthRenderPassSpec.DebugName = pipelineSpec.DebugName;
			pipelineSpec.RenderPass = RenderPass::Create(preDepthRenderPassSpec);
			m_PreDepthTransparentPipeline = VertexArray::Create(pipelineSpec);

			// TODO: Need PreDepth-Transparent-Animated pipeline

		}

		// Hierarchical Z buffer
		{
			TextureSpecification spec;
			spec.Format = TextureFormat::RED32F;
			spec.Width = 1;
			spec.Height = 1;
			spec.Properties.SamplerWrap = TextureWrap::Clamp;
			spec.Properties.SamplerFilter = TextureFilter::Nearest;
			spec.DebugName = "HierarchicalZ";

			m_HierarchicalDepthTexture = Texture2D::CreateFromSpecification(spec);

			Ref<Shader> shader = Renderer::GetShaderLibrary()->Get("HZB");
			m_HierarchicalDepthPipeline = ComputePipeline::Create(shader);
			m_HierarchicalDepthMaterial = Material::Create(shader, "HZB");
		}

		// Pre-Integration
		{
			TextureSpecification spec;
			spec.Format = TextureFormat::RED8UN;
			spec.Width = 1;
			spec.Height = 1;
			spec.DebugName = "Pre-Integration";

			m_VisibilityTexture = Texture2D::CreateFromSpecification(spec);

			Ref<Shader> shader = Renderer::GetShaderLibrary()->Get("Pre-Integration");
			m_PreIntegrationPipeline = ComputePipeline::Create(shader);
			m_PreIntegrationMaterial = Material::Create(shader, "Pre-Integration");
		}

		// Geometry
		{
			FramebufferSpecification geoFramebufferSpec;
			geoFramebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::RGBA16F, TextureFormat::RGBA, TextureFormat::DEPTH32FSTENCIL8UINT };
			geoFramebufferSpec.ExistingImages[3] = m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>();

			// Don't blend with luminance in the alpha channel.
			geoFramebufferSpec.Attachments.Attachments[1].Blend = false;
			geoFramebufferSpec.Samples = 1;
			geoFramebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			geoFramebufferSpec.DebugName = "Geometry";
			geoFramebufferSpec.ClearDepthOnLoad = false;
			Ref<Framebuffer> framebuffer = Framebuffer::Create(geoFramebufferSpec);

			RenderPassSpecification renderPassSpec;
			renderPassSpec.DebugName = geoFramebufferSpec.DebugName;
			renderPassSpec.Framebuffer = framebuffer;
			Ref<RenderPass> renderPass = RenderPass::Create(renderPassSpec);

			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.DebugName = "PBR-Static";
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("HazelPBR_Static");
			pipelineSpecification.DepthCompareOp = DepthCompareOperator::Equal;
			pipelineSpecification.DepthWrite = false;
			pipelineSpecification.Layout = vertexLayout;
			pipelineSpecification.InstanceLayout = instanceLayout;
			pipelineSpecification.LineWidth = m_LineWidth;
			pipelineSpecification.RenderPass = renderPass;
			m_GeometryPipeline = VertexArray::Create(pipelineSpecification);

			//
			// Transparent Geometry
			//
			pipelineSpecification.DebugName = "PBR-Transparent";
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("HazelPBR_Transparent");
			pipelineSpecification.DepthCompareOp = DepthCompareOperator::GreaterOrEqual;
			m_TransparentGeometryPipeline = VertexArray::Create(pipelineSpecification);

			//
			// Animated Geometry
			//
			pipelineSpecification.DebugName = "PBR-Anim";
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("HazelPBR_Anim");
			pipelineSpecification.DepthCompareOp = DepthCompareOperator::Equal;
			pipelineSpecification.BoneInfluenceLayout = boneInfluenceLayout;
			m_GeometryPipelineAnim = VertexArray::Create(pipelineSpecification);

			// TODO: Need Transparent-Animated geometry pipeline

		}

		// Selected Geometry isolation (for outline pass)
		{
			FramebufferSpecification framebufferSpec;
			framebufferSpec.DebugName = "SelectedGeometry";
			framebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::Depth };
			framebufferSpec.Samples = 1;
			framebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			framebufferSpec.DepthClearValue = 1.0f;

			RenderPassSpecification renderPassSpec;
			renderPassSpec.DebugName = framebufferSpec.DebugName;
			renderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
			Ref<RenderPass> renderPass = RenderPass::Create(renderPassSpec);

			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.DebugName = framebufferSpec.DebugName;
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("SelectedGeometry");
			pipelineSpecification.Layout = vertexLayout;
			pipelineSpecification.InstanceLayout = instanceLayout;
			pipelineSpecification.RenderPass = renderPass;
			pipelineSpecification.DepthCompareOp = DepthCompareOperator::LessOrEqual;
			m_SelectedGeometryPipeline = VertexArray::Create(pipelineSpecification);
			m_SelectedGeometryMaterial = Material::Create(pipelineSpecification.Shader, pipelineSpecification.DebugName);

			pipelineSpecification.DebugName = "SelectedGeometry-Anim";
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("SelectedGeometry_Anim");
			pipelineSpecification.BoneInfluenceLayout = boneInfluenceLayout;
			m_SelectedGeometryPipelineAnim = VertexArray::Create(pipelineSpecification); // Note: same framebuffer and renderpass as m_SelectedGeometryPipeline
		}

		// Pre-convolution Compute
		{
			auto shader = Renderer::GetShaderLibrary()->Get("Pre-Convolution");
			m_GaussianBlurPipeline = ComputePipeline::Create(shader);
			TextureSpecification spec;
			spec.Format = TextureFormat::RGBA32F;
			spec.Width = 1;
			spec.Height = 1;
			spec.Properties.SamplerWrap = TextureWrap::Clamp;
			spec.DebugName = "Pre-Convoluted";
			m_PreConvolutedTexture = Texture2D::CreateFromSpecification(spec);

			m_GaussianBlurMaterial = Material::Create(shader);
		}

		// Bloom Compute
		{
			auto shader = Renderer::GetShaderLibrary()->Get("Bloom");
			m_BloomComputePipeline = ComputePipeline::Create(shader);
			TextureSpecification spec;
			spec.Format = TextureFormat::RGBA32F;
			spec.Width = 1;
			spec.Height = 1;
			spec.Properties.SamplerWrap = TextureWrap::Clamp;
			spec.Properties.Storage = true;
			spec.Properties.GenerateMips = true;
			spec.DebugName = "BloomCompute-0";
			m_BloomComputeTextures[0] = Texture2D::CreateFromSpecification(spec);
			spec.DebugName = "BloomCompute-1";
			m_BloomComputeTextures[1] = Texture2D::CreateFromSpecification(spec);
			spec.DebugName = "BloomCompute-2";
			m_BloomComputeTextures[2] = Texture2D::CreateFromSpecification(spec);
			m_BloomComputeMaterial = Material::Create(shader);

			m_BloomDirtTexture = Renderer::GetBlackTexture();
		}

		// Deinterleaving
		{
			TextureSpecification imageSpec;
			imageSpec.Format = TextureFormat::RED32F;
			imageSpec.Layers = 16;
			imageSpec.Usage = TextureUsage::Attachment;
			imageSpec.DebugName = "Deinterleaved";
			Ref<Texture2D> image = Texture2D::CreateFromSpecification(imageSpec);
			image->Invalidate();
			image->CreatePerLayerImageViews();

			FramebufferSpecification deinterleavingFramebufferSpec;
			deinterleavingFramebufferSpec.Attachments.Attachments = { 8, FramebufferTextureSpecification{ TextureFormat::RED32F } };
			deinterleavingFramebufferSpec.ClearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
			deinterleavingFramebufferSpec.DebugName = "Deinterleaving";
			deinterleavingFramebufferSpec.ExistingImage = image;

			Ref<Shader> shader = Renderer::GetShaderLibrary()->Get("Deinterleaving");
			VertexArraySpecification pipelineSpec;
			pipelineSpec.DebugName = "Deinterleaving";
			pipelineSpec.DepthWrite = false;
			pipelineSpec.DepthTest = false;

			pipelineSpec.Shader = shader;
			pipelineSpec.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 }
			};

			// 2 frame buffers, 2 render passes .. 8 attachments each
			for (int32 rp = 0; rp < 2; rp++)
			{
				deinterleavingFramebufferSpec.ExistingImageLayers.clear();
				for (int32 layer = 0; layer < 8; layer++)
					deinterleavingFramebufferSpec.ExistingImageLayers.emplace_back(rp * 8 + layer);

				Ref<Framebuffer> framebuffer = Framebuffer::Create(deinterleavingFramebufferSpec);

				RenderPassSpecification deinterleavingRenderPassSpec;
				deinterleavingRenderPassSpec.Framebuffer = framebuffer;
				deinterleavingRenderPassSpec.DebugName = "Deinterleaving";
				pipelineSpec.RenderPass = RenderPass::Create(deinterleavingRenderPassSpec);

				m_DeinterleavingPipelines[rp] = VertexArray::Create(pipelineSpec);
			}
			m_DeinterleavingMaterial = Material::Create(pipelineSpec.Shader, pipelineSpec.DebugName);

		}

		// HBAO
		{
			TextureSpecification imageSpec;
			imageSpec.Format = TextureFormat::RG16F;
			imageSpec.Usage = TextureUsage::Storage;
			imageSpec.Layers = 16;
			imageSpec.DebugName = "HBAO-Output";
			Ref<Texture2D> image = Texture2D::CreateFromSpecification(imageSpec);
			image->Invalidate();
			image->CreatePerLayerImageViews();

			m_HBAOOutputImage = image;

			Ref<Shader> shader = Renderer::GetShaderLibrary()->Get("HBAO");

			m_HBAOPipeline = ComputePipeline::Create(shader);
			m_HBAOMaterial = Material::Create(shader, "HBAO");

			for (int32 i = 0; i < 16; i++)
				HBAODataUB.Float2Offsets[i] = glm::vec4((float)(i % 4) + 0.5f, (float)(i / 4) + 0.5f, 0.0f, 1.f);
		
			// TODO
		//	std::memcpy(HBAODataUB.Jitters, Noise::HBAOJitter().data(), sizeof(glm::vec4) * 16);
		}

		// GTAO
		{
			{
				TextureSpecification imageSpec;
				if (m_Options.GTAOBentNormals)
					imageSpec.Format = TextureFormat::RED32UI;
				else
					imageSpec.Format = TextureFormat::RED8UI;
				imageSpec.Usage = TextureUsage::Storage;
				imageSpec.Layers = 1;
				imageSpec.DebugName = "GTAO";
				m_GTAOOutputImage = Texture2D::CreateFromSpecification(imageSpec);
				m_GTAOOutputImage->Invalidate();
			}

			// GTAO-Edges
			{
				TextureSpecification imageSpec;
				imageSpec.Format = TextureFormat::RED8UN;
				imageSpec.Usage = TextureUsage::Storage;
				imageSpec.DebugName = "GTAO-Edges";
				m_GTAOEdgesOutputImage = Texture2D::CreateFromSpecification(imageSpec);
				m_GTAOEdgesOutputImage->Invalidate();
			}

			Ref<Shader> shader = Renderer::GetShaderLibrary()->Get("GTAO");

			m_GTAOPipeline = ComputePipeline::Create(shader);
			m_GTAOMaterial = Material::Create(shader, "GTAO");
		}

		// GTAO Denoise
		{
			{
				TextureSpecification imageSpec;
				if (m_Options.GTAOBentNormals)
					imageSpec.Format = TextureFormat::RED32UI;
				else
					imageSpec.Format = TextureFormat::RED8UI;
				imageSpec.Usage = TextureUsage::Storage;
				imageSpec.Layers = 1;
				imageSpec.DebugName = "GTAO-Denoise";
				m_GTAODenoiseImage = Texture2D::CreateFromSpecification(imageSpec);
				m_GTAODenoiseImage->Invalidate();

				Ref<Shader> shader = Renderer::GetShaderLibrary()->Get("GTAO-Denoise");
				m_GTAODenoiseMaterial[0] = Material::Create(shader, "GTAO-Denoise-Ping");
				m_GTAODenoiseMaterial[1] = Material::Create(shader, "GTAO-Denoise-Pong");

				m_GTAODenoisePipeline = ComputePipeline::Create(shader);
			}

			// GTAO Composite
			{
				VertexArraySpecification aoCompositePipelineSpec;
				aoCompositePipelineSpec.DebugName = "AO-Composite";
				RenderPassSpecification renderPassSpec;
				renderPassSpec.DebugName = "AO-Composite";
				FramebufferSpecification framebufferSpec;
				framebufferSpec.DebugName = "AO-Composite";
				framebufferSpec.Attachments = { TextureFormat::RGBA32F };
				framebufferSpec.ExistingImages[0] = m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(0).As<Texture2D>();
				framebufferSpec.Blend = true;
				framebufferSpec.ClearColorOnLoad = false;
				framebufferSpec.BlendMode = FramebufferBlendMode::ZeroSrcColor;

				renderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
				aoCompositePipelineSpec.RenderPass = RenderPass::Create(renderPassSpec);
				m_AOCompositeRenderPass = aoCompositePipelineSpec.RenderPass;
				aoCompositePipelineSpec.DepthTest = false;
				aoCompositePipelineSpec.Layout = {
					{ "a_Position", ShaderDataType::Float3 },
					{ "a_TexCoord", ShaderDataType::Float2 },
				};
				aoCompositePipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("AO-Composite");
				m_AOCompositePipeline = VertexArray::Create(aoCompositePipelineSpec);
				m_AOCompositeMaterial = Material::Create(aoCompositePipelineSpec.Shader, "GTAO-Composite");
			}
		}

		// Reinterleaving
		{
			FramebufferSpecification reinterleavingFramebufferSpec;
			reinterleavingFramebufferSpec.Attachments = { TextureFormat::RG16F, };
			reinterleavingFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			reinterleavingFramebufferSpec.DebugName = "Reinterleaving";

			Ref<Framebuffer> framebuffer = Framebuffer::Create(reinterleavingFramebufferSpec);
			Ref<Shader> shader = Renderer::GetShaderLibrary()->Get("Reinterleaving");
			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpecification.BackfaceCulling = false;
			pipelineSpecification.Shader = shader;
			pipelineSpecification.DepthTest = false;
			pipelineSpecification.DepthWrite = false;

			RenderPassSpecification renderPassSpec;
			renderPassSpec.Framebuffer = framebuffer;
			renderPassSpec.DebugName = "Reinterleaving";
			pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);
			pipelineSpecification.DebugName = "Reinterleaving";
			pipelineSpecification.DepthWrite = false;
			m_ReinterleavingPipeline = VertexArray::Create(pipelineSpecification);

			m_ReinterleavingMaterial = Material::Create(pipelineSpecification.Shader, pipelineSpecification.DebugName);
		}

		// HBAO Blur
		{
			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpecification.BackfaceCulling = false;
			pipelineSpecification.DepthTest = false;
			pipelineSpecification.DepthWrite = false;

			auto shader = Renderer::GetShaderLibrary()->Get("HBAOBlur");

			FramebufferSpecification hbaoBlurFramebufferSpec;
			hbaoBlurFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			hbaoBlurFramebufferSpec.Attachments.Attachments.emplace_back(TextureFormat::RG16F);

			RenderPassSpecification renderPassSpec;
			renderPassSpec.DebugName = hbaoBlurFramebufferSpec.DebugName;

			pipelineSpecification.Shader = shader;
			pipelineSpecification.DebugName = "HBAOBlur";
			// First blur pass
			{
				pipelineSpecification.DebugName = "HBAOBlur1";
				hbaoBlurFramebufferSpec.DebugName = "HBAOBlur1";
				renderPassSpec.Framebuffer = Framebuffer::Create(hbaoBlurFramebufferSpec);
				pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);
				m_HBAOBlurPipelines[0] = VertexArray::Create(pipelineSpecification);
			}
			// Second blur pass
			{
				pipelineSpecification.DebugName = "HBAOBlur2";
				hbaoBlurFramebufferSpec.DebugName = "HBAOBlur2";
				renderPassSpec.Framebuffer = Framebuffer::Create(hbaoBlurFramebufferSpec);
				pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);
				m_HBAOBlurPipelines[1] = VertexArray::Create(pipelineSpecification);
			}
			m_HBAOBlurMaterials[0] = Material::Create(shader, "HBAOBlur");
			m_HBAOBlurMaterials[1] = Material::Create(shader, "HBAOBlur2");

		}

		// SSR
		{
			TextureSpecification imageSpec;
			imageSpec.Format = TextureFormat::RGBA16F;
			imageSpec.Usage = TextureUsage::Storage;
			imageSpec.DebugName = "SSR";
			Ref<Texture2D> image = Texture2D::CreateFromSpecification(imageSpec);
			image->Invalidate();

			m_SSRImage = image;

			Ref<Shader> shader = Renderer::GetShaderLibrary()->Get("SSR");

			m_SSRPipeline = ComputePipeline::Create(shader);
			m_SSRMaterial = Material::Create(shader, "SSR");
		}

		// SSR Composite
		{
			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpecification.BackfaceCulling = false;
			pipelineSpecification.DepthTest = false;
			pipelineSpecification.DepthWrite = false;
			pipelineSpecification.DebugName = "SSR-Composite";
			auto shader = Renderer::GetShaderLibrary()->Get("SSR-Composite");
			pipelineSpecification.Shader = shader;

			FramebufferSpecification framebufferSpec;
			framebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			framebufferSpec.Attachments.Attachments.emplace_back(TextureFormat::RGBA32F);
			framebufferSpec.ExistingImages[0] = m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(0).As<Texture2D>();
			framebufferSpec.DebugName = "SSR-Composite";
			framebufferSpec.BlendMode = FramebufferBlendMode::SrcAlphaOneMinusSrcAlpha;
			framebufferSpec.ClearColorOnLoad = false;

			RenderPassSpecification renderPassSpec;
			renderPassSpec.Framebuffer = Framebuffer::Create(framebufferSpec);
			renderPassSpec.DebugName = framebufferSpec.DebugName;
			pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);

			m_SSRCompositePipeline = VertexArray::Create(pipelineSpecification);
			m_SSRCompositeMaterial = Material::Create(shader, "SSR-Composite");
		}

		// Composite
		{
			FramebufferSpecification compFramebufferSpec;
			compFramebufferSpec.DebugName = "SceneComposite";
			compFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			compFramebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::Depth };
			compFramebufferSpec.Transfer = true;

			Ref<Framebuffer> framebuffer = Framebuffer::Create(compFramebufferSpec);

			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpecification.BackfaceCulling = false;
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("SceneComposite");

			RenderPassSpecification renderPassSpec;
			renderPassSpec.Framebuffer = framebuffer;
			renderPassSpec.DebugName = "SceneComposite";
			pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);
			pipelineSpecification.DebugName = "SceneComposite";
			pipelineSpecification.DepthWrite = false;
			pipelineSpecification.DepthTest = false;
			m_CompositePipeline = VertexArray::Create(pipelineSpecification);
		}

		// DOF
		{
			FramebufferSpecification compFramebufferSpec;
			compFramebufferSpec.DebugName = "POST-DepthOfField";
			compFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			compFramebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::Depth };
			compFramebufferSpec.Transfer = true;

			Ref<Framebuffer> framebuffer = Framebuffer::Create(compFramebufferSpec);

			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpecification.BackfaceCulling = false;
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("DOF");

			RenderPassSpecification renderPassSpec;
			renderPassSpec.Framebuffer = framebuffer;
			renderPassSpec.DebugName = compFramebufferSpec.DebugName;
			pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);
			pipelineSpecification.DebugName = compFramebufferSpec.DebugName;
			pipelineSpecification.DepthWrite = false;
			m_DOFPipeline = VertexArray::Create(pipelineSpecification);
			m_DOFMaterial = Material::Create(pipelineSpecification.Shader, pipelineSpecification.DebugName);
		}

		// TODO: Edge Detection
		if (false)
		{
			FramebufferSpecification compFramebufferSpec;
			compFramebufferSpec.DebugName = "POST-EdgeDetection";
			compFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			compFramebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::Depth };
			compFramebufferSpec.Transfer = true;

			Ref<Framebuffer> framebuffer = Framebuffer::Create(compFramebufferSpec);

			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpecification.BackfaceCulling = false;
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("EdgeDetection");

			RenderPassSpecification renderPassSpec;
			renderPassSpec.Framebuffer = framebuffer;
			renderPassSpec.DebugName = compFramebufferSpec.DebugName;
			pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);
			pipelineSpecification.DebugName = compFramebufferSpec.DebugName;
			pipelineSpecification.DepthWrite = false;
			m_EdgeDetectionPipeline = VertexArray::Create(pipelineSpecification);
			m_EdgeDetectionMaterial = Material::Create(pipelineSpecification.Shader, pipelineSpecification.DebugName);
		}

		// External compositing
		{
			FramebufferSpecification extCompFramebufferSpec;
			extCompFramebufferSpec.DebugName = "External-Composite";
			extCompFramebufferSpec.Attachments = { TextureFormat::RGBA32F, TextureFormat::DEPTH32FSTENCIL8UINT };
			extCompFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			extCompFramebufferSpec.ClearColorOnLoad = false;
			extCompFramebufferSpec.ClearDepthOnLoad = false;
			// Use the color buffer from the final compositing pass, but the depth buffer from
			// the actual 3D geometry pass, in case we want to composite elements behind meshes
			// in the scene
			extCompFramebufferSpec.ExistingImages[0] = m_CompositePipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage().As<Texture2D>();
			extCompFramebufferSpec.ExistingImages[1] = m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>();
			Ref<Framebuffer> framebuffer = Framebuffer::Create(extCompFramebufferSpec);

			RenderPassSpecification renderPassSpec;
			renderPassSpec.DebugName = extCompFramebufferSpec.DebugName;
			renderPassSpec.Framebuffer = framebuffer;
			m_ExternalCompositeRenderPass = RenderPass::Create(renderPassSpec);

			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.DebugName = "Wireframe";
			pipelineSpecification.BackfaceCulling = false;
			pipelineSpecification.Wireframe = true;
			pipelineSpecification.DepthTest = true;
			pipelineSpecification.LineWidth = 2.0f;
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("Wireframe");
			pipelineSpecification.Layout = vertexLayout;
			pipelineSpecification.InstanceLayout = instanceLayout;
			pipelineSpecification.RenderPass = m_ExternalCompositeRenderPass;
			m_GeometryWireframePipeline = VertexArray::Create(pipelineSpecification);

			pipelineSpecification.DepthTest = false;
			pipelineSpecification.DebugName = "Wireframe-OnTop";
			m_GeometryWireframeOnTopPipeline = VertexArray::Create(pipelineSpecification);

			pipelineSpecification.DepthTest = true;
			pipelineSpecification.DebugName = "Wireframe-Anim";
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("Wireframe_Anim");
			pipelineSpecification.BoneInfluenceLayout = boneInfluenceLayout;
			m_GeometryWireframePipelineAnim = VertexArray::Create(pipelineSpecification); // Note: same framebuffer and renderpass as m_GeometryWireframePipeline

			pipelineSpecification.DepthTest = false;
			pipelineSpecification.DebugName = "Wireframe-Anim-OnTop";
			m_GeometryWireframeOnTopPipelineAnim = VertexArray::Create(pipelineSpecification);

		}

		// TODO: Read-back Image
		if (false) // WIP
		{
			TextureSpecification spec;
			spec.Format = TextureFormat::RGBA32F;
			spec.Usage = TextureUsage::HostRead;
			spec.Transfer = true;
			spec.DebugName = "ReadBack";
			m_ReadBackImage = Texture2D::CreateFromSpecification(spec);
		}

		// Temporary framebuffers for re-use
		{
			FramebufferSpecification framebufferSpec;
			framebufferSpec.Attachments = { TextureFormat::RGBA32F };
			framebufferSpec.Samples = 1;
			framebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };
			framebufferSpec.BlendMode = FramebufferBlendMode::OneZero;
			framebufferSpec.DebugName = "Temporaries";

			for (uint32 i = 0; i < 2; i++)
				m_TempFramebuffers.emplace_back(Framebuffer::Create(framebufferSpec));
		}

		// Jump Flood (outline)
		{
			RenderPassSpecification renderPassSpec;
			renderPassSpec.DebugName = "JumpFlood-Init";
			renderPassSpec.Framebuffer = m_TempFramebuffers[0];

			VertexArraySpecification pipelineSpecification;
			pipelineSpecification.DebugName = renderPassSpec.DebugName;
			pipelineSpecification.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);
			pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("JumpFlood_Init");
			m_JumpFloodInitPipeline = VertexArray::Create(pipelineSpecification);
			m_JumpFloodInitMaterial = Material::Create(pipelineSpecification.Shader, pipelineSpecification.DebugName);

			const char *passName[2] = { "EvenPass", "OddPass" };
			for (uint32 i = 0; i < 2; i++)
			{
				renderPassSpec.Framebuffer = m_TempFramebuffers[(i + 1) % 2];
				renderPassSpec.DebugName = fmt::format("JumpFlood-{0}", passName[i]);

				pipelineSpecification.DebugName = renderPassSpec.DebugName;
				pipelineSpecification.RenderPass = RenderPass::Create(renderPassSpec);
				pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("JumpFlood_Pass");
				m_JumpFloodPassPipeline[i] = VertexArray::Create(pipelineSpecification);
				m_JumpFloodPassMaterial[i] = Material::Create(pipelineSpecification.Shader, pipelineSpecification.DebugName);
			}

			// Outline compositing
			{
				pipelineSpecification.RenderPass = m_CompositePipeline->GetSpecification().RenderPass;
				pipelineSpecification.DebugName = "JumpFlood-Composite";
				pipelineSpecification.Shader = Renderer::GetShaderLibrary()->Get("JumpFlood_Composite");
				pipelineSpecification.DepthTest = false;
				m_JumpFloodCompositePipeline = VertexArray::Create(pipelineSpecification);
				m_JumpFloodCompositeMaterial = Material::Create(pipelineSpecification.Shader, pipelineSpecification.DebugName);
			}
		}

		// Grid
		{
			VertexArraySpecification pipelineSpec;
			pipelineSpec.DebugName = "Grid";
			pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("Grid");
			pipelineSpec.BackfaceCulling = false;
			pipelineSpec.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpec.RenderPass = m_ExternalCompositeRenderPass;
			m_GridPipeline = VertexArray::Create(pipelineSpec);

			const float gridScale = 16.025f;
			const float gridSize = 0.025f;
			m_GridMaterial = Material::Create(pipelineSpec.Shader, pipelineSpec.DebugName);
			m_GridMaterial->Set("u_Settings.Scale", gridScale);
			m_GridMaterial->Set("u_Settings.Size", gridSize);
		}

		// Collider
		m_SimpleColliderMaterial = Material::Create(Renderer::GetShaderLibrary()->Get("Wireframe"), "SimpleCollider");
		m_SimpleColliderMaterial->Set("u_MaterialUniforms.Color", m_Options.SimplePhysicsCollidersColor);
		m_ComplexColliderMaterial = Material::Create(Renderer::GetShaderLibrary()->Get("Wireframe"), "ComplexCollider");
		m_ComplexColliderMaterial->Set("u_MaterialUniforms.Color", m_Options.ComplexPhysicsCollidersColor);

		m_WireframeMaterial = Material::Create(Renderer::GetShaderLibrary()->Get("Wireframe"), "Wireframe");
		m_WireframeMaterial->Set("u_MaterialUniforms.Color", glm::vec4{ 1.0f, 0.5f, 0.0f, 1.0f });

		// Skybox
		{
			auto skyboxShader = Renderer::GetShaderLibrary()->Get("Skybox");

			VertexArraySpecification pipelineSpec;
			pipelineSpec.DebugName = "Skybox";
			pipelineSpec.Shader = skyboxShader;
			pipelineSpec.DepthWrite = false;
			pipelineSpec.DepthTest = false;
			pipelineSpec.Layout = {
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 },
			};
			pipelineSpec.RenderPass = m_GeometryPipeline->GetSpecification().RenderPass;
			m_SkyboxPipeline = VertexArray::Create(pipelineSpec);
			m_SkyboxMaterial = Material::Create(pipelineSpec.Shader, pipelineSpec.DebugName);
			m_SkyboxMaterial->SetFlag(MaterialFlag::DepthTest, false);
		}

		// TODO: resizeable/flushable
		const size_t TransformBufferCount = 10 * 1024; // 10240 transforms
		m_SubmeshTransformBuffers.resize(framesInFlight);
		for (uint32 i = 0; i < framesInFlight; i++)
		{
			m_SubmeshTransformBuffers[i].Buffer = VertexBuffer::Create(sizeof(TransformVertexData) * TransformBufferCount);
			m_SubmeshTransformBuffers[i].Data = new TransformVertexData[TransformBufferCount];
		}

		const size_t BoneTransformBufferCount = 1 * 1024; // basically means limited to 1024 animated meshes   TODO: resizeable/flushable
		m_BoneTransformStorageBuffers.resize(Renderer::GetConfig().FramesInFlight);
		for (auto &buffer : m_BoneTransformStorageBuffers)
		{
			buffer = StorageBuffer::Create(static_cast<uint32>(sizeof(BoneTransforms) * BoneTransformBufferCount), 0);
		}
		m_BoneTransformsData = new BoneTransforms[BoneTransformBufferCount];

		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance]() mutable { instance->m_ResourcesCreatedGPU = true; });

		InitMaterials();
		InitOptions();
	}

	void SceneRenderer::InitMaterials()
	{
		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->m_HBAOBlurMaterials[0]->Set("u_InputTex", instance->m_ReinterleavingPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage().As<Texture2D>());
			instance->m_HBAOBlurMaterials[1]->Set("u_InputTex", instance->m_HBAOBlurPipelines[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage().As<Texture2D>());

			instance->m_GTAODenoiseMaterial[0]->Set("u_Edges", instance->m_GTAOEdgesOutputImage);
			instance->m_GTAODenoiseMaterial[0]->Set("u_AOTerm", instance->m_GTAOOutputImage);
			instance->m_GTAODenoiseMaterial[0]->Set("o_AOTerm", instance->m_GTAODenoiseImage);

			instance->m_GTAODenoiseMaterial[1]->Set("u_Edges", instance->m_GTAOEdgesOutputImage);
			instance->m_GTAODenoiseMaterial[1]->Set("u_AOTerm", instance->m_GTAODenoiseImage);
			instance->m_GTAODenoiseMaterial[1]->Set("o_AOTerm", instance->m_GTAOOutputImage);
		});
	}

	void SceneRenderer::Shutdown()
	{
		delete[] m_BoneTransformsData;
		for (auto &transformBuffer : m_SubmeshTransformBuffers)
			delete[] transformBuffer.Data;
	}

	void SceneRenderer::InitOptions()
	{
		//TODO: Deserialization?
		/*
		if (m_Options.EnableGTAO)
			*(int *)&m_Options.ReflectionOcclusionMethod |= (int)ShaderDef::AOMethod::GTAO;
		if (m_Options.EnableHBAO)
			*(int *)&m_Options.ReflectionOcclusionMethod |= (int)ShaderDef::AOMethod::HBAO;
		*/

		// Special macros are strictly starting with "__HL_"
	//	Renderer::SetGlobalMacroInShaders("__HL_REFLECTION_OCCLUSION_METHOD", fmt::format("{}", (int)m_Options.ReflectionOcclusionMethod));
	//	Renderer::SetGlobalMacroInShaders("__HL_AO_METHOD", fmt::format("{}", ShaderDef::GetAOMethod(m_Options.EnableGTAO, m_Options.EnableHBAO)));
	//	Renderer::SetGlobalMacroInShaders("__HL_GTAO_COMPUTE_BENT_NORMALS", fmt::format("{}", (int)m_Options.GTAOBentNormals));
	}

	void SceneRenderer::InsertGPUPerfMarker(Ref<CommandBuffer> renderCommandBuffer, const std::string &label, const glm::vec4 &markerColor)
	{
		Renderer::Submit([=]
		{
		//	Renderer::RT_InsertGPUPerfMarker(renderCommandBuffer, label, markerColor);
		});
	}

	void SceneRenderer::BeginGPUPerfMarker(Ref<CommandBuffer> renderCommandBuffer, const std::string &label, const glm::vec4 &markerColor)
	{
		Renderer::Submit([=]
		{
		//	Renderer::RT_BeginGPUPerfMarker(renderCommandBuffer, label, markerColor);
		});
	}

	void SceneRenderer::EndGPUPerfMarker(Ref<CommandBuffer> renderCommandBuffer)
	{
		Renderer::Submit([=]
		{
		//	Renderer::RT_EndGPUPerfMarker(renderCommandBuffer);
		});
	}

	void SceneRenderer::SetScene(const Ref<Scene> &scene)
	{
		HL_ASSERT(!m_Active, "Can't change scenes while rendering");
		m_Scene = scene;
	}

	void SceneRenderer::SetViewportSize(uint32 width, uint32 height)
	{
		width = (uint32)(width); // * m_Specification.Tiering.RendererScale);
		height = (uint32)(height); // * m_Specification.Tiering.RendererScale);

		if (m_ViewportWidth != width || m_ViewportHeight != height)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			m_InvViewportWidth = 1.f / (float)width;
			m_InvViewportHeight = 1.f / (float)height;
			m_NeedsResize = true;
		}
	}

	void SceneRenderer::UpdateHBAOData()
	{
		const auto &opts = m_Options;
		UBHBAOData &hbaoData = HBAODataUB;

		// radius
		const float meters2viewSpace = 1.0f;
		const float R = opts.HBAORadius * meters2viewSpace;
		const float R2 = R * R;
		hbaoData.NegInvR2 = -1.0f / R2;
		hbaoData.InvQuarterResolution = 1.f / glm::vec2{ (float)m_ViewportWidth / 4, (float)m_ViewportHeight / 4 };
		hbaoData.RadiusToScreen = R * 0.5f * (float)m_ViewportHeight / (tanf(m_SceneData.SceneCamera.FOV * 0.5f) * 2.0f);

		const float *P = glm::value_ptr(m_SceneData.SceneCamera.Camera.GetProjection());
		const glm::vec4 projInfoPerspective = {
				 2.0f / (P[4 * 0 + 0]),                  // (x) * (R - L)/N
				 2.0f / (P[4 * 1 + 1]),                  // (y) * (T - B)/N
				-(1.0f - P[4 * 2 + 0]) / P[4 * 0 + 0],   // L/N
				-(1.0f + P[4 * 2 + 1]) / P[4 * 1 + 1],   // B/N
		};
		hbaoData.PerspectiveInfo = projInfoPerspective;
		hbaoData.IsOrtho = m_SceneData.SceneCamera.Camera.IsOrthographic();
		hbaoData.PowExponent = glm::max(opts.HBAOIntensity, 0.f);
		hbaoData.NDotVBias = glm::min(std::max(0.f, opts.HBAOBias), 1.f);
		hbaoData.AOMultiplier = 1.f / (1.f - hbaoData.NDotVBias);
		hbaoData.ShadowTolerance = m_Options.AOShadowTolerance;
	}

	// Some other settings are directly set in gui
	void SceneRenderer::UpdateGTAOData()
	{
		CBGTAOData &gtaoData = GTAODataCB;
		gtaoData.NDCToViewMul_x_PixelSize = { CameraDataUB.NDCToViewMul * (gtaoData.HalfRes ? ScreenDataUB.InvHalfResolution : ScreenDataUB.InvFullResolution) };
		gtaoData.HZBUVFactor = m_SSROptions.HZBUvFactor;
		gtaoData.ShadowTolerance = m_Options.AOShadowTolerance;
	}

	void SceneRenderer::BeginScene(const Camera &camera)
	{
		HL_ASSERT(m_Scene);
		HL_ASSERT(!m_Active);
		m_Active = true;

		const bool updatedAnyShaders = Renderer::UpdateDirtyShaders();
		if (updatedAnyShaders)
			InitMaterials();

		if (m_ResourcesCreatedGPU)
			m_ResourcesCreated = true;

		if (!m_ResourcesCreated)
			return;

		m_GTAOFinalImage = m_Options.GTAODenoisePasses && m_Options.GTAODenoisePasses % 2 != 0 ? m_GTAODenoiseImage : m_GTAOOutputImage;

		SceneRendererCamera rendererCam;
		rendererCam.Camera = camera;
		rendererCam.ViewMatrix = camera.GetViewMatrix();
		rendererCam.FOV = camera.GetPerspectiveFOV();
		rendererCam.Far = camera.GetPerspectiveFarPlane();
		rendererCam.Near = camera.GetPerspectiveNearPlane();

		m_SceneData.SceneCamera = rendererCam;
		m_SceneData.SceneEnvironment = m_Scene->m_Environment;
		m_SceneData.SceneEnvironmentIntensity = m_Scene->m_EnvironmentIntensity;
		m_SceneData.ActiveLight = m_Scene->GetLight();
		m_SceneData.SceneLightEnvironment = m_Scene->m_LightEnvironment;
		m_SceneData.SkyboxLod = m_Scene->m_SkyboxLod;

		if (m_NeedsResize)
		{
			m_NeedsResize = false;

			const glm::uvec2 viewportSize = { m_ViewportWidth, m_ViewportHeight };

			ScreenDataUB.FullResolution = { m_ViewportWidth, m_ViewportHeight };
			ScreenDataUB.InvFullResolution = { m_InvViewportWidth,  m_InvViewportHeight };
			ScreenDataUB.HalfResolution = glm::ivec2{ m_ViewportWidth,  m_ViewportHeight } / 2;
			ScreenDataUB.InvHalfResolution = { m_InvViewportWidth * 2.0f,  m_InvViewportHeight * 2.0f };

			// Both Pre-depth and geometry framebuffers need to be resized first.
			// Note the _Anim variants of these pipelines share the same framebuffer
			m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			m_PreDepthTransparentPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			m_SelectedGeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

			//Dependent on Geometry 
			m_SSRCompositePipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

			m_VisibilityTexture->Resize(m_ViewportWidth, m_ViewportHeight);
			m_ReinterleavingPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			m_HBAOBlurPipelines[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			m_HBAOBlurPipelines[1]->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
			m_AOCompositePipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight); //Only resize after geometry framebuffer
			m_CompositePipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

			if (m_DOFPipeline)
				m_DOFPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

			if (m_ReadBackImage)
				m_ReadBackImage->Resize({ m_ViewportWidth, m_ViewportHeight });

			// HZB
		//	{
		//		//HZB size must be power of 2's
		//		const glm::uvec2 numMips = glm::ceil(glm::log2(glm::vec2(viewportSize)));
		//		m_SSROptions.NumDepthMips = glm::max(numMips.x, numMips.y);
		//
		//		const glm::uvec2 hzbSize = HL_BIT(numMips);
		//		m_HierarchicalDepthTexture->Resize(hzbSize.x, hzbSize.y);
		//
		//		const glm::vec2 hzbUVFactor = { (glm::vec2)viewportSize / (glm::vec2)hzbSize };
		//		m_SSROptions.HZBUvFactor = hzbUVFactor;
		//	}

			// Light culling
			{
				constexpr uint32 TILE_SIZE = 16u;
				glm::uvec2 size = viewportSize;
				size += TILE_SIZE - viewportSize % TILE_SIZE;
				m_LightCullingWorkGroups = { size / TILE_SIZE, 1 };
				RendererDataUB.TilesCountX = m_LightCullingWorkGroups.x;

				m_StorageBufferSet->Resize(14, 0, m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * 4 * 1024);
				m_StorageBufferSet->Resize(23, 0, m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * 4 * 1024);
			}

			// GTAO
			{
				glm::uvec2 gtaoSize = GTAODataCB.HalfRes ? (viewportSize + 1u) / 2u : viewportSize;
				glm::uvec2 denoiseSize = gtaoSize;
				const TextureFormat gtaoImageFormat = m_Options.GTAOBentNormals ? TextureFormat::RED32UI : TextureFormat::RED8UI;
				m_GTAOOutputImage->GetSpecification().Format = gtaoImageFormat;
				m_GTAODenoiseImage->GetSpecification().Format = gtaoImageFormat;

				m_GTAOOutputImage->Resize(gtaoSize);
				m_GTAODenoiseImage->Resize(gtaoSize);
				m_GTAOEdgesOutputImage->Resize(gtaoSize);

				constexpr uint32 WORK_GROUP_SIZE = 16u;
				gtaoSize += WORK_GROUP_SIZE - gtaoSize % WORK_GROUP_SIZE;
				m_GTAOWorkGroups.x = gtaoSize.x / WORK_GROUP_SIZE;
				m_GTAOWorkGroups.y = gtaoSize.y / WORK_GROUP_SIZE;

				constexpr uint32 DENOISE_WORK_GROUP_SIZE = 8u;
				denoiseSize += DENOISE_WORK_GROUP_SIZE - denoiseSize % DENOISE_WORK_GROUP_SIZE;
				m_GTAODenoiseWorkGroups.x = (denoiseSize.x + 2u * DENOISE_WORK_GROUP_SIZE - 1u) / (DENOISE_WORK_GROUP_SIZE * 2u); // 2 horizontal pixels at a time.
				m_GTAODenoiseWorkGroups.y = denoiseSize.y / DENOISE_WORK_GROUP_SIZE;
			}

			// Quarter-size hbao textures
			{
				glm::uvec2 quarterSize = (viewportSize + 3u) / 4u;
				m_DeinterleavingPipelines[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(quarterSize.x, quarterSize.y);
				m_DeinterleavingPipelines[1]->GetSpecification().RenderPass->GetSpecification().Framebuffer->Resize(quarterSize.x, quarterSize.y);
				m_HBAOOutputImage->Resize(quarterSize);
				m_HBAOOutputImage->CreatePerLayerImageViews();

				constexpr uint32 WORK_GROUP_SIZE = 16u;
				quarterSize += WORK_GROUP_SIZE - quarterSize % WORK_GROUP_SIZE;
				m_HBAOWorkGroups.x = quarterSize.x / 16u;
				m_HBAOWorkGroups.y = quarterSize.y / 16u;
				m_HBAOWorkGroups.z = 16;
			}

			//SSR
			{
				constexpr uint32 WORK_GROUP_SIZE = 8u;
				glm::uvec2 ssrSize = m_SSROptions.HalfRes ? (viewportSize + 1u) / 2u : viewportSize;
				m_SSRImage->Resize(ssrSize);
				m_PreConvolutedTexture->Resize(ssrSize.x, ssrSize.y);
				ssrSize += WORK_GROUP_SIZE - ssrSize % WORK_GROUP_SIZE;
				m_SSRWorkGroups.x = ssrSize.x / WORK_GROUP_SIZE;
				m_SSRWorkGroups.y = ssrSize.y / WORK_GROUP_SIZE;
			}

			//Bloom
			{
				glm::uvec2 bloomSize = (viewportSize + 1u) / 2u;
				bloomSize += m_BloomComputeWorkgroupSize - bloomSize % m_BloomComputeWorkgroupSize;
				m_BloomComputeTextures[0]->Resize(bloomSize);
				m_BloomComputeTextures[1]->Resize(bloomSize);
				m_BloomComputeTextures[2]->Resize(bloomSize);
			}

			for (auto &tempFB : m_TempFramebuffers)
				tempFB->Resize(m_ViewportWidth, m_ViewportHeight);

			if (m_ExternalCompositeRenderPass)
				m_ExternalCompositeRenderPass->GetSpecification().Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
		}

		// Update uniform buffers
		UBCamera &cameraData = CameraDataUB;
		UBScene &sceneData = SceneDataUB;
		UBShadow &shadowData = ShadowData;
		UBRendererData &rendererData = RendererDataUB;
		UBPointLights &pointLightData = PointLightsUB;
		UBHBAOData &hbaoData = HBAODataUB;
		UBScreenData &screenData = ScreenDataUB;
		UBSpotLights &spotLightData = SpotLightUB;
		UBSpotShadowData &spotShadowData = SpotShadowDataUB;

		auto &sceneCamera = m_SceneData.SceneCamera;
		const auto viewProjection = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;
		const glm::mat4 viewInverse = glm::inverse(sceneCamera.ViewMatrix);
		const glm::mat4 projectionInverse = glm::inverse(sceneCamera.Camera.GetProjection());
		const glm::vec3 cameraPosition = viewInverse[3];

		cameraData.ViewProjection = viewProjection;
		cameraData.Projection = sceneCamera.Camera.GetProjection();
		cameraData.InverseProjection = projectionInverse;
		cameraData.View = sceneCamera.ViewMatrix;
		cameraData.InverseView = viewInverse;
		cameraData.InverseViewProjection = viewInverse * cameraData.InverseProjection;

		float depthLinearizeMul = (-cameraData.Projection[3][2]);     // float depthLinearizeMul = ( clipFar * clipNear ) / ( clipFar - clipNear );
		float depthLinearizeAdd = (cameraData.Projection[2][2]);     // float depthLinearizeAdd = clipFar / ( clipFar - clipNear );
		// correct the handedness issue.
		if (depthLinearizeMul * depthLinearizeAdd < 0)
			depthLinearizeAdd = -depthLinearizeAdd;
		cameraData.DepthUnpackConsts = { depthLinearizeMul, depthLinearizeAdd };
		const float *P = glm::value_ptr(m_SceneData.SceneCamera.Camera.GetProjection());
		const glm::vec4 projInfoPerspective = {
				 2.0f / (P[4 * 0 + 0]),                  // (x) * (R - L)/N
				 2.0f / (P[4 * 1 + 1]),                  // (y) * (T - B)/N
				-(1.0f - P[4 * 2 + 0]) / P[4 * 0 + 0],  // L/N
				-(1.0f + P[4 * 2 + 1]) / P[4 * 1 + 1],  // B/N
		};
		float tanHalfFOVY = 1.0f / cameraData.Projection[1][1];    // = tanf( drawContext.Camera.GetYFOV( ) * 0.5f );
		float tanHalfFOVX = 1.0f / cameraData.Projection[0][0];    // = tanHalfFOVY * drawContext.Camera.GetAspect( );
		cameraData.CameraTanHalfFOV = { tanHalfFOVX, tanHalfFOVY };
		cameraData.NDCToViewMul = { projInfoPerspective[0], projInfoPerspective[1] };
		cameraData.NDCToViewAdd = { projInfoPerspective[2], projInfoPerspective[3] };

		Ref<SceneRenderer> instance = this;
		Renderer::Submit([instance, cameraData]() mutable
		{
			uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			instance->m_UniformBufferSet->GetUniform(Binding::CameraData, 0, bufferIndex)->SetData(&cameraData, sizeof(cameraData));
		});

		const auto lightEnvironment = m_SceneData.SceneLightEnvironment;
		const std::vector<PointLight> &pointLightsVec = lightEnvironment.PointLights;
		pointLightData.Count = int(pointLightsVec.size());
		std::memcpy(pointLightData.PointLights, pointLightsVec.data(), lightEnvironment.GetPointLightsSize()); // TODO: Do we really have to copy that?
		Renderer::Submit([instance, &pointLightData]() mutable
		{
			const uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			Ref<UniformBuffer> bufferSet = instance->m_UniformBufferSet->GetUniform(Binding::PointLightData, 0, bufferIndex);
			bufferSet->SetData(&pointLightData, 16ull + sizeof(PointLight) * pointLightData.Count);
		});

		const std::vector<SpotLight> &spotLightsVec = lightEnvironment.SpotLights;
		spotLightData.Count = int32(spotLightsVec.size());
		std::memcpy(spotLightData.SpotLights, spotLightsVec.data(), lightEnvironment.GetSpotLightsSize()); //(Karim) Do we really have to copy that?
		Renderer::Submit([instance, &spotLightData]() mutable
		{
			const uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			Ref<UniformBuffer> bufferSet = instance->m_UniformBufferSet->GetUniform(19, 0, bufferIndex);
			bufferSet->SetData(&spotLightData, 16ull + sizeof(SpotLight) * spotLightData.Count);
		});

		for (size_t i = 0; i < spotLightsVec.size(); ++i)
		{
			auto &light = spotLightsVec[i];
			if (!light.CastsShadows)
				continue;

			glm::mat4 projection = glm::perspective(glm::radians(light.Angle), 1.f, 0.1f, light.Range);
			// NOTE(Yan): ShadowMatrices[0] because we only support ONE shadow casting spot light at the moment and it MUST be index 0
			spotShadowData.ShadowMatrices[0] = projection * glm::lookAt(light.Position, light.Position - light.Direction, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		Renderer::Submit([instance, spotShadowData, spotLightsVec]() mutable
		{
			const uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			Ref<UniformBuffer> bufferSet = instance->m_UniformBufferSet->GetUniform(20, 0, bufferIndex);
			bufferSet->SetData(&spotShadowData, (uint32)(sizeof(glm::mat4) * spotLightsVec.size()));
		});

		const auto &directionalLight = m_SceneData.SceneLightEnvironment.DirectionalLights[0];
		sceneData.Lights.Direction = directionalLight.Direction;
		sceneData.Lights.Radiance = directionalLight.Radiance;
		sceneData.Lights.Intensity = directionalLight.Intensity;
		sceneData.Lights.ShadowAmount = directionalLight.ShadowAmount;

		sceneData.CameraPosition = cameraPosition;
		sceneData.EnvironmentMapIntensity = m_SceneData.SceneEnvironmentIntensity;
		Renderer::Submit([instance, sceneData]() mutable
		{
			uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			instance->m_UniformBufferSet->GetUniform(Binding::SceneData, 0, bufferIndex)->SetData(&sceneData, sizeof(sceneData));
		});

		if (m_Options.EnableHBAO)
			UpdateHBAOData();
		if (m_Options.EnableGTAO)
			UpdateGTAOData();

		Renderer::Submit([instance, hbaoData]() mutable
		{
			const uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			instance->m_UniformBufferSet->GetUniform(Binding::HBAOData, 0, bufferIndex)->SetData(&hbaoData, sizeof(hbaoData));
		});

		Renderer::Submit([instance, screenData]() mutable
		{
			const uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			instance->m_UniformBufferSet->GetUniform(Binding::ScreenData, 0, bufferIndex)->SetData(&screenData, sizeof(screenData));
		});

		CascadeData cascades[4];
		if (m_UseManualCascadeSplits)
			CalculateCascadesManualSplit(cascades, sceneCamera, directionalLight.Direction);
		else
			CalculateCascades(cascades, sceneCamera, directionalLight.Direction);


		// TODO: four cascades for now
		for (int32 i = 0; i < 4; i++)
		{
			CascadeSplits[i] = cascades[i].SplitDepth;
			shadowData.ViewProjection[i] = cascades[i].ViewProj;
		}
		Renderer::Submit([instance, shadowData]() mutable
		{
			const uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			instance->m_UniformBufferSet->GetUniform(Binding::ShadowData, 0, bufferIndex)->SetData(&shadowData, sizeof(shadowData));
		});

		rendererData.CascadeSplits = CascadeSplits;
		Renderer::Submit([instance, rendererData]() mutable
		{
			const uint32 bufferIndex = Renderer::GetCurrentFrameIndex();
			instance->m_UniformBufferSet->GetUniform(Binding::RendererData, 0, bufferIndex)->SetData(&rendererData, sizeof(rendererData));
		});

		Renderer::SetSceneEnvironment(this,
									  m_SceneData.SceneEnvironment,
									  nullptr);
									  //m_ShadowPassPipelines[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage(),
									  //m_SpotShadowPassPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage());
	}

	void SceneRenderer::EndScene()
	{
		HL_ASSERT(m_Active);
#if MULTI_THREAD
		Ref<SceneRenderer> instance = this;
		s_ThreadPool.emplace_back(([instance]() mutable
		{
			instance->FlushDrawList();
		}));
#else 
		FlushDrawList();
#endif

		m_Active = false;
	}

	void SceneRenderer::WaitForThreads()
	{
		for (uint32 i = 0; i < s_ThreadPool.size(); i++)
			s_ThreadPool[i].join();

		s_ThreadPool.clear();
	}

	void SceneRenderer::SubmitStaticModel(const Ref<StaticModel> &model, const Ref<MaterialTable> &materialTable, const glm::mat4 &transform, const Ref<Material> &overrideMaterial)
	{
		Ref<MeshFile> meshSource = model->Get();
		const auto &submeshData = meshSource->GetSubmeshes();
		for (uint32 submeshIndex : model->GetSubmeshes())
		{
			glm::mat4 submeshTransform = transform * submeshData[submeshIndex].LocalTransform;

			const auto &submeshes = model->Get()->GetSubmeshes();
			uint32 materialIndex = submeshes[submeshIndex].MaterialIndex;

			AssetHandle materialHandle = materialTable->HasMaterial(materialIndex) 
				? materialTable->GetMaterial(materialIndex) 
				: model->GetMaterials()->GetMaterial(materialIndex);
			
			HL_ASSERT(materialHandle);
			Ref<MaterialAsset> material = AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle);

			MeshKey meshKey = { model->Handle, materialHandle, submeshIndex, false };
			auto &transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();

			transformStorage.MRow[0] = { submeshTransform[0][0], submeshTransform[1][0], submeshTransform[2][0], submeshTransform[3][0] };
			transformStorage.MRow[1] = { submeshTransform[0][1], submeshTransform[1][1], submeshTransform[2][1], submeshTransform[3][1] };
			transformStorage.MRow[2] = { submeshTransform[0][2], submeshTransform[1][2], submeshTransform[2][2], submeshTransform[3][2] };

			// Main geo
			{
				bool isTransparent = material->IsTransparent();
				auto &destDrawList = !isTransparent ? m_StaticMeshDrawList : m_TransparentStaticMeshDrawList;
				StaticDrawCommand &dc = destDrawList[meshKey];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.MaterialTable = materialTable;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
			}

			// Shadow pass
			if (material->IsShadowCasting())
			{
				auto &dc = m_StaticMeshShadowPassDrawList[meshKey];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.MaterialTable = materialTable;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
			}
		}
	}

	void SceneRenderer::SubmitDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materials, const glm::mat4 &transform, const Ref<Material> &overrideMaterial, const std::vector<glm::mat4> &boneTransforms)
	{
		// TODO: Culling, sorting, etc.

		const Ref<MeshFile> meshSource = model->Get();
		const std::vector<Mesh> &submeshes = meshSource->GetSubmeshes();
		const Mesh &submesh = submeshes[submeshIndex];
		uint32 materialIndex = submesh.MaterialIndex;
		bool isRigged = submesh.IsRigged;

		AssetHandle materialHandle = materials->HasMaterial(materialIndex)
			? materials->GetMaterial(materialIndex)
			: model->GetMaterials()->GetMaterial(materialIndex);

		Ref<MaterialAsset> material = AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle);

		MeshKey meshKey = { model->Handle, materialHandle, submeshIndex, false };
		auto &transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();

		transformStorage.MRow[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
		transformStorage.MRow[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
		transformStorage.MRow[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

		if (isRigged)
		{
			CopyToBoneTransformStorage(meshKey, meshSource, boneTransforms);
		}
		// Main geo
		{
			bool isTransparent = material->IsTransparent();
			auto &destDrawList = !isTransparent ? m_DrawList : m_TransparentDrawList;
			DynamicDrawCommand &dc = destDrawList[meshKey];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.MaterialTable = materials;
			dc.OverrideMaterial = overrideMaterial;
			dc.InstanceCount++;
			dc.IsRigged = isRigged;  // TODO: would it be better to have separate draw list for rigged meshes, or this flag is OK?
		}

		// Shadow pass
		if (material->IsShadowCasting())
		{
			auto &dc = m_ShadowPassDrawList[meshKey];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.MaterialTable = materials;
			dc.OverrideMaterial = overrideMaterial;
			dc.InstanceCount++;
			dc.IsRigged = isRigged;
		}
	}

	void SceneRenderer::SubmitSelectedStaticModel(const Ref<StaticModel> &model, const Ref<MaterialTable> &materialTable, const glm::mat4 &transform, const Ref<Material> &overrideMaterial)
	{
		Ref<MeshFile> meshSource = model->Get();
		const auto &submeshData = meshSource->GetSubmeshes();
		for (uint32 submeshIndex : model->GetSubmeshes())
		{
			glm::mat4 submeshTransform = transform * submeshData[submeshIndex].LocalTransform;

			const auto &submeshes = model->Get()->GetSubmeshes();
			uint32 materialIndex = submeshes[submeshIndex].MaterialIndex;

			AssetHandle materialHandle = materialTable->HasMaterial(materialIndex) 
				? materialTable->GetMaterial(materialIndex) 
				: model->GetMaterials()->GetMaterial(materialIndex);
			
			HL_ASSERT(materialHandle);
			Ref<MaterialAsset> material = AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle);

			MeshKey meshKey = { model->Handle, materialHandle, submeshIndex, true };
			auto &transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();

			transformStorage.MRow[0] = { submeshTransform[0][0], submeshTransform[1][0], submeshTransform[2][0], submeshTransform[3][0] };
			transformStorage.MRow[1] = { submeshTransform[0][1], submeshTransform[1][1], submeshTransform[2][1], submeshTransform[3][1] };
			transformStorage.MRow[2] = { submeshTransform[0][2], submeshTransform[1][2], submeshTransform[2][2], submeshTransform[3][2] };

			// Main geo
			{
				bool isTransparent = material->IsTransparent();
				auto &destDrawList = !isTransparent ? m_StaticMeshDrawList : m_TransparentStaticMeshDrawList;
				auto &dc = destDrawList[meshKey];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.MaterialTable = materialTable;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
			}

			// Selected mesh list
			{
				auto &dc = m_SelectedStaticMeshDrawList[meshKey];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.MaterialTable = materialTable;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
			}

			// Shadow pass
			if (material->IsShadowCasting())
			{
				auto &dc = m_StaticMeshShadowPassDrawList[meshKey];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.MaterialTable = materialTable;
				dc.OverrideMaterial = overrideMaterial;
				dc.InstanceCount++;
			}
		}
	}

	void SceneRenderer::SubmitSelectedDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, const Ref<MaterialTable> &materialTable, const glm::mat4 &transform, const Ref<Material> &overrideMaterial, const std::vector<glm::mat4> &boneTransforms)
	{
		// TODO: Culling, sorting, etc.

		const auto meshSource = model->Get();
		const auto &submeshes = meshSource->GetSubmeshes();
		const auto &submesh = submeshes[submeshIndex];
		uint32 materialIndex = submesh.MaterialIndex;
		bool isRigged = submesh.IsRigged;

		AssetHandle materialHandle = materialTable->HasMaterial(materialIndex) 
			? materialTable->GetMaterial(materialIndex) 
			: model->GetMaterials()->GetMaterial(materialIndex);
		
		HL_ASSERT(materialHandle);
		Ref<MaterialAsset> material = AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle);

		MeshKey meshKey = { model->Handle, materialHandle, submeshIndex, true };
		auto &transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();

		transformStorage.MRow[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
		transformStorage.MRow[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
		transformStorage.MRow[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

		if (isRigged)
		{
			CopyToBoneTransformStorage(meshKey, meshSource, boneTransforms);
		}

		uint32 instanceIndex = 0;

		// Main geo
		{
			bool isTransparent = material->IsTransparent();
			auto &destDrawList = !isTransparent ? m_DrawList : m_TransparentDrawList;
			auto &dc = destDrawList[meshKey];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.MaterialTable = materialTable;
			dc.OverrideMaterial = overrideMaterial;

			instanceIndex = dc.InstanceCount;
			dc.InstanceCount++;
			dc.IsRigged = isRigged;
		}

		// Selected mesh list
		{
			auto &dc = m_SelectedMeshDrawList[meshKey];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.MaterialTable = materialTable;
			dc.OverrideMaterial = overrideMaterial;
			dc.InstanceCount++;
			dc.InstanceOffset = instanceIndex;
			dc.IsRigged = isRigged;
		}

		// Shadow pass
		if (material->IsShadowCasting())
		{
			auto &dc = m_ShadowPassDrawList[meshKey];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.MaterialTable = materialTable;
			dc.OverrideMaterial = overrideMaterial;
			dc.InstanceCount++;
			dc.IsRigged = isRigged;
		}
	}

	void SceneRenderer::SubmitPhysicsDebugStaticModel(const Ref<StaticModel> &model, const glm::mat4 &transform, const bool isPrimitiveCollider)
	{
		HL_ASSERT(model->Handle);
		Ref<MeshFile> meshSource = model->Get();
		const auto &submeshData = meshSource->GetSubmeshes();
		for (uint32 submeshIndex : model->GetSubmeshes())
		{
			glm::mat4 submeshTransform = transform * submeshData[submeshIndex].LocalTransform;

			MeshKey meshKey = { model->Handle, 5, submeshIndex, false };
			auto &transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();

			transformStorage.MRow[0] = { submeshTransform[0][0], submeshTransform[1][0], submeshTransform[2][0], submeshTransform[3][0] };
			transformStorage.MRow[1] = { submeshTransform[0][1], submeshTransform[1][1], submeshTransform[2][1], submeshTransform[3][1] };
			transformStorage.MRow[2] = { submeshTransform[0][2], submeshTransform[1][2], submeshTransform[2][2], submeshTransform[3][2] };

			{
				auto &dc = m_StaticColliderDrawList[meshKey];
				dc.Model = model;
				dc.SubmeshIndex = submeshIndex;
				dc.OverrideMaterial = isPrimitiveCollider ? m_SimpleColliderMaterial : m_ComplexColliderMaterial;
				dc.InstanceCount++;
			}

		}
	}

	void SceneRenderer::SubmitPhysicsDebugDynamicModel(const Ref<DynamicModel> &model, uint32 submeshIndex, const glm::mat4 &transform)
	{
		Ref<MeshFile> meshSource = model->Get();
		const auto &submeshData = meshSource->GetSubmeshes();
		glm::mat4 submeshTransform = transform * submeshData[submeshIndex].LocalTransform;

		MeshKey meshKey = { model->Handle, 5, submeshIndex, false };
		auto &transformStorage = m_MeshTransformMap[meshKey].Transforms.emplace_back();

		transformStorage.MRow[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
		transformStorage.MRow[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
		transformStorage.MRow[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

		{
			auto &dc = m_ColliderDrawList[meshKey];
			dc.Model = model;
			dc.SubmeshIndex = submeshIndex;
			dc.InstanceCount++;
		}
	}

	void SceneRenderer::ClearPass(Ref<RenderPass> renderPass, bool explicitClear)
	{
		Renderer::BeginRenderPass(m_CommandBuffer, renderPass, explicitClear);
		Renderer::EndRenderPass(m_CommandBuffer);
	}

	void SceneRenderer::ShadowMapPass()
	{
#if 0
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		m_GPUTimeQueries.DirShadowMapPassQuery = m_CommandBuffer->BeginTimestampQuery();

		auto &directionalLights = m_SceneData.SceneLightEnvironment.DirectionalLights;
		if (directionalLights[0].Intensity == 0.0f || !directionalLights[0].CastShadows)
		{
			// Clear shadow maps
			for (int i = 0; i < 4; i++)
				ClearPass(m_ShadowPassPipelines[i]->GetSpecification().RenderPass);

			return;
		}

		// TODO: change to four cascades (or set number)
		for (int i = 0; i < 4; i++)
		{
			Renderer::BeginRenderPass(m_CommandBuffer, m_ShadowPassPipelines[i]->GetSpecification().RenderPass);

			// static glm::mat4 scaleBiasMatrix = glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f }) * glm::translate(glm::mat4(1.0f), { 1, 1, 1 });

			// Render entities
			const Buffer cascade(&i, sizeof(uint32));
			for (auto &[mk, dc] : m_StaticMeshShadowPassDrawList)
			{
				HL_ASSERT(m_MeshTransformMap.find(mk) != m_MeshTransformMap.end());
				const auto &transformData = m_MeshTransformMap.at(mk);
				Renderer::RenderStaticMeshWithMaterial(m_CommandBuffer, m_ShadowPassPipelines[i], m_UniformBufferSet, nullptr, dc.StaticMesh, dc.SubmeshIndex, m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, dc.InstanceCount, m_ShadowPassMaterial, cascade);
			}
			for (auto &[mk, dc] : m_ShadowPassDrawList)
			{
				HL_ASSERT(m_MeshTransformMap.find(mk) != m_MeshTransformMap.end());
				const auto &transformData = m_MeshTransformMap.at(mk);
				if (dc.IsRigged)
				{
					const auto &boneTransformsData = m_MeshBoneTransformsMap.at(mk);
					Renderer::RenderMeshWithMaterial(m_CommandBuffer, m_ShadowPassPipelinesAnim[i], m_UniformBufferSet, nullptr, dc.Mesh, dc.SubmeshIndex, m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, m_BoneTransformStorageBuffers, boneTransformsData.BoneTransformsBaseIndex, dc.InstanceCount, m_ShadowPassMaterial, cascade);
				}
				else
				{
					Renderer::RenderMeshWithMaterial(m_CommandBuffer, m_ShadowPassPipelines[i], m_UniformBufferSet, nullptr, dc.Mesh, dc.SubmeshIndex, m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, {}, 0, dc.InstanceCount, m_ShadowPassMaterial, cascade);
				}
			}

			Renderer::EndRenderPass(m_CommandBuffer);
		}

		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.DirShadowMapPassQuery);
#endif
	}

	void SceneRenderer::SpotShadowMapPass()
	{
#if 0
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		m_GPUTimeQueries.SpotShadowMapPassQuery = m_CommandBuffer->BeginTimestampQuery();

		// Spot shadow maps
		Renderer::BeginRenderPass(m_CommandBuffer, m_SpotShadowPassPipeline->GetSpecification().RenderPass);
		for (uint32 i = 0; i < 1; i++)
		{

			const Buffer lightIndex(&i, sizeof(uint32));
			for (auto &[mk, dc] : m_StaticMeshShadowPassDrawList)
			{
				HL_ASSERT(m_MeshTransformMap.find(mk) != m_MeshTransformMap.end());
				const auto &transformData = m_MeshTransformMap.at(mk);
				Renderer::RenderStaticMeshWithMaterial(m_CommandBuffer, m_SpotShadowPassPipeline, m_UniformBufferSet, nullptr, dc.StaticMesh, dc.SubmeshIndex, m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, dc.InstanceCount, m_SpotShadowPassMaterial, lightIndex);
			}
			for (auto &[mk, dc] : m_ShadowPassDrawList)
			{
				HL_ASSERT(m_MeshTransformMap.find(mk) != m_MeshTransformMap.end());
				const auto &transformData = m_MeshTransformMap.at(mk);
				if (dc.IsRigged)
				{
					const auto &boneTransformsData = m_MeshBoneTransformsMap.at(mk);
					Renderer::RenderMeshWithMaterial(m_CommandBuffer, m_SpotShadowPassAnimPipeline, m_UniformBufferSet, nullptr, dc.Mesh, dc.SubmeshIndex, m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, m_BoneTransformStorageBuffers, boneTransformsData.BoneTransformsBaseIndex, dc.InstanceCount, m_SpotShadowPassMaterial, lightIndex);
				}
				else
				{
					Renderer::RenderMeshWithMaterial(m_CommandBuffer, m_SpotShadowPassPipeline, m_UniformBufferSet, nullptr, dc.Mesh, dc.SubmeshIndex, m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, {}, 0, dc.InstanceCount, m_SpotShadowPassMaterial, lightIndex);
				}
			}

		}
		Renderer::EndRenderPass(m_CommandBuffer);
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.SpotShadowMapPassQuery);
#endif
	}

	void SceneRenderer::PreDepthPass()
	{
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		m_GPUTimeQueries.DepthPrePassQuery = m_CommandBuffer->BeginTimestampQuery();
		SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "PreDepthPass");
		Renderer::BeginRenderPass(m_CommandBuffer, m_PreDepthPipeline->GetSpecification().RenderPass);
		
		for (auto &[mk, dc] : m_StaticMeshDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			Renderer::RenderInstancedStaticMeshWithMaterial(
				m_CommandBuffer, 
				m_PreDepthPipeline, 
				m_UniformBufferSet, nullptr, 
				dc.Model, dc.SubmeshIndex, 
				m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
				dc.InstanceCount, 
				m_PreDepthMaterial);
		}
		for (auto &[mk, dc] : m_DrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			if (dc.IsRigged)
			{
				const auto &boneTransformsData = m_MeshBoneTransformsMap.at(mk);
				Renderer::RenderInstancedDynamicMeshWithMaterial(
					m_CommandBuffer, 
					m_PreDepthPipelineAnim, 
					m_UniformBufferSet, nullptr, 
					dc.Model, dc.SubmeshIndex, 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, m_BoneTransformStorageBuffers, 
					boneTransformsData.BoneTransformsBaseIndex, 
					dc.InstanceCount, 
					m_PreDepthMaterial);
			}
			else
			{
				Renderer::RenderInstancedDynamicMeshWithMaterial(
					m_CommandBuffer, 
					m_PreDepthPipeline, 
					m_UniformBufferSet, nullptr, 
					dc.Model, dc.SubmeshIndex, 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
					{}, 
					0, 
					dc.InstanceCount, 
					m_PreDepthMaterial);
			}
		}

		Renderer::EndRenderPass(m_CommandBuffer);

		Renderer::BeginRenderPass(m_CommandBuffer, m_PreDepthTransparentPipeline->GetSpecification().RenderPass);
#if 1
		for (auto &[mk, dc] : m_TransparentStaticMeshDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			Renderer::RenderInstancedStaticMeshWithMaterial(
				m_CommandBuffer, 
				m_PreDepthTransparentPipeline, 
				m_UniformBufferSet, nullptr, 
				dc.Model, dc.SubmeshIndex, 
				m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
				{}, 
				0, 
				dc.InstanceCount, 
				m_PreDepthMaterial);
		}
		for (auto &[mk, dc] : m_TransparentDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			if (dc.IsRigged)
			{
				const auto &boneTransformsData = m_MeshBoneTransformsMap.at(mk);

				// TODO: This needs to be pre-depth transparent-anim pipeline
				Renderer::RenderInstancedDynamicMeshWithMaterial(
					m_CommandBuffer, 
					m_PreDepthPipelineAnim, 
					m_UniformBufferSet, nullptr, 
					dc.Model, dc.SubmeshIndex, 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
					m_BoneTransformStorageBuffers, boneTransformsData.BoneTransformsBaseIndex, 
					dc.InstanceCount, 
					m_PreDepthMaterial);
			}
			else
			{
				Renderer::RenderInstancedDynamicMeshWithMaterial(
					m_CommandBuffer, 
					m_PreDepthTransparentPipeline, 
					m_UniformBufferSet, nullptr, 
					dc.Model, dc.SubmeshIndex, 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
					{}, 
					0, 
					dc.InstanceCount, 
					m_PreDepthMaterial);
			}
		}
#endif

		Renderer::EndRenderPass(m_CommandBuffer);
		SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.DepthPrePassQuery);
	}

	void SceneRenderer::HZBCompute()
	{
#if 0
		Ref<VulkanComputePipeline> pipeline = m_HierarchicalDepthPipeline.As<VulkanComputePipeline>();

		auto srcDepthImage = m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().TargetFramebuffer->GetDepthImage().As<VulkanImage2D>();

		m_GPUTimeQueries.HierarchicalDepthQuery = m_CommandBuffer->BeginTimestampQuery();

		Renderer::Submit([srcDepthImage, commandBuffer = m_CommandBuffer, hierarchicalZTex = m_HierarchicalDepthTexture.As<VulkanTexture2D>(), material = m_HierarchicalDepthMaterial.As<VulkanMaterial>(), pipeline]() mutable
		{
			const VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

			Ref<VulkanImage2D> hierarchicalZImage = hierarchicalZTex->GetImage().As<VulkanImage2D>();

			auto shader = material->GetShader().As<VulkanShader>();

			VkDescriptorSetLayout descriptorSetLayout = shader->GetDescriptorSetLayout(0);

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &descriptorSetLayout;

			constexpr uint32 maxMipBatchSize = 4;
			const uint32 hzbMipCount = hierarchicalZTex->GetMipLevelCount();

			pipeline->RT_Begin(commandBuffer);

			auto ReduceHZB = [hzbMipCount, maxMipBatchSize, &shader, &hierarchicalZImage, &device, &pipeline, &allocInfo]
			(const uint32 startDestMip, const uint32 parentMip, Ref<VulkanImage2D> parentImage, const glm::vec2 &DispatchThreadIdToBufferUV, const glm::vec2 &InputViewportMaxBound, const bool isFirstPass)
			{
				const VkDescriptorSet descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
				struct HierarchicalZComputePushConstants
				{
					glm::vec2 DispatchThreadIdToBufferUV;
					glm::vec2 InputViewportMaxBound;
					glm::vec2 InvSize;
					int FirstLod;
					bool IsFirstPass;
					char Padding[3]{ 0, 0, 0 };
				} hierarchicalZComputePushConstants;

				hierarchicalZComputePushConstants.IsFirstPass = isFirstPass;
				hierarchicalZComputePushConstants.FirstLod = (int)startDestMip;
				hierarchicalZComputePushConstants.DispatchThreadIdToBufferUV = DispatchThreadIdToBufferUV;
				hierarchicalZComputePushConstants.InputViewportMaxBound = InputViewportMaxBound;


				std::array<VkWriteDescriptorSet, 5> writeDescriptors{};
				std::array<VkDescriptorImageInfo, 5> hzbImageInfos{};
				const uint32 endDestMip = glm::min(startDestMip + maxMipBatchSize, hzbMipCount);
				uint32 destMip;
				for (destMip = startDestMip; destMip < endDestMip; ++destMip)
				{
					uint32 idx = destMip - startDestMip;
					hzbImageInfos[idx] = hierarchicalZImage->GetDescriptorInfo();
					hzbImageInfos[idx].imageView = hierarchicalZImage->RT_GetMipImageView(destMip);
					hzbImageInfos[idx].sampler = VK_NULL_HANDLE;
				}

				// Fill the rest .. or we could enable the null descriptor feature
				destMip -= startDestMip;
				for (; destMip < maxMipBatchSize; ++destMip)
				{
					hzbImageInfos[destMip] = hierarchicalZImage->GetDescriptorInfo();
					hzbImageInfos[destMip].imageView = hierarchicalZImage->RT_GetMipImageView(hzbMipCount - 1);
					hzbImageInfos[destMip].sampler = VK_NULL_HANDLE;
				}

				for (uint32 i = 0; i < maxMipBatchSize; ++i)
				{
					writeDescriptors[i] = *shader->GetDescriptorSet("o_HZB");
					writeDescriptors[i].dstSet = descriptorSet; // Should this be set inside the shader?
					writeDescriptors[i].dstArrayElement = i;
					writeDescriptors[i].pImageInfo = &hzbImageInfos[i];
				}

				hzbImageInfos[4] = parentImage->GetDescriptorInfo();
				hzbImageInfos[4].sampler = VulkanRenderer::GetPointSampler();

				writeDescriptors[4] = *shader->GetDescriptorSet("u_InputDepth");
				writeDescriptors[4].dstSet = descriptorSet; // Should this be set inside the shader?
				writeDescriptors[4].pImageInfo = &hzbImageInfos[maxMipBatchSize];

				vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

				const glm::ivec2 srcSize{ Math::DivideAndRoundUp(parentImage->GetSize(), 1u << parentMip) };
				const glm::ivec2 dstSize = Math::DivideAndRoundUp(hierarchicalZImage->GetSize(), 1u << startDestMip);
				hierarchicalZComputePushConstants.InvSize = glm::vec2{ 1.0f / (float)srcSize.x, 1.0f / (float)srcSize.y };

				pipeline->SetPushConstants(&hierarchicalZComputePushConstants, sizeof(hierarchicalZComputePushConstants));

				pipeline->Dispatch(descriptorSet, Math::DivideAndRoundUp(dstSize.x, 8), Math::DivideAndRoundUp(dstSize.y, 8), 1);
			};
			glm::ivec2 srcSize = srcDepthImage->GetSize();

			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "HZB");

			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "HZB-FirstPass");
			// Reduce first 4 mips
			ReduceHZB(0, 0, srcDepthImage, { 1.0f / glm::vec2{ srcSize } }, { (glm::vec2{ srcSize } - 0.5f) / glm::vec2{ srcSize } }, true);
			Renderer::RT_EndGPUPerfMarker(commandBuffer);
			// Reduce the next mips
			for (uint32 startDestMip = maxMipBatchSize; startDestMip < hzbMipCount; startDestMip += maxMipBatchSize)
			{
				Renderer::RT_BeginGPUPerfMarker(commandBuffer, fmt::format("HZB-Pass({})", startDestMip));
				srcSize = Math::DivideAndRoundUp(hierarchicalZTex->GetSize(), 1u << uint32(startDestMip - 1));
				ReduceHZB(startDestMip, startDestMip - 1, hierarchicalZImage, { 2.0f / glm::vec2{ srcSize } }, glm::vec2{ 1.0f }, false);
				Renderer::RT_EndGPUPerfMarker(commandBuffer);
			}


			Renderer::RT_EndGPUPerfMarker(commandBuffer);
			pipeline->End();
		});
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.HierarchicalDepthQuery);
#endif
	}

	void SceneRenderer::PreIntegration()
	{
#if 0
		Ref<VulkanComputePipeline> pipeline = m_PreIntegrationPipeline.As<VulkanComputePipeline>();

		m_GPUTimeQueries.PreIntegrationQuery = m_CommandBuffer->BeginTimestampQuery();
		glm::vec2 projectionParams = { m_SceneData.SceneCamera.Far, m_SceneData.SceneCamera.Near }; // Reversed 
		Renderer::Submit([projectionParams, hzbUVFactor = m_SSROptions.HZBUvFactor, depthImage = m_HierarchicalDepthTexture->GetImage().As<VulkanImage2D>(), commandBuffer = m_CommandBuffer.As<VulkanRenderCommandBuffer>(),
						 visibilityTexture = m_VisibilityTexture, material = m_PreIntegrationMaterial.As<VulkanMaterial>(), pipeline]() mutable
		{
			const VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

			Ref<VulkanImage2D> visibilityImage = visibilityTexture->GetImage().As<VulkanImage2D>();

			struct PreIntegrationComputePushConstants
			{
				glm::vec2 HZBResFactor;
				glm::vec2 ResFactor;
				glm::vec2 ProjectionParams; //(x) = Near plane, (y) = Far plane
				int PrevLod = 0;
			} preIntegrationComputePushConstants;

			//Clear to 100% visibility .. TODO: this can be done once when the image is resized
			VkClearColorValue clearColor{ { 1.f } };
			VkImageSubresourceRange subresourceRange{};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.layerCount = 1;
			subresourceRange.levelCount = 1;
			vkCmdClearColorImage(commandBuffer->GetCommandBuffer(Renderer::RT_GetCurrentFrameIndex()), visibilityImage->GetImageInfo().Image, VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &subresourceRange);

			std::array<VkWriteDescriptorSet, 3> writeDescriptors{};

			auto shader = material->GetShader().As<VulkanShader>();
			VkDescriptorSetLayout descriptorSetLayout = shader->GetDescriptorSetLayout(0);

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &descriptorSetLayout;

			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "PreIntegration");
			pipeline->RT_Begin(commandBuffer);

			auto visibilityDescriptorImageInfo = visibilityImage->GetDescriptorInfo();
			auto depthDescriptorImageInfo = depthImage->GetDescriptorInfo();
			depthDescriptorImageInfo.sampler = VulkanRenderer::GetPointSampler();

			for (uint32 i = 1; i < visibilityTexture->GetMipLevelCount(); i++)
			{
				Renderer::RT_BeginGPUPerfMarker(commandBuffer, fmt::format("PreIntegration-Pass({})", i));
				auto [mipWidth, mipHeight] = visibilityTexture->GetMipSize(i);
				const auto workGroupsX = (uint32)glm::ceil((float)mipWidth / 8.0f);
				const auto workGroupsY = (uint32)glm::ceil((float)mipHeight / 8.0f);

				// Output visibilityImage
				visibilityDescriptorImageInfo.imageView = visibilityImage->RT_GetMipImageView(i);

				const VkDescriptorSet descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
				writeDescriptors[0] = *shader->GetDescriptorSet("o_VisibilityImage");
				writeDescriptors[0].dstSet = descriptorSet;
				writeDescriptors[0].pImageInfo = &visibilityDescriptorImageInfo;

				// Input visibilityImage
				writeDescriptors[1] = *shader->GetDescriptorSet("u_VisibilityTex");
				writeDescriptors[1].dstSet = descriptorSet;
				writeDescriptors[1].pImageInfo = &visibilityImage->GetDescriptorInfo();

				// Input HZB
				writeDescriptors[2] = *shader->GetDescriptorSet("u_HZB");
				writeDescriptors[2].dstSet = descriptorSet;
				writeDescriptors[2].pImageInfo = &depthDescriptorImageInfo;

				vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

				auto [width, height] = visibilityTexture->GetMipSize(i);
				glm::vec2 resFactor = 1.0f / glm::vec2{ width, height };
				preIntegrationComputePushConstants.HZBResFactor = resFactor * hzbUVFactor;
				preIntegrationComputePushConstants.ResFactor = resFactor;
				preIntegrationComputePushConstants.ProjectionParams = projectionParams;
				preIntegrationComputePushConstants.PrevLod = (int)i - 1;

				pipeline->SetPushConstants(&preIntegrationComputePushConstants, sizeof(preIntegrationComputePushConstants));
				pipeline->Dispatch(descriptorSet, workGroupsX, workGroupsY, 1);
				Renderer::RT_EndGPUPerfMarker(commandBuffer);
			}
			Renderer::RT_EndGPUPerfMarker(commandBuffer);
			pipeline->End();
		});
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.PreIntegrationQuery);
#endif
	}

	void SceneRenderer::LightCullingPass()
	{
		m_LightCullingMaterial->Set("u_DepthMap", m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>());

		m_GPUTimeQueries.LightCullingPassQuery = m_CommandBuffer->BeginTimestampQuery();
		SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "LightCulling", { 1.0f, 1.0f, 1.0f, 1.0f });
		
	//	Renderer::LightCulling(m_CommandBuffer, m_LightCullingPipeline, m_UniformBufferSet, m_StorageBufferSet, m_LightCullingMaterial, m_LightCullingWorkGroups);
		
		SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.LightCullingPassQuery);
	}

	void SceneRenderer::GeometryPass()
	{
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		m_GPUTimeQueries.GeometryPassQuery = m_CommandBuffer->BeginTimestampQuery();

		Renderer::BeginRenderPass(m_CommandBuffer, m_SelectedGeometryPipeline->GetSpecification().RenderPass);
		for (auto &[mk, dc] : m_SelectedStaticMeshDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			Renderer::RenderInstancedStaticMeshWithMaterial(
				m_CommandBuffer, 
				m_SelectedGeometryPipeline, 
				m_UniformBufferSet, nullptr, 
				dc.Model, dc.SubmeshIndex, 
				m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData), 
				dc.InstanceCount, 
				m_SelectedGeometryMaterial);
		}
		for (auto &[mk, dc] : m_SelectedMeshDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			if (dc.IsRigged)
			{
				const auto &boneTransformsData = m_MeshBoneTransformsMap.at(mk);
				Renderer::RenderInstancedDynamicMeshWithMaterial(
					m_CommandBuffer, 
					m_SelectedGeometryPipelineAnim, 
					m_UniformBufferSet, nullptr, 
					dc.Model, dc.SubmeshIndex, 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData), 
					m_BoneTransformStorageBuffers, boneTransformsData.BoneTransformsBaseIndex + dc.InstanceOffset, 
					dc.InstanceCount, 
					m_SelectedGeometryMaterial);
			}
			else
			{
				Renderer::RenderInstancedDynamicMeshWithMaterial(
					m_CommandBuffer, 
					m_SelectedGeometryPipeline, 
					m_UniformBufferSet, nullptr, 
					dc.Model, dc.SubmeshIndex, 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData), 
					{}, 0, 
					dc.InstanceCount, 
					m_SelectedGeometryMaterial);
			}
		}
		Renderer::EndRenderPass(m_CommandBuffer);

		Renderer::BeginRenderPass(m_CommandBuffer, m_GeometryPipeline->GetSpecification().RenderPass);
		
		// Skybox
		m_SkyboxMaterial->Set("u_Uniforms.TextureLod", m_SceneData.SkyboxLod);
		m_SkyboxMaterial->Set("u_Uniforms.Intensity", m_SceneData.SceneEnvironmentIntensity);

		const Ref<Texture3D> radianceMap = m_SceneData.SceneEnvironment 
			? m_SceneData.SceneEnvironment->GetRadianceMap() 
			: Renderer::GetBlackCubeTexture();

		m_SkyboxMaterial->Set("u_Texture", radianceMap);

		SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Skybox", { 0.3f, 0.0f, 1.0f, 1.0f });
		Renderer::RenderFullscreenQuad(m_CommandBuffer, m_SkyboxPipeline, m_UniformBufferSet, nullptr, m_SkyboxMaterial);
		SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

		// Render static meshes
		SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Static Meshes");
		for (auto &[mk, dc] : m_StaticMeshDrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			Renderer::RenderInstancedStaticMesh(
				m_CommandBuffer, 
				m_GeometryPipeline, 
				m_UniformBufferSet, 
				m_StorageBufferSet, 
				dc.Model, 
				dc.SubmeshIndex, 
				dc.MaterialTable ? dc.MaterialTable : dc.Model->GetMaterials(), 
				m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
				dc.InstanceCount);
		}
		SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

		// Render dynamic meshes
		SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Dynamic Meshes");
		for (auto &[mk, dc] : m_DrawList)
		{
			const auto &transformData = m_MeshTransformMap.at(mk);
			if (dc.IsRigged)
			{
				const auto &boneTransformsData = m_MeshBoneTransformsMap.at(mk);
				Renderer::RenderInstancedDynamicSubmesh(
					m_CommandBuffer, 
					m_GeometryPipelineAnim, 
					m_UniformBufferSet, m_StorageBufferSet, 
					dc.Model, dc.SubmeshIndex, 
					dc.MaterialTable ? dc.MaterialTable : dc.Model->GetMaterials(), 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
					m_BoneTransformStorageBuffers, 
					boneTransformsData.BoneTransformsBaseIndex, 
					dc.InstanceCount);
			}
			else
			{
				Renderer::RenderInstancedDynamicSubmesh(
					m_CommandBuffer, 
					m_GeometryPipeline, 
					m_UniformBufferSet, m_StorageBufferSet, 
					dc.Model, dc.SubmeshIndex, 
					dc.MaterialTable ? dc.MaterialTable : dc.Model->GetMaterials(), 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
					{}, 0, 
					dc.InstanceCount);
			}
		}
		SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

		{
			// Render static meshes
			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Static Transparent Meshes");
			for (auto &[mk, dc] : m_TransparentStaticMeshDrawList)
			{
				const auto &transformData = m_MeshTransformMap.at(mk);
				Renderer::RenderInstancedStaticMesh(
					m_CommandBuffer, 
					m_TransparentGeometryPipeline, 
					m_UniformBufferSet, 
					m_StorageBufferSet, 
					dc.Model, 
					dc.SubmeshIndex, 
					dc.MaterialTable ? dc.MaterialTable : dc.Model->GetMaterials(), 
					m_SubmeshTransformBuffers[frameIndex].Buffer, 
					transformData.TransformOffset, 
					dc.InstanceCount);
			}
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

			// Render dynamic meshes
			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Dynamic Transparent Meshes");
			for (auto &[mk, dc] : m_TransparentDrawList)
			{
				const auto &transformData = m_MeshTransformMap.at(mk);
				//Renderer::RenderSubmesh(m_CommandBuffer, m_GeometryPipeline, m_UniformBufferSet, m_StorageBufferSet, dc.Mesh, dc.SubmeshIndex, dc.MaterialTable ? dc.MaterialTable : dc.Mesh->GetMaterials(), dc.Transform);
				Renderer::RenderInstancedDynamicSubmesh(
					m_CommandBuffer, 
					m_TransparentGeometryPipeline, 
					m_UniformBufferSet, m_StorageBufferSet, 
					dc.Model, dc.SubmeshIndex, 
					dc.MaterialTable ? dc.MaterialTable : dc.Model->GetMaterials(), 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
					{}, 
					0, 
					dc.InstanceCount);
			}
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);
		}


		Renderer::EndRenderPass(m_CommandBuffer);
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.GeometryPassQuery);
	}

	void SceneRenderer::PreConvolutionCompute()
	{
		// TODO: Other techniques might need it in the future
		
		if (!m_Options.EnableSSR)
			return;
		
#if 0

		Ref<VulkanComputePipeline> pipeline = m_GaussianBlurPipeline.As<VulkanComputePipeline>();
		struct PreConvolutionComputePushConstants
		{
			int PrevLod = 0;
			int Mode = 0; // 0 = Copy, 1 = GaussianHorizontal, 2 = GaussianVertical
		} preConvolutionComputePushConstants;

		//Might change to be maximum res used by other techniques other than SSR.
		int halfRes = int(m_SSROptions.HalfRes);

		m_GPUTimeQueries.PreConvolutionQuery = m_CommandBuffer->BeginTimestampQuery();

		Renderer::Submit([preConvolutionComputePushConstants, inputColorImage = m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().TargetFramebuffer->GetImage().As<VulkanImage2D>(),
						 preConvolutedTexture = m_PreConvolutedTexture.As<VulkanTexture2D>(), commandBuffer = m_CommandBuffer.As<VulkanRenderCommandBuffer>(),
						 material = m_GaussianBlurMaterial.As<VulkanMaterial>(), pipeline, halfRes]() mutable
		{
			const VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

			Ref<VulkanImage2D> preConvolutedImage = preConvolutedTexture->GetImage().As<VulkanImage2D>();

			auto shader = material->GetShader().As<VulkanShader>();

			VkDescriptorSetLayout descriptorSetLayout = shader->GetDescriptorSetLayout(0);
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &descriptorSetLayout;

			std::array<VkWriteDescriptorSet, 2> writeDescriptors{};

			const VkDescriptorSet descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
			auto descriptorImageInfo = preConvolutedImage->GetDescriptorInfo();
			descriptorImageInfo.imageView = preConvolutedImage->RT_GetMipImageView(0);
			descriptorImageInfo.sampler = VK_NULL_HANDLE;

			// Output Pre-convoluted image
			writeDescriptors[0] = *shader->GetDescriptorSet("o_Image");
			writeDescriptors[0].dstSet = descriptorSet;
			writeDescriptors[0].pImageInfo = &descriptorImageInfo;

			// Input original colors
			writeDescriptors[1] = *shader->GetDescriptorSet("u_Input");
			writeDescriptors[1].dstSet = descriptorSet;
			writeDescriptors[1].pImageInfo = &inputColorImage->GetDescriptorInfo();

			uint32 workGroupsX = (uint32)glm::ceil((float)inputColorImage->GetWidth() / 16.0f);
			uint32 workGroupsY = (uint32)glm::ceil((float)inputColorImage->GetHeight() / 16.0f);

			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "Pre-Convolution");
			pipeline->RT_Begin(commandBuffer);
			vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "Pre-Convolution-FirstPass");
			pipeline->SetPushConstants(&preConvolutionComputePushConstants, sizeof preConvolutionComputePushConstants);
			pipeline->Dispatch(descriptorSet, workGroupsX, workGroupsY, 1);
			Renderer::RT_EndGPUPerfMarker(commandBuffer);

			const uint32 mipCount = preConvolutedTexture->GetMipLevelCount();
			for (uint32 mip = 1; mip < mipCount; mip++)
			{
				Renderer::RT_BeginGPUPerfMarker(commandBuffer, fmt::format("Pre-Convolution-Mip({})", mip));

				auto [mipWidth, mipHeight] = preConvolutedTexture->GetMipSize(mip);
				workGroupsX = (uint32)glm::ceil((float)mipWidth / 16.0f);
				workGroupsY = (uint32)glm::ceil((float)mipHeight / 16.0f);

				const VkDescriptorSet descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
				// Output image
				descriptorImageInfo.imageView = preConvolutedImage->RT_GetMipImageView(mip);

				writeDescriptors[0] = *shader->GetDescriptorSet("o_Image");
				writeDescriptors[0].dstSet = descriptorSet;
				writeDescriptors[0].pImageInfo = &descriptorImageInfo;

				writeDescriptors[1] = *shader->GetDescriptorSet("u_Input");
				writeDescriptors[1].dstSet = descriptorSet;
				writeDescriptors[1].pImageInfo = &preConvolutedImage->GetDescriptorInfo();

				vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);
				preConvolutionComputePushConstants.PrevLod = (int)mip - 1;

				auto blur = [&](const int mode)
				{
					Renderer::RT_BeginGPUPerfMarker(commandBuffer, fmt::format("Pre-Convolution-Mode({})", mode));
					preConvolutionComputePushConstants.Mode = (int)mode;
					pipeline->SetPushConstants(&preConvolutionComputePushConstants, sizeof(preConvolutionComputePushConstants));
					pipeline->Dispatch(descriptorSet, workGroupsX, workGroupsY, 1);
					Renderer::RT_EndGPUPerfMarker(commandBuffer);
				};



				blur(1); // Horizontal blur
				blur(2); // Vertical Blur
				Renderer::RT_EndGPUPerfMarker(commandBuffer);

			}
			pipeline->End();
			Renderer::RT_EndGPUPerfMarker(commandBuffer);
		});
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.PreConvolutionQuery);
#endif
	}

	void SceneRenderer::DeinterleavingPass()
	{
		m_DeinterleavingMaterial->Set("u_Depth", m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>());

		for (int32 i = 0; i < 2; i++)
		{
			Renderer::Submit([i, material = m_DeinterleavingMaterial]() mutable
			{
				material->Set("u_Info.UVOffsetIndex", i);
			});
			Renderer::BeginRenderPass(m_CommandBuffer, m_DeinterleavingPipelines[i]->GetSpecification().RenderPass);
			Renderer::RenderFullscreenQuad(m_CommandBuffer, m_DeinterleavingPipelines[i], m_UniformBufferSet, nullptr, m_DeinterleavingMaterial);
			Renderer::EndRenderPass(m_CommandBuffer);
		}
	}

	void SceneRenderer::HBAOCompute()
	{
		m_HBAOMaterial->Set("u_LinearDepthTexArray", m_DeinterleavingPipelines[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage().As<Texture2D>());
		m_HBAOMaterial->Set("u_ViewNormalsMaskTex", m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(1).As<Texture2D>());
		m_HBAOMaterial->Set("o_Color", m_HBAOOutputImage);

	//	Renderer::DispatchComputeShader(m_CommandBuffer, m_HBAOPipeline, m_UniformBufferSet, nullptr, m_HBAOMaterial, m_HBAOWorkGroups);
	}

	void SceneRenderer::GTAOCompute()
	{
#if 0
		m_GTAOMaterial->Set("u_HiZDepth", m_HierarchicalDepthTexture);
		m_GTAOMaterial->Set("u_HilbertLut", Renderer::GetHilbertLut());
		m_GTAOMaterial->Set("u_ViewNormal", m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(1).As<Texture2D>());
		m_GTAOMaterial->Set("o_AOwBentNormals", m_GTAOOutputImage);
		m_GTAOMaterial->Set("o_Edges", m_GTAOEdgesOutputImage);


		const Buffer pushConstantBuffer(&GTAODataCB, sizeof GTAODataCB);

		m_GPUTimeQueries.GTAOPassQuery = m_CommandBuffer->BeginTimestampQuery();
		Renderer::DispatchComputeShader(m_CommandBuffer, m_GTAOPipeline, m_UniformBufferSet, nullptr, m_GTAOMaterial, m_GTAOWorkGroups, pushConstantBuffer);
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.GTAOPassQuery);
#endif
	}

	void SceneRenderer::GTAODenoiseCompute()
	{
#if 0
		struct DenoisePushConstant
		{
			float DenoiseBlurBeta;
			bool HalfRes;
			char Padding1[3]{ 0, 0, 0 };
		} denoisePushConstant;

		denoisePushConstant.DenoiseBlurBeta = GTAODataCB.DenoiseBlurBeta;
		denoisePushConstant.HalfRes = GTAODataCB.HalfRes;
		const Buffer pushConstantBuffer(&denoisePushConstant, sizeof denoisePushConstant);

		m_GPUTimeQueries.GTAODenoisePassQuery = m_CommandBuffer->BeginTimestampQuery();
		for (uint32 pass = 0; pass < (uint32)m_Options.GTAODenoisePasses; ++pass)
			Renderer::DispatchComputeShader(m_CommandBuffer, m_GTAODenoisePipeline, m_UniformBufferSet, nullptr, m_GTAODenoiseMaterial[uint32(pass % 2 != 0)], m_GTAODenoiseWorkGroups, pushConstantBuffer);
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.GTAODenoisePassQuery);
#endif
	}

	void SceneRenderer::ReinterleavingPass()
	{
		if (!m_Options.EnableHBAO)
		{
			ClearPass(m_ReinterleavingPipeline->GetSpecification().RenderPass);
			return;
		}
		Renderer::BeginRenderPass(m_CommandBuffer, m_ReinterleavingPipeline->GetSpecification().RenderPass);
		m_ReinterleavingMaterial->Set("u_TexResultsArray", m_HBAOOutputImage);
		Renderer::RenderFullscreenQuad(m_CommandBuffer, m_ReinterleavingPipeline, nullptr, nullptr, m_ReinterleavingMaterial);
		Renderer::EndRenderPass(m_CommandBuffer);
	}

	void SceneRenderer::HBAOBlurPass()
	{
		{
			Renderer::BeginRenderPass(m_CommandBuffer, m_HBAOBlurPipelines[0]->GetSpecification().RenderPass);
			m_HBAOBlurMaterials[0]->Set("u_Info.InvResDirection", glm::vec2{ m_InvViewportWidth, 0.0f });
			m_HBAOBlurMaterials[0]->Set("u_Info.Sharpness", m_Options.HBAOBlurSharpness);
			Renderer::RenderFullscreenQuad(m_CommandBuffer, m_HBAOBlurPipelines[0], nullptr, nullptr, m_HBAOBlurMaterials[0]);
			Renderer::EndRenderPass(m_CommandBuffer);
		}

		{
			Renderer::BeginRenderPass(m_CommandBuffer, m_HBAOBlurPipelines[1]->GetSpecification().RenderPass);
			m_HBAOBlurMaterials[1]->Set("u_Info.InvResDirection", glm::vec2{ 0.0f, m_InvViewportHeight });
			m_HBAOBlurMaterials[1]->Set("u_Info.Sharpness", m_Options.HBAOBlurSharpness);
			Renderer::RenderFullscreenQuad(m_CommandBuffer, m_HBAOBlurPipelines[1], nullptr, nullptr, m_HBAOBlurMaterials[1]);
			Renderer::EndRenderPass(m_CommandBuffer);
		}
	}

	void SceneRenderer::AOComposite()
	{
		if (m_Options.EnableGTAO)
			m_AOCompositeMaterial->Set("u_GTAOTex", m_GTAOFinalImage);
		if (m_Options.EnableHBAO)
			m_AOCompositeMaterial->Set("u_HBAOTex", m_HBAOBlurPipelines[1]->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage().As<Texture2D>());
		m_GPUTimeQueries.AOCompositePassQuery = m_CommandBuffer->BeginTimestampQuery();
		Renderer::BeginRenderPass(m_CommandBuffer, m_AOCompositeRenderPass);
		Renderer::RenderFullscreenQuad(m_CommandBuffer, m_AOCompositePipeline, nullptr, nullptr, m_AOCompositeMaterial);
		Renderer::EndRenderPass(m_CommandBuffer);
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.AOCompositePassQuery);
	}

	void SceneRenderer::JumpFloodPass()
	{
		m_GPUTimeQueries.JumpFloodPassQuery = m_CommandBuffer->BeginTimestampQuery();
		Renderer::BeginRenderPass(m_CommandBuffer, m_JumpFloodInitPipeline->GetSpecification().RenderPass);

		auto framebuffer = m_SelectedGeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer;
		m_JumpFloodInitMaterial->Set("u_Texture", framebuffer->GetImage().As<Texture2D>());

		Renderer::RenderFullscreenQuad(m_CommandBuffer, m_JumpFloodInitPipeline, nullptr, nullptr, m_JumpFloodInitMaterial);
		Renderer::EndRenderPass(m_CommandBuffer);

		m_JumpFloodPassMaterial[0]->Set("u_Texture", m_TempFramebuffers[0]->GetImage().As<Texture2D>());
		m_JumpFloodPassMaterial[1]->Set("u_Texture", m_TempFramebuffers[1]->GetImage().As<Texture2D>());

		int32 steps = 2;
		int32 step = (int)glm::round(glm::pow<int>(steps - 1, 2));
		int32 index = 0;
		
		Allocator vertexOverrides;
		Ref<Framebuffer> passFB = m_JumpFloodPassPipeline[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer;
		glm::vec2 texelSize = { 1.0f / (float)passFB->GetWidth(), 1.0f / (float)passFB->GetHeight() };
		vertexOverrides.Allocate(sizeof(glm::vec2) + sizeof(int));
		vertexOverrides.Write(glm::value_ptr(texelSize), sizeof(glm::vec2));
		while (step != 0)
		{
			vertexOverrides.Write(&step, sizeof(int), sizeof(glm::vec2));

			Renderer::BeginRenderPass(m_CommandBuffer, m_JumpFloodPassPipeline[index]->GetSpecification().RenderPass);
		//	Renderer::SubmitFullscreenQuadWithOverrides(m_CommandBuffer, m_JumpFloodPassPipeline[index], nullptr, m_JumpFloodPassMaterial[index], vertexOverrides, Buffer());
			Renderer::EndRenderPass(m_CommandBuffer);

			index = (index + 1) % 2;
			step /= 2;
		}

		vertexOverrides.Release();

		m_JumpFloodCompositeMaterial->Set("u_Texture", m_TempFramebuffers[1]->GetImage().As<Texture2D>());
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.JumpFloodPassQuery);
	}

	void SceneRenderer::SSRCompute()
	{
		m_SSRMaterial->Set("outColor", m_SSRImage);
		m_SSRMaterial->Set("u_InputColor", m_PreConvolutedTexture);
		m_SSRMaterial->Set("u_VisibilityBuffer", m_VisibilityTexture);
		m_SSRMaterial->Set("u_HiZBuffer", m_HierarchicalDepthTexture);
		m_SSRMaterial->Set("u_Normal", m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(1).As<Texture2D>());
		m_SSRMaterial->Set("u_MetalnessRoughness", m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(2).As<Texture2D>());

		/*
		if ((int32)m_Options.ReflectionOcclusionMethod & (int)ShaderDef::AOMethod::GTAO)
			m_SSRMaterial->Set("u_GTAOTex", m_GTAOFinalImage);

		if ((int32)m_Options.ReflectionOcclusionMethod & (int)ShaderDef::AOMethod::HBAO)
			m_SSRMaterial->Set("u_HBAOTex", (m_HBAOBlurPipelines[1]->GetSpecification().RenderPass->GetSpecification().TargetFramebuffer->GetImage()));
		*/

	//	const Buffer pushConstantsBuffer(&m_SSROptions, sizeof m_SSROptions);

		m_GPUTimeQueries.SSRQuery = m_CommandBuffer->BeginTimestampQuery();
	//	Renderer::DispatchComputeShader(m_CommandBuffer, m_SSRPipeline, m_UniformBufferSet, nullptr, m_SSRMaterial, m_SSRWorkGroups, pushConstantsBuffer);
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.SSRQuery);
	}

	void SceneRenderer::SSRCompositePass()
	{
		// Currently scales the SSR, renders with transparency.
		// The alpha channel is the confidence.
		m_SSRCompositeMaterial->Set("u_SSR", m_SSRImage);

		m_GPUTimeQueries.SSRCompositeQuery = m_CommandBuffer->BeginTimestampQuery();
		Renderer::BeginRenderPass(m_CommandBuffer, m_SSRCompositePipeline->GetSpecification().RenderPass);
		Renderer::RenderFullscreenQuad(m_CommandBuffer, m_SSRCompositePipeline, m_UniformBufferSet, nullptr, m_SSRCompositeMaterial);
		Renderer::EndRenderPass(m_CommandBuffer);
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.SSRCompositeQuery);
	}

	void SceneRenderer::BloomCompute()
	{
#if 0
		Ref<VulkanComputePipeline> pipeline = m_BloomComputePipeline.As<VulkanComputePipeline>();

		//m_BloomComputeMaterial->Set("o_Image", m_BloomComputeTexture);

		struct BloomComputePushConstants
		{
			glm::vec4 Params;
			float LOD = 0.0f;
			int Mode = 0; // 0 = prefilter, 1 = downsample, 2 = firstUpsample, 3 = upsample
		} bloomComputePushConstants;
		bloomComputePushConstants.Params = { m_BloomSettings.Threshold, m_BloomSettings.Threshold - m_BloomSettings.Knee, m_BloomSettings.Knee * 2.0f, 0.25f / m_BloomSettings.Knee };
		bloomComputePushConstants.Mode = 0;

		auto inputImage = m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().TargetFramebuffer->GetImage().As<VulkanImage2D>();

		m_GPUTimeQueries.BloomComputePassQuery = m_CommandBuffer->BeginTimestampQuery();

		Renderer::Submit([bloomComputePushConstants, inputImage, workGroupSize = m_BloomComputeWorkgroupSize, commandBuffer = m_CommandBuffer, bloomTextures = m_BloomComputeTextures, ubs = m_UniformBufferSet, material = m_BloomComputeMaterial.As<VulkanMaterial>(), pipeline]() mutable
		{
			constexpr bool useComputeQueue = false;

			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();

			Ref<VulkanImage2D> images[3] =
			{
				bloomTextures[0]->GetImage().As<VulkanImage2D>(),
				bloomTextures[1]->GetImage().As<VulkanImage2D>(),
				bloomTextures[2]->GetImage().As<VulkanImage2D>()
			};

			auto shader = pipeline->GetShader().As<VulkanShader>();

			auto descriptorImageInfo = images[0]->GetDescriptorInfo();
			descriptorImageInfo.imageView = images[0]->RT_GetMipImageView(0);

			std::array<VkWriteDescriptorSet, 3> writeDescriptors;

			VkDescriptorSetLayout descriptorSetLayout = shader->GetDescriptorSetLayout(0);

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &descriptorSetLayout;

			pipeline->RT_Begin(useComputeQueue ? nullptr : commandBuffer);

			if (false)
			{
				VkImageMemoryBarrier imageMemoryBarrier = {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageMemoryBarrier.image = inputImage->GetImageInfo().Image;
				imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, inputImage->GetSpecification().Mips, 0, 1 };
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				vkCmdPipelineBarrier(
					pipeline->GetActiveCommandBuffer(),
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}

			// Output image
			VkDescriptorSet descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
			writeDescriptors[0] = *shader->GetDescriptorSet("o_Image");
			writeDescriptors[0].dstSet = descriptorSet; // Should this be set inside the shader?
			writeDescriptors[0].pImageInfo = &descriptorImageInfo;

			// Input image
			writeDescriptors[1] = *shader->GetDescriptorSet("u_Texture");
			writeDescriptors[1].dstSet = descriptorSet; // Should this be set inside the shader?
			writeDescriptors[1].pImageInfo = &inputImage->GetDescriptorInfo();

			writeDescriptors[2] = *shader->GetDescriptorSet("u_BloomTexture");
			writeDescriptors[2].dstSet = descriptorSet; // Should this be set inside the shader?
			writeDescriptors[2].pImageInfo = &inputImage->GetDescriptorInfo();

			vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

			uint32 workGroupsX = bloomTextures[0]->GetWidth() / workGroupSize;
			uint32 workGroupsY = bloomTextures[0]->GetHeight() / workGroupSize;

			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "Bloom-Prefilter");
			pipeline->SetPushConstants(&bloomComputePushConstants, sizeof(bloomComputePushConstants));
			pipeline->Dispatch(descriptorSet, workGroupsX, workGroupsY, 1);
			Renderer::RT_EndGPUPerfMarker(commandBuffer);

			{
				VkImageMemoryBarrier imageMemoryBarrier = {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageMemoryBarrier.image = images[0]->GetImageInfo().Image;
				imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, images[0]->GetSpecification().Mips, 0, 1 };
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				vkCmdPipelineBarrier(
					pipeline->GetActiveCommandBuffer(),
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}

			bloomComputePushConstants.Mode = 1;


			uint32 mips = bloomTextures[0]->GetMipLevelCount() - 2;
			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "Bloom-DownSample");
			for (uint32 i = 1; i < mips; i++)
			{
				auto [mipWidth, mipHeight] = bloomTextures[0]->GetMipSize(i);
				workGroupsX = (uint32)glm::ceil((float)mipWidth / (float)workGroupSize);
				workGroupsY = (uint32)glm::ceil((float)mipHeight / (float)workGroupSize);

				{
					// Output image
					descriptorImageInfo.imageView = images[1]->RT_GetMipImageView(i);

					descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
					writeDescriptors[0] = *shader->GetDescriptorSet("o_Image");
					writeDescriptors[0].dstSet = descriptorSet; // Should this be set inside the shader?
					writeDescriptors[0].pImageInfo = &descriptorImageInfo;

					// Input image
					writeDescriptors[1] = *shader->GetDescriptorSet("u_Texture");
					writeDescriptors[1].dstSet = descriptorSet; // Should this be set inside the shader?
					auto descriptor = bloomTextures[0]->GetImage().As<VulkanImage2D>()->GetDescriptorInfo();
					//descriptor.sampler = samplerClamp;
					writeDescriptors[1].pImageInfo = &descriptor;

					writeDescriptors[2] = *shader->GetDescriptorSet("u_BloomTexture");
					writeDescriptors[2].dstSet = descriptorSet; // Should this be set inside the shader?
					writeDescriptors[2].pImageInfo = &inputImage->GetDescriptorInfo();

					vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

					bloomComputePushConstants.LOD = i - 1.0f;
					pipeline->SetPushConstants(&bloomComputePushConstants, sizeof(bloomComputePushConstants));
					pipeline->Dispatch(descriptorSet, workGroupsX, workGroupsY, 1);
				}

				{
					VkImageMemoryBarrier imageMemoryBarrier = {};
					imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
					imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
					imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
					imageMemoryBarrier.image = images[1]->GetImageInfo().Image;
					imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, images[1]->GetSpecification().Mips, 0, 1 };
					imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
					imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					vkCmdPipelineBarrier(
						pipeline->GetActiveCommandBuffer(),
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						0,
						0, nullptr,
						0, nullptr,
						1, &imageMemoryBarrier);
				}

				{
					descriptorImageInfo.imageView = images[0]->RT_GetMipImageView(i);

					// Output image
					descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
					writeDescriptors[0] = *shader->GetDescriptorSet("o_Image");
					writeDescriptors[0].dstSet = descriptorSet; // Should this be set inside the shader?
					writeDescriptors[0].pImageInfo = &descriptorImageInfo;

					// Input image
					writeDescriptors[1] = *shader->GetDescriptorSet("u_Texture");
					writeDescriptors[1].dstSet = descriptorSet; // Should this be set inside the shader?
					auto descriptor = bloomTextures[1]->GetImage().As<VulkanImage2D>()->GetDescriptorInfo();
					//descriptor.sampler = samplerClamp;
					writeDescriptors[1].pImageInfo = &descriptor;

					writeDescriptors[2] = *shader->GetDescriptorSet("u_BloomTexture");
					writeDescriptors[2].dstSet = descriptorSet; // Should this be set inside the shader?
					writeDescriptors[2].pImageInfo = &inputImage->GetDescriptorInfo();

					vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

					bloomComputePushConstants.LOD = (float)i;
					pipeline->SetPushConstants(&bloomComputePushConstants, sizeof(bloomComputePushConstants));
					pipeline->Dispatch(descriptorSet, workGroupsX, workGroupsY, 1);
				}

				{
					VkImageMemoryBarrier imageMemoryBarrier = {};
					imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
					imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
					imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
					imageMemoryBarrier.image = images[0]->GetImageInfo().Image;
					imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, images[0]->GetSpecification().Mips, 0, 1 };
					imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
					imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					vkCmdPipelineBarrier(
						pipeline->GetActiveCommandBuffer(),
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						0,
						0, nullptr,
						0, nullptr,
						1, &imageMemoryBarrier);
				}
			}
			Renderer::RT_EndGPUPerfMarker(commandBuffer);

			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "Bloom-FirstUpsamle");
			bloomComputePushConstants.Mode = 2;
			workGroupsX *= 2;
			workGroupsY *= 2;

			// Output image
			descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
			descriptorImageInfo.imageView = images[2]->RT_GetMipImageView(mips - 2);

			writeDescriptors[0] = *shader->GetDescriptorSet("o_Image");
			writeDescriptors[0].dstSet = descriptorSet; // Should this be set inside the shader?
			writeDescriptors[0].pImageInfo = &descriptorImageInfo;

			// Input image
			writeDescriptors[1] = *shader->GetDescriptorSet("u_Texture");
			writeDescriptors[1].dstSet = descriptorSet; // Should this be set inside the shader?
			writeDescriptors[1].pImageInfo = &bloomTextures[0]->GetImage().As<VulkanImage2D>()->GetDescriptorInfo();

			writeDescriptors[2] = *shader->GetDescriptorSet("u_BloomTexture");
			writeDescriptors[2].dstSet = descriptorSet; // Should this be set inside the shader?
			writeDescriptors[2].pImageInfo = &inputImage->GetDescriptorInfo();

			vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

			bloomComputePushConstants.LOD--;
			pipeline->SetPushConstants(&bloomComputePushConstants, sizeof(bloomComputePushConstants));

			auto [mipWidth, mipHeight] = bloomTextures[2]->GetMipSize(mips - 2);
			workGroupsX = (uint32)glm::ceil((float)mipWidth / (float)workGroupSize);
			workGroupsY = (uint32)glm::ceil((float)mipHeight / (float)workGroupSize);
			pipeline->Dispatch(descriptorSet, workGroupsX, workGroupsY, 1);

			{
				VkImageMemoryBarrier imageMemoryBarrier = {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
				imageMemoryBarrier.image = images[2]->GetImageInfo().Image;
				imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, images[2]->GetSpecification().Mips, 0, 1 };
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				vkCmdPipelineBarrier(
					pipeline->GetActiveCommandBuffer(),
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}

			Renderer::RT_EndGPUPerfMarker(commandBuffer);

			Renderer::RT_BeginGPUPerfMarker(commandBuffer, "Bloom-Upsample");
			bloomComputePushConstants.Mode = 3;

			// Upsample
			for (int32_t mip = mips - 3; mip >= 0; mip--)
			{
				auto [mipWidth, mipHeight] = bloomTextures[2]->GetMipSize(mip);
				workGroupsX = (uint32)glm::ceil((float)mipWidth / (float)workGroupSize);
				workGroupsY = (uint32)glm::ceil((float)mipHeight / (float)workGroupSize);

				// Output image
				descriptorImageInfo.imageView = images[2]->RT_GetMipImageView(mip);
				auto descriptorSet = VulkanRenderer::RT_AllocateDescriptorSet(allocInfo);
				writeDescriptors[0] = *shader->GetDescriptorSet("o_Image");
				writeDescriptors[0].dstSet = descriptorSet; // Should this be set inside the shader?
				writeDescriptors[0].pImageInfo = &descriptorImageInfo;

				// Input image
				writeDescriptors[1] = *shader->GetDescriptorSet("u_Texture");
				writeDescriptors[1].dstSet = descriptorSet; // Should this be set inside the shader?
				writeDescriptors[1].pImageInfo = &bloomTextures[0]->GetImage().As<VulkanImage2D>()->GetDescriptorInfo();

				writeDescriptors[2] = *shader->GetDescriptorSet("u_BloomTexture");
				writeDescriptors[2].dstSet = descriptorSet; // Should this be set inside the shader?
				writeDescriptors[2].pImageInfo = &images[2]->GetDescriptorInfo();

				vkUpdateDescriptorSets(device, (uint32)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

				bloomComputePushConstants.LOD = (float)mip;
				pipeline->SetPushConstants(&bloomComputePushConstants, sizeof(bloomComputePushConstants));
				pipeline->Dispatch(descriptorSet, workGroupsX, workGroupsY, 1);

				{
					VkImageMemoryBarrier imageMemoryBarrier = {};
					imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
					imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
					imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
					imageMemoryBarrier.image = images[2]->GetImageInfo().Image;
					imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, images[2]->GetSpecification().Mips, 0, 1 };
					imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
					imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					vkCmdPipelineBarrier(
						pipeline->GetActiveCommandBuffer(),
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
						0,
						0, nullptr,
						0, nullptr,
						1, &imageMemoryBarrier);
				}
			}
			Renderer::RT_EndGPUPerfMarker(commandBuffer);

			pipeline->End();
		});
		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.BloomComputePassQuery);
#endif
	}

	void SceneRenderer::EdgeDetectionPass()
	{
		Renderer::BeginRenderPass(m_CommandBuffer, m_EdgeDetectionPipeline->GetSpecification().RenderPass);
		m_EdgeDetectionMaterial->Set("u_ViewNormalsTexture", m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(1).As<Texture2D>());
		m_EdgeDetectionMaterial->Set("u_DepthTexture", m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>());
		Renderer::RenderFullscreenQuad(m_CommandBuffer, m_EdgeDetectionPipeline, m_UniformBufferSet, nullptr, m_EdgeDetectionMaterial);
		Renderer::EndRenderPass(m_CommandBuffer);
	}

	void SceneRenderer::CompositePass()
	{
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		m_GPUTimeQueries.CompositePassQuery = m_CommandBuffer->BeginTimestampQuery();

		Renderer::BeginRenderPass(m_CommandBuffer, m_CompositePipeline->GetSpecification().RenderPass, true);

		auto framebuffer = m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer;
		float exposure = m_SceneData.SceneCamera.Camera.GetExposure();
		int textureSamples = framebuffer->GetSpecification().Samples;

		m_CompositeMaterial->Set("u_Uniforms.Exposure", exposure);
		if (m_BloomSettings.Enabled)
		{
			m_CompositeMaterial->Set("u_Uniforms.BloomIntensity", m_BloomSettings.Intensity);
			m_CompositeMaterial->Set("u_Uniforms.BloomDirtIntensity", m_BloomSettings.DirtIntensity);
		}
		else
		{
			m_CompositeMaterial->Set("u_Uniforms.BloomIntensity", 0.0f);
			m_CompositeMaterial->Set("u_Uniforms.BloomDirtIntensity", 0.0f);
		}

		m_CompositeMaterial->Set("u_Uniforms.Opacity", m_Opacity);
	//	m_CompositeMaterial->Set("u_Uniforms.Time", HLApplication::Get().GetTime()); // TODO
		m_CompositeMaterial->Set("u_Uniforms.Time", 1.0f);

		// CompositeMaterial->Set("u_Uniforms.TextureSamples", textureSamples);

		auto inputImage = m_GeometryPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage();
		m_CompositeMaterial->Set("u_Texture", inputImage.As<Texture2D>());
		m_CompositeMaterial->Set("u_BloomTexture", m_BloomComputeTextures[2]);
		m_CompositeMaterial->Set("u_BloomDirtTexture", m_BloomDirtTexture);
		m_CompositeMaterial->Set("u_DepthTexture", m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>());
		m_CompositeMaterial->Set("u_TransparentDepthTexture", m_PreDepthTransparentPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>());

		SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Composite");
		Renderer::RenderFullscreenQuad(m_CommandBuffer, m_CompositePipeline, m_UniformBufferSet, nullptr, m_CompositeMaterial);
		SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

		if (m_DOFSettings.Enabled)
		{
			Renderer::EndRenderPass(m_CommandBuffer);

			Renderer::BeginRenderPass(m_CommandBuffer, m_DOFPipeline->GetSpecification().RenderPass);
			m_DOFMaterial->Set("u_Texture", m_CompositePipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage().As<Texture2D>());
			m_DOFMaterial->Set("u_DepthTexture", m_PreDepthPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetDepthImage().As<Texture2D>());
			m_DOFMaterial->Set("u_Uniforms.DOFParams", glm::vec2(m_DOFSettings.FocusDistance, m_DOFSettings.BlurSize));
			Renderer::RenderFullscreenQuad(m_CommandBuffer, m_DOFPipeline, m_UniformBufferSet, nullptr, m_DOFMaterial);

			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "JumpFlood-Composite");
			Renderer::RenderFullscreenQuad(m_CommandBuffer, m_JumpFloodCompositePipeline, nullptr, nullptr, m_JumpFloodCompositeMaterial);
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

			Renderer::EndRenderPass(m_CommandBuffer);

			// Copy DOF image to composite pipeline
		//	Renderer::CopyImage(m_CommandBuffer,
		//						m_DOFPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(),
		//						m_CompositePipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage());

			// WIP - will later be used for debugging/editor mouse picking
			if (m_ReadBackImage)
			{
			//	Renderer::CopyImage(m_CommandBuffer,
			//						m_DOFPipeline->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetImage(),
			//						m_ReadBackImage);

				/*
				{
					auto alloc = m_ReadBackImage.As<VulkanImage2D>()->GetImageInfo().MemoryAlloc;
					VulkanAllocator allocator("SceneRenderer");
					glm::vec4 *mappedMem = allocator.MapMemory<glm::vec4>(alloc);
					delete[] m_ReadBackBuffer;
					m_ReadBackBuffer = new glm::vec4[m_ReadBackImage->GetWidth() * m_ReadBackImage->GetHeight()];
					memcpy(m_ReadBackBuffer, mappedMem, sizeof(glm::vec4) * m_ReadBackImage->GetWidth() * m_ReadBackImage->GetHeight());
					allocator.UnmapMemory(alloc);
				}
				*/
			}
		}
		else
		{
			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "JumpFlood-Composite");
			Renderer::RenderFullscreenQuad(m_CommandBuffer, m_JumpFloodCompositePipeline, nullptr, nullptr, m_JumpFloodCompositeMaterial);
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

			Renderer::EndRenderPass(m_CommandBuffer);
		}

		m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.CompositePassQuery);

		// Grid
		if (GetOptions().ShowGrid)
		{
			Renderer::BeginRenderPass(m_CommandBuffer, m_ExternalCompositeRenderPass);
			const static glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(8.0f));
			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Grid");
		//	Renderer::RenderQuad(m_CommandBuffer, m_GridPipeline, m_UniformBufferSet, nullptr, m_GridMaterial, transform);
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);
			Renderer::EndRenderPass(m_CommandBuffer);
		}

		if (m_Options.ShowSelectedInWireframe)
		{
			Renderer::BeginRenderPass(m_CommandBuffer, m_ExternalCompositeRenderPass);

			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Static Meshes Wireframe");
			for (auto &[mk, dc] : m_SelectedStaticMeshDrawList)
			{
				const auto &transformData = m_MeshTransformMap.at(mk);
				Renderer::RenderInstancedStaticMeshWithMaterial(
					m_CommandBuffer, 
					m_GeometryWireframePipeline, 
					m_UniformBufferSet, nullptr, 
					dc.Model, dc.SubmeshIndex, 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData), 
					dc.InstanceCount, 
					m_WireframeMaterial);
			}
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Dynamic Meshes Wireframe");
			for (auto &[mk, dc] : m_SelectedMeshDrawList)
			{
				const auto &transformData = m_MeshTransformMap.at(mk);
				if (dc.IsRigged)
				{
					const auto &boneTransformsData = m_MeshBoneTransformsMap.at(mk);
					Renderer::RenderInstancedDynamicMeshWithMaterial(
						m_CommandBuffer, 
						m_GeometryWireframePipelineAnim, 
						m_UniformBufferSet, nullptr, 
						dc.Model, dc.SubmeshIndex, 
						m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData), 
						m_BoneTransformStorageBuffers, boneTransformsData.BoneTransformsBaseIndex + dc.InstanceOffset, 
						dc.InstanceCount, 
						m_WireframeMaterial);
				}
				else
				{
					Renderer::RenderInstancedDynamicMeshWithMaterial(
						m_CommandBuffer, 
						m_GeometryWireframePipeline, 
						m_UniformBufferSet, nullptr, 
						dc.Model, dc.SubmeshIndex, 
						m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset + dc.InstanceOffset * sizeof(TransformVertexData), 
						{}, 
						0, 
						dc.InstanceCount, 
						m_WireframeMaterial);
				}
			}
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

			Renderer::EndRenderPass(m_CommandBuffer);
		}

		if (m_Options.ShowPhysicsColliders)
		{
			Renderer::BeginRenderPass(m_CommandBuffer, m_ExternalCompositeRenderPass);
			auto pipeline = m_Options.ShowPhysicsCollidersOnTop ? m_GeometryWireframeOnTopPipeline : m_GeometryWireframePipeline;
			auto pipelineAnim = m_Options.ShowPhysicsCollidersOnTop ? m_GeometryWireframeOnTopPipelineAnim : m_GeometryWireframePipelineAnim;

			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Static Meshes Collider");
			for (auto &[mk, dc] : m_StaticColliderDrawList)
			{
				HL_ASSERT(m_MeshTransformMap.find(mk) != m_MeshTransformMap.end());
				const auto &transformData = m_MeshTransformMap.at(mk);
				Renderer::RenderInstancedStaticMeshWithMaterial(
					m_CommandBuffer, 
					pipeline, 
					m_UniformBufferSet, nullptr, 
					dc.Model, dc.SubmeshIndex, 
					m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
					dc.InstanceCount, 
					dc.OverrideMaterial);
			}
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);


			SceneRenderer::BeginGPUPerfMarker(m_CommandBuffer, "Dynamic Meshes Collider");
			for (auto &[mk, dc] : m_ColliderDrawList)
			{
				HL_ASSERT(m_MeshTransformMap.find(mk) != m_MeshTransformMap.end());
				const auto &transformData = m_MeshTransformMap.at(mk);
				if (dc.IsRigged)
				{
					const auto &boneTransformsData = m_MeshBoneTransformsMap.at(mk);
					Renderer::RenderInstancedDynamicMeshWithMaterial(
						m_CommandBuffer, 
						pipelineAnim, 
						m_UniformBufferSet, nullptr, 
						dc.Model, dc.SubmeshIndex, 
						m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset,
						m_BoneTransformStorageBuffers, boneTransformsData.BoneTransformsBaseIndex, 
						dc.InstanceCount, 
						m_SimpleColliderMaterial);
				}
				else
				{
					Renderer::RenderInstancedDynamicMeshWithMaterial(
						m_CommandBuffer, 
						pipeline, 
						m_UniformBufferSet, nullptr, 
						dc.Model, dc.SubmeshIndex, 
						m_SubmeshTransformBuffers[frameIndex].Buffer, transformData.TransformOffset, 
						{}, 
						0, 
						dc.InstanceCount, 
						m_SimpleColliderMaterial);
				}
			}
			SceneRenderer::EndGPUPerfMarker(m_CommandBuffer);

			Renderer::EndRenderPass(m_CommandBuffer);
		}
	}

	Ref<SceneRenderer> SceneRenderer::Create(const Ref<Scene> &scene, const SceneRendererSpecification &specification)
	{
		return Ref<SceneRenderer>::Create(scene, specification);
	}

	void SceneRenderer::FlushDrawList()
	{
		if (m_ResourcesCreated && m_ViewportWidth > 0 && m_ViewportHeight > 0)
		{
			// Reset GPU time queries
			m_GPUTimeQueries = SceneRenderer::GPUTimeQueries();

			PreRender();

			m_CommandBuffer->Begin();

			// Main render passes
			ShadowMapPass();
			SpotShadowMapPass();
			PreDepthPass();
			HZBCompute();
			PreIntegration();
			LightCullingPass();
			GeometryPass();

			// HBAO
			if (m_Options.EnableHBAO)
			{
				m_GPUTimeQueries.HBAOPassQuery = m_CommandBuffer->BeginTimestampQuery();
				DeinterleavingPass();
				HBAOCompute();
				ReinterleavingPass();
				HBAOBlurPass();
				m_CommandBuffer->EndTimestampQuery(m_GPUTimeQueries.HBAOPassQuery);
			}
			// GTAO
			if (m_Options.EnableGTAO)
			{
				GTAOCompute();
				GTAODenoiseCompute();
			}

			if (m_Options.EnableGTAO || m_Options.EnableHBAO)
				AOComposite();

			PreConvolutionCompute();

			// Post-processing
			JumpFloodPass();

			//SSR
			if (m_Options.EnableSSR)
			{
				SSRCompute();
				SSRCompositePass();
			}

			BloomCompute();
			CompositePass();

			m_CommandBuffer->End();
			m_CommandBuffer->Submit();
		}
		else
		{
			// Empty pass
			m_CommandBuffer->Begin();

			ClearPass();

			m_CommandBuffer->End();
			m_CommandBuffer->Submit();
		}

		UpdateStatistics();

		m_DrawList.clear();
		m_TransparentDrawList.clear();
		m_SelectedMeshDrawList.clear();
		m_ShadowPassDrawList.clear();

		m_StaticMeshDrawList.clear();
		m_TransparentStaticMeshDrawList.clear();
		m_SelectedStaticMeshDrawList.clear();
		m_StaticMeshShadowPassDrawList.clear();

		m_ColliderDrawList.clear();
		m_StaticColliderDrawList.clear();
		m_SceneData = {};

		m_MeshTransformMap.clear();
		m_MeshBoneTransformsMap.clear();
	}

	void SceneRenderer::PreRender()
	{
		uint32 frameIndex = Renderer::GetCurrentFrameIndex();

		uint32 offset = 0;
		for (auto &[key, transformData] : m_MeshTransformMap)
		{
			transformData.TransformOffset = offset * sizeof(TransformVertexData);
			for (const auto &transform : transformData.Transforms)
			{
				m_SubmeshTransformBuffers[frameIndex].Data[offset] = transform;
				offset++;
			}

		}

		m_SubmeshTransformBuffers[frameIndex].Buffer->SetData(m_SubmeshTransformBuffers[frameIndex].Data, offset * sizeof(TransformVertexData));

		uint32 index = 0;
		for (auto &[key, boneTransformsData] : m_MeshBoneTransformsMap)
		{
			boneTransformsData.BoneTransformsBaseIndex = index;
			for (const auto &boneTransforms : boneTransformsData.BoneTransformsData)
			{
				m_BoneTransformsData[index++] = boneTransforms;
			}
		}

		if (index > 0)
		{
			Ref<SceneRenderer> instance = this;
			Renderer::Submit([instance, index]() mutable
			{
				instance->m_BoneTransformStorageBuffers[Renderer::GetCurrentFrameIndex()]->SetData(instance->m_BoneTransformsData, static_cast<uint32>(index * sizeof(BoneTransforms)));
			});
		}
	}

	void SceneRenderer::CopyToBoneTransformStorage(const MeshKey &meshKey, const Ref<MeshFile> &meshSource, const std::vector<glm::mat4> &boneTransforms)
	{
		auto &boneTransformStorage = m_MeshBoneTransformsMap[meshKey].BoneTransformsData.emplace_back();
		if (boneTransforms.empty())
		{
			boneTransformStorage.fill(glm::identity<glm::mat4>());
		}
		else
		{
			for (size_t i = 0; i < meshSource->m_BoneInfo.size(); ++i)
			{
				const auto submeshInvTransform = meshSource->m_BoneInfo[i].SubMeshInverseTransform;
				const auto boneTransform = boneTransforms[meshSource->m_BoneInfo[i].BoneIndex];
				const auto invBindPose = meshSource->m_BoneInfo[i].InverseBindPose;
				boneTransformStorage[i] = submeshInvTransform * boneTransform * invBindPose;
			}
		}
	}

	void SceneRenderer::ClearPass()
	{
		Renderer::BeginRenderPass(m_CommandBuffer, m_PreDepthPipeline->GetSpecification().RenderPass, true);
		Renderer::EndRenderPass(m_CommandBuffer);

		Renderer::BeginRenderPass(m_CommandBuffer, m_CompositePipeline->GetSpecification().RenderPass, true);
		Renderer::EndRenderPass(m_CommandBuffer);

		Renderer::BeginRenderPass(m_CommandBuffer, m_DOFPipeline->GetSpecification().RenderPass, true);
		Renderer::EndRenderPass(m_CommandBuffer);
	}

	const Ref<VertexArray> &SceneRenderer::GetFinalPipeline()
	{
		return m_CompositePipeline;
	}

	const Ref<RenderPass> &SceneRenderer::GetFinalRenderPass()
	{
		return GetFinalPipeline()->GetSpecification().RenderPass;
	}

	Ref<Texture2D> SceneRenderer::GetFinalPassImage()
	{
		if (!m_ResourcesCreated)
			return nullptr;

		return GetFinalPipeline()
			->GetSpecification().RenderPass
			->GetSpecification().Framebuffer
			->GetImage()
			.As<Texture2D>();
	}

	const SceneRendererOptions &SceneRenderer::GetOptions()
	{
		return m_Options;
	}

	void SceneRenderer::CalculateCascades(CascadeData *cascades, const SceneRendererCamera &sceneCamera, const glm::vec3 &lightDirection) const
	{
		//TODO: Reversed Z projection?

		float scaleToOrigin = m_ScaleShadowCascadesToOrigin;

		glm::mat4 viewMatrix = sceneCamera.ViewMatrix;
		constexpr glm::vec4 origin = glm::vec4(glm::vec3(0.0f), 1.0f);
		viewMatrix[3] = glm::lerp(viewMatrix[3], origin, scaleToOrigin);

		auto viewProjection = sceneCamera.Camera.GetProjection() * viewMatrix;

		const int SHADOW_MAP_CASCADE_COUNT = 4;
		float cascadeSplits[SHADOW_MAP_CASCADE_COUNT];

		float nearClip = sceneCamera.Near;
		float farClip = sceneCamera.Far;
		float clipRange = farClip - nearClip;

		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

		// Calculate split depths based on view camera frustum
		// Based on method presented in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
		for (uint32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float p = (i + 1) / static_cast<float>(SHADOW_MAP_CASCADE_COUNT);
			float log = minZ * std::pow(ratio, p);
			float uniform = minZ + range * p;
			float d = CascadeSplitLambda * (log - uniform) + uniform;
			cascadeSplits[i] = (d - nearClip) / clipRange;
		}

		cascadeSplits[3] = 0.3f;

		// Manually set cascades here
		// cascadeSplits[0] = 0.05f;
		// cascadeSplits[1] = 0.15f;
		// cascadeSplits[2] = 0.3f;
		// cascadeSplits[3] = 1.0f;

		// Calculate orthographic projection matrix for each cascade
		float lastSplitDist = 0.0;
		for (uint32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float splitDist = cascadeSplits[i];

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

			// Project frustum corners into world space
			glm::mat4 invCam = glm::inverse(viewProjection);
			for (uint32 i = 0; i < 8; i++)
			{
				glm::vec4 invCorner = invCam * glm::vec4(frustumCorners[i], 1.0f);
				frustumCorners[i] = invCorner / invCorner.w;
			}

			for (uint32 i = 0; i < 4; i++)
			{
				glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
				frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
				frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
			}

			// Get frustum center
			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (uint32 i = 0; i < 8; i++)
				frustumCenter += frustumCorners[i];

			frustumCenter /= 8.0f;

			//frustumCenter *= 0.01f;

			float radius = 0.0f;
			for (uint32 i = 0; i < 8; i++)
			{
				float distance = glm::length(frustumCorners[i] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = std::ceil(radius * 16.0f) / 16.0f;

			glm::vec3 maxExtents = glm::vec3(radius);
			glm::vec3 minExtents = -maxExtents;

			glm::vec3 lightDir = -lightDirection;
			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDir * -minExtents.z, frustumCenter, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f + CascadeNearPlaneOffset, maxExtents.z - minExtents.z + CascadeFarPlaneOffset);

			// Offset to texel space to avoid shimmering (from https://stackoverflow.com/questions/33499053/cascaded-shadow-map-shimmering)
			glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
			float ShadowMapResolution = (float)m_ShadowPassPipelines[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetWidth();
			glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * ShadowMapResolution / 2.0f;
			glm::vec4 roundedOrigin = glm::round(shadowOrigin);
			glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset = roundOffset * 2.0f / ShadowMapResolution;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;

			lightOrthoMatrix[3] += roundOffset;

			// Store split distance and matrix in cascade
			cascades[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
			cascades[i].ViewProj = lightOrthoMatrix * lightViewMatrix;
			cascades[i].View = lightViewMatrix;

			lastSplitDist = cascadeSplits[i];
		}
	}

	void SceneRenderer::CalculateCascadesManualSplit(CascadeData *cascades, const SceneRendererCamera &sceneCamera, const glm::vec3 &lightDirection) const
	{
		//TODO: Reversed Z projection?

		float scaleToOrigin = m_ScaleShadowCascadesToOrigin;

		glm::mat4 viewMatrix = sceneCamera.ViewMatrix;
		constexpr glm::vec4 origin = glm::vec4(glm::vec3(0.0f), 1.0f);
		viewMatrix[3] = glm::lerp(viewMatrix[3], origin, scaleToOrigin);

		auto viewProjection = sceneCamera.Camera.GetProjection() * viewMatrix;

		const int SHADOW_MAP_CASCADE_COUNT = 4;

		float nearClip = sceneCamera.Near;
		float farClip = sceneCamera.Far;
		float clipRange = farClip - nearClip;

		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

		// Calculate orthographic projection matrix for each cascade
		float lastSplitDist = 0.0;
		for (uint32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
		{
			float splitDist = m_ShadowCascadeSplits[0];
			lastSplitDist = 0.0;

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

			// Project frustum corners into world space
			glm::mat4 invCam = glm::inverse(viewProjection);
			for (uint32 i = 0; i < 8; i++)
			{
				glm::vec4 invCorner = invCam * glm::vec4(frustumCorners[i], 1.0f);
				frustumCorners[i] = invCorner / invCorner.w;
			}

			for (uint32 i = 0; i < 4; i++)
			{
				glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
				frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
				frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
			}

			// Get frustum center
			glm::vec3 frustumCenter = glm::vec3(0.0f);
			for (uint32 i = 0; i < 8; i++)
				frustumCenter += frustumCorners[i];

			frustumCenter /= 8.0f;

			//frustumCenter *= 0.01f;

			float radius = 0.0f;
			for (uint32 i = 0; i < 8; i++)
			{
				float distance = glm::length(frustumCorners[i] - frustumCenter);
				radius = glm::max(radius, distance);
			}
			radius = std::ceil(radius * 16.0f) / 16.0f;
			radius *= m_ShadowCascadeSplits[1];

			glm::vec3 maxExtents = glm::vec3(radius);
			glm::vec3 minExtents = -maxExtents;

			glm::vec3 lightDir = -lightDirection;
			glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDir * -minExtents.z, frustumCenter, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f + CascadeNearPlaneOffset, maxExtents.z - minExtents.z + CascadeFarPlaneOffset);

			// Offset to texel space to avoid shimmering (from https://stackoverflow.com/questions/33499053/cascaded-shadow-map-shimmering)
			glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
			float ShadowMapResolution = (float)m_ShadowPassPipelines[0]->GetSpecification().RenderPass->GetSpecification().Framebuffer->GetWidth();
			glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * ShadowMapResolution / 2.0f;
			glm::vec4 roundedOrigin = glm::round(shadowOrigin);
			glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset = roundOffset * 2.0f / ShadowMapResolution;
			roundOffset.z = 0.0f;
			roundOffset.w = 0.0f;

			lightOrthoMatrix[3] += roundOffset;

			// Store split distance and matrix in cascade
			cascades[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
			cascades[i].ViewProj = lightOrthoMatrix * lightViewMatrix;
			cascades[i].View = lightViewMatrix;

			lastSplitDist = m_ShadowCascadeSplits[0];
		}
	}

	void SceneRenderer::UpdateStatistics()
	{
		m_Statistics.DrawCalls = 0;
		m_Statistics.Instances = 0;
		m_Statistics.Meshes = 0;

		for (auto &[mk, dc] : m_SelectedStaticMeshDrawList)
		{
			m_Statistics.Instances += dc.InstanceCount;
			m_Statistics.DrawCalls++;
			m_Statistics.Meshes++;
		}

		for (auto &[mk, dc] : m_StaticMeshDrawList)
		{
			m_Statistics.Instances += dc.InstanceCount;
			m_Statistics.DrawCalls++;
			m_Statistics.Meshes++;
		}

		for (auto &[mk, dc] : m_SelectedMeshDrawList)
		{
			m_Statistics.Instances += dc.InstanceCount;
			m_Statistics.DrawCalls++;
			m_Statistics.Meshes++;
		}

		for (auto &[mk, dc] : m_DrawList)
		{
			m_Statistics.Instances += dc.InstanceCount;
			m_Statistics.DrawCalls++;
			m_Statistics.Meshes++;
		}

		m_Statistics.SavedDraws = m_Statistics.Instances - m_Statistics.DrawCalls;

		uint32 frameIndex = Renderer::GetCurrentFrameIndex();
		m_Statistics.TotalGPUTime = m_CommandBuffer->GetExecutionGPUTime(frameIndex);
	}

	void SceneRenderer::SetLineWidth(const float width)
	{
		m_LineWidth = width;
		if (m_GeometryWireframePipeline)
			m_GeometryWireframePipeline->GetSpecification().LineWidth = width;
	}

}