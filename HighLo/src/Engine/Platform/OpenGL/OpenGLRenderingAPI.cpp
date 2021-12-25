// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLRenderingAPI.h"

#ifdef HIGHLO_API_OPENGL

#include <glm/glm.hpp>
#include <glm/gtx/extended_min_max.hpp>
#include <glad/glad.h>

#include "Engine/Renderer/Renderer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLMaterial.h"
#include "OpenGLVertexArray.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"

namespace highlo
{
	namespace utils
	{
		static GLenum ConvertToOpenGLPrimitiveType(PrimitiveType type)
		{
			switch (type)
			{
				case PrimitiveType::None:
				case PrimitiveType::Triangles:
					return GL_TRIANGLES;

				case PrimitiveType::TriangleStrip:
					return GL_TRIANGLE_STRIP;

				case PrimitiveType::Lines:
					return GL_LINES;

				case PrimitiveType::LineStrip:
					return GL_LINE_STRIP;

				case PrimitiveType::Patch:
					return GL_PATCHES;
			}

			HL_ASSERT(false);
			return 0;
		}

		static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
		{
			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH:
				{
					HL_CORE_ERROR("[OpenGL Debug HIGH] {0}", message);
					break;
				}

				case GL_DEBUG_SEVERITY_MEDIUM:
				{
					HL_CORE_WARN("[OpenGL Debug MEDIUM] {0}", message);
					break;
				}

				case GL_DEBUG_SEVERITY_LOW:
				{
					HL_CORE_INFO("[OpenGL Debug LOW] {0}", message);
					break;
				}

				case GL_DEBUG_SEVERITY_NOTIFICATION:
				{
					// HL_CORE_TRACE("[OpenGL Debug NOTIFICATION] {0}", message);
					break;
				}
			}
		}

		static void Clear(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		static void SetClearColor(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
		}

		static void DrawIndexed(uint32 count, PrimitiveType type, bool depthTest)
		{
			if (!depthTest)
				glDisable(GL_DEPTH_TEST);

			GLenum glPrimitiveType = utils::ConvertToOpenGLPrimitiveType(type);
			glDrawElements(glPrimitiveType, count, GL_UNSIGNED_INT, nullptr);

			if (!depthTest)
				glEnable(GL_DEPTH_TEST);
		}

		static void SetLineThickness(float thickness)
		{
			glLineWidth(thickness);
		}
	}

	struct OpenGLDrawData
	{
		Ref<VertexBuffer> FullscreenQuadVertexBuffer;
		Ref<IndexBuffer> FullscreenQuadIndexBuffer;
		VertexArraySpecification FullscreenQuadVertexArraySpec;

		Ref<RenderPass> ActiveRenderPass;
	};

	struct OpenGLQuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	static OpenGLDrawData *s_GLDrawData;

	void OpenGLRenderingAPI::Init()
	{
		s_GLDrawData = new OpenGLDrawData();

		glDebugMessageCallback(utils::OpenGLLogMessage, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		// Load Renderer Caps
		auto& caps = Renderer::GetCapabilities();
		caps.Vendor = (const char*)glGetString(GL_VENDOR);
		caps.Device = (const char*)glGetString(GL_RENDERER);
		caps.Version = (const char*)glGetString(GL_VERSION);
		glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &caps.MaxTextureUnits);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy);

		utils::DumpGPUInfos();

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // for seamless cube maps
		glFrontFace(GL_CCW);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_STENCIL_TEST);

		GLenum error = glGetError();
		while (error != GL_NO_ERROR)
		{
			HL_CORE_ERROR("OpenGL Error: {0}", error);
			error = glGetError();
		}

		float x = -1;
		float y = -1;
		float width = 2, height = 2;

		OpenGLQuadVertex *data = new OpenGLQuadVertex[4];

		data[0].Position = glm::vec3(x, y, 0.1f);
		data[0].TexCoord = glm::vec2(0, 0);

		data[1].Position = glm::vec3(x + width, y, 0.1f);
		data[1].TexCoord = glm::vec2(1, 0);

		data[2].Position = glm::vec3(x + width, y + height, 0.1f);
		data[2].TexCoord = glm::vec2(1, 1);

		data[3].Position = glm::vec3(x, y + height, 0.1f);
		data[3].TexCoord = glm::vec2(0, 1);

		s_GLDrawData->FullscreenQuadVertexBuffer = VertexBuffer::Create(data, 4 * sizeof(OpenGLQuadVertex));
		
		uint32 indices[6] = { 0, 1, 2, 2, 3, 0, };
		s_GLDrawData->FullscreenQuadIndexBuffer = IndexBuffer::Create(indices, 6 * sizeof(uint32));
	}

	void OpenGLRenderingAPI::Shutdown()
	{
		OpenGLShader::ClearUniformBuffers();
		delete s_GLDrawData;
	}

	void OpenGLRenderingAPI::BeginFrame()
	{
	}

	void OpenGLRenderingAPI::EndFrame()
	{
	}

	void OpenGLRenderingAPI::BeginRenderPass(Ref<CommandBuffer> renderCommandBuffer, Ref<RenderPass> renderPass, bool shouldClear)
	{
	//	HL_ASSERT(!s_GLDrawData->ActiveRenderPass, "Another RenderPass has already been started and not ended!");
	//	s_GLDrawData->ActiveRenderPass = renderPass;
	//
	//	renderPass->GetSpecification().Framebuffer->Bind();
	}

	void OpenGLRenderingAPI::EndRenderPass(Ref<CommandBuffer> renderCommandBuffer)
	{
	//	HL_ASSERT(s_GLDrawData->ActiveRenderPass, "Did you forget to call BeginRenderPass() ?");
	//
	//	s_GLDrawData->ActiveRenderPass->GetSpecification().Framebuffer->Unbind();
	//	s_GLDrawData->ActiveRenderPass = nullptr;
	}

	void OpenGLRenderingAPI::RenderDynamicModel(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materialTable, Ref<VertexBuffer> transformBuffer, uint32 transformOffset, uint32 instanceCount)
	{
	}

	void OpenGLRenderingAPI::RenderStaticModel(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materialTable, const Transform &transform)
	{
		Ref<MeshFile> meshAsset = model->Get();
		Ref<OpenGLVertexBuffer> vbo = meshAsset->GetVertexBuffer().As<OpenGLVertexBuffer>();
		Ref<OpenGLIndexBuffer> ibo = meshAsset->GetIndexBuffer().As<OpenGLIndexBuffer>();

		vbo->Bind();
		va->Bind();
		ibo->Bind();

		auto &materials = model->GetMaterials()->GetMaterials();
		for (Mesh &submesh : meshAsset->GetSubmeshes())
		{
			auto material = materials[submesh.MaterialIndex].As<OpenGLMaterial>();
			auto shader = material->GetShader().As<OpenGLShader>();
			
			shader->Bind();
			material->UpdateForRendering();

			auto finalTransform = transform.GetTransform() * submesh.LocalTransform.GetTransform();
			shader->SetUniform("u_Renderer.Transform", finalTransform);

			if (material->GetFlag(MaterialFlag::DepthTest))
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);

			glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(submesh.BaseIndex * sizeof(uint32)), submesh.BaseVertex);
		}
	}

	void OpenGLRenderingAPI::RenderDynamicModelWithMaterial(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<VertexBuffer> transformBuffer, uint32 transformOffset, uint32 instanceCount, Ref<Material> material, Allocator additionalUniforms)
	{
		// TODO
	}

	void OpenGLRenderingAPI::RenderStaticModelWithMaterial(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materialTable, const Transform &transform)
	{
		Ref<MeshFile> meshAsset = model->Get();
		Ref<OpenGLVertexBuffer> vbo = meshAsset->GetVertexBuffer().As<OpenGLVertexBuffer>();
		Ref<OpenGLIndexBuffer> ibo = meshAsset->GetIndexBuffer().As<OpenGLIndexBuffer>();

		vbo->Bind();
		va->Bind();
		ibo->Bind();

		auto shader = va->GetSpecification().Shader.As<OpenGLShader>();
		shader->Bind();

		auto &materials = materialTable->GetMaterials();
		for (Mesh &submesh : meshAsset->GetSubmeshes())
		{
			auto material = materials[submesh.MaterialIndex].As<OpenGLMaterial>();
			material->UpdateForRendering();

			auto finalTransform = transform.GetTransform() * submesh.LocalTransform.GetTransform();
			shader->SetUniform("u_Renderer.Transform", finalTransform);

			if (material->GetFlag(MaterialFlag::DepthTest))
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);

			glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(submesh.BaseIndex * sizeof(uint32)), submesh.BaseVertex);
		}
	}

	void OpenGLRenderingAPI::RenderQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const Transform &transform)
	{
		s_GLDrawData->FullscreenQuadVertexBuffer->Bind();
		va->Bind();
		s_GLDrawData->FullscreenQuadIndexBuffer->Bind();

		Ref<OpenGLMaterial> glMaterial = material.As<OpenGLMaterial>();
		glMaterial->UpdateForRendering();

		auto shader = material->GetShader().As<OpenGLShader>();
		shader->SetUniform("u_Renderer.Transform", transform.GetTransform());

		if (material->GetFlag(MaterialFlag::DepthTest))
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glDrawElements(GL_TRIANGLES, s_GLDrawData->FullscreenQuadIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderingAPI::RenderGeometry(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const Transform &transform, uint32 indexCount)
	{
		
	}

	void OpenGLRenderingAPI::SubmitFullscreenQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<Material> material)
	{
	}

	void OpenGLRenderingAPI::SubmitFullscreenQuad(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material)
	{
	}

	void OpenGLRenderingAPI::SubmitFullscreenQuadWithOverrides(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<Material> material, Allocator vertexShaderOverrides, Allocator fragmentShaderOverrides)
	{
	}

	void OpenGLRenderingAPI::DispatchComputeShader(Ref<CommandBuffer> renderCommandBuffer, Ref<ComputePipeline> computePipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::ivec3 &groups)
	{
	}

	void OpenGLRenderingAPI::ClearTexture(Ref<CommandBuffer> renderCommandBuffer, Ref<Texture2D> texture)
	{
		
	}

	Ref<Environment> OpenGLRenderingAPI::CreateEnvironment(const FileSystemPath &filePath, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		if (!Renderer::GetConfig().ComputeEnvironmentMaps)
			return Renderer::GetEmptyEnvironment();

		Ref<OpenGLShader> equirectangularConversionShader = Renderer::GetShaderLibrary()->Get("EquirectangularToCubeMap").As<OpenGLShader>();
		Ref<OpenGLShader> envFilteringShader = Renderer::GetShaderLibrary()->Get("EnvironmentMipFilter").As<OpenGLShader>();
		Ref<OpenGLShader> envIrradianceShader = Renderer::GetShaderLibrary()->Get("EnvironmentIrradiance").As<OpenGLShader>();

		Ref<Texture3D> envUnfiltered = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);
		Ref<Texture3D> envFiltered = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);
		Ref<Texture3D> irradianceMap = Texture3D::Create(TextureFormat::RGBA32F, irradianceMapSize, irradianceMapSize);

		Ref<Texture2D> envEquirect = Texture2D::LoadFromFile(filePath, TextureFormat::RGBA32F);
		HL_ASSERT(envEquirect->GetFormat() == TextureFormat::RGBA32F, "Texture is not a HDR Texture!");

		equirectangularConversionShader->Bind();
		envEquirect->Bind(1);

		// Create unfiltered environment map
		glBindImageTexture(0, envUnfiltered->GetRendererID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(cubemapSize / 32, cubemapSize / 32, 6);
		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		glGenerateTextureMipmap(envUnfiltered->GetRendererID());

		// Create filtered environment map
		glCopyImageSubData(envUnfiltered->GetRendererID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
			envFiltered->GetRendererID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
			envFiltered->GetWidth(), envFiltered->GetHeight(), 6);

		envFilteringShader->Bind();
		envUnfiltered->Bind(1);

		const float deltaRoughness = 1.0f / glm::max((float)(envFiltered->GetMipLevelCount() - 1u), 1.0f);
		for (uint32 level = 1, size = cubemapSize / 2; level < envFiltered->GetMipLevelCount(); ++level, size /= 2)
		{
			glBindImageTexture(0, envFiltered->GetRendererID(), level, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);

			// Upload Roughness to Shader
			float value = (float)level * deltaRoughness;
			const GLint roughnessShaderLocation = glGetUniformLocation(envFilteringShader->GetRendererID(), "u_Uniforms.Roughness");
			HL_ASSERT(roughnessShaderLocation != -1);
			glUniform1f(roughnessShaderLocation, value);

			const GLuint numGroups = glm::max(1u, size / 32);
			glDispatchCompute(numGroups, numGroups, 6);
			glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}

		envIrradianceShader->Bind();
		envFiltered->Bind(1);

		// Create irradiance map
		glBindImageTexture(0, irradianceMap->GetRendererID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(irradianceMap->GetWidth() / 32, irradianceMap->GetHeight() / 32, 6);
		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		glGenerateTextureMipmap(irradianceMap->GetRendererID());

		return Ref<Environment>::Create(filePath, envUnfiltered, envFiltered, irradianceMap, Renderer::GetBRDFLutTexture());
	}
}

#endif // HIGHLO_API_OPENGL
