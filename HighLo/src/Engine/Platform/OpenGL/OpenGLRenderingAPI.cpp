// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLRenderingAPI.h"

#ifdef HIGHLO_API_OPENGL

#include <glm/glm.hpp>
#include <glm/gtx/extended_min_max.hpp>
#include <glad/glad.h>

#include "Engine/Renderer/Renderer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture2D.h"
#include "OpenGLTexture3D.h"
#include "OpenGLMaterial.h"
#include "OpenGLVertexArray.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"

#define GL_RENDERING_API_LOG_PREFIX "OPENGL>       "

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
					HL_CORE_ERROR(GL_RENDERING_API_LOG_PREFIX "[-] {0} [-]", message);
					HL_ASSERT(false);
					break;
				}

				case GL_DEBUG_SEVERITY_MEDIUM:
				{
					HL_CORE_WARN(GL_RENDERING_API_LOG_PREFIX "[-] {0} [-]", message);
					break;
				}

				case GL_DEBUG_SEVERITY_LOW:
				{
					HL_CORE_INFO(GL_RENDERING_API_LOG_PREFIX "[+] {0} [+]", message);
					break;
				}

				case GL_DEBUG_SEVERITY_NOTIFICATION:
				{
				//	HL_CORE_TRACE(GL_RENDERING_API_LOG_PREFIX "[=] {0} [=]", message);
					break;
				}
			}
		}
	}

	struct GLQuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	struct AnimatedBoneTransformUniformBuffer
	{
		glm::mat4 BoneTransform[100];
	};

	struct GLRendererData
	{
		Ref<VertexBuffer> FullscreenQuadVertexBuffer;
		Ref<IndexBuffer> FullscreenQuadIndexBuffer;
		Ref<RenderPass> ActiveRenderPass;
	};

	static GLRendererData *s_GLRendererData;

	void OpenGLRenderingAPI::Init()
	{
		s_GLRendererData = new GLRendererData();

		glDebugMessageCallback(utils::OpenGLLogMessage, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Load Renderer Caps
		auto& caps = Renderer::GetCapabilities();
		caps.Vendor = (const char*)glGetString(GL_VENDOR);
		caps.Device = (const char*)glGetString(GL_RENDERER);
		caps.Version = (const char*)glGetString(GL_VERSION);
		glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &caps.MaxTextureUnits);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &caps.MaxTextures);
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
	//	HL_ASSERT(error == GL_NO_ERROR, fmt::format("OpenGL Error: {}", error).c_str());
		while (error != GL_NO_ERROR)
		{
			HL_CORE_ERROR(GL_RENDERING_API_LOG_PREFIX "[-] OpenGL Error: {0} [-]", error);
			error = glGetError();
		//	HL_ASSERT(error == GL_NO_ERROR, fmt::format("OpenGL Error: {}", error).c_str());
		}

		float x = -1;
		float y = -1;
		float width = 2, height = 2;

		GLQuadVertex *data = new GLQuadVertex[4];

		data[0].Position = glm::vec3(x, y, 0.1f);
		data[0].TexCoord = glm::vec2(0, 0);

		data[1].Position = glm::vec3(x + width, y, 0.1f);
		data[1].TexCoord = glm::vec2(1, 0);

		data[2].Position = glm::vec3(x + width, y + height, 0.1f);
		data[2].TexCoord = glm::vec2(1, 1);

		data[3].Position = glm::vec3(x, y + height, 0.1f);
		data[3].TexCoord = glm::vec2(0, 1);

		uint32 indices[6] = { 0, 1, 2, 2, 3, 0, };

		s_GLRendererData->FullscreenQuadVertexBuffer = VertexBuffer::Create(data, 4 * sizeof(GLQuadVertex));
		s_GLRendererData->FullscreenQuadIndexBuffer = IndexBuffer::Create(indices, 6 * sizeof(uint32));
	}

	void OpenGLRenderingAPI::Shutdown()
	{
		OpenGLShader::ClearUniformBuffers();
		delete s_GLRendererData;
	}

	void OpenGLRenderingAPI::BeginFrame()
	{
	}

	void OpenGLRenderingAPI::EndFrame()
	{
	}

	void OpenGLRenderingAPI::BeginRenderPass(const Ref<CommandBuffer> &renderCommandBuffer, const Ref<RenderPass> &renderPass, bool shouldClear)
	{
		HL_ASSERT(!s_GLRendererData->ActiveRenderPass, "Another RenderPass has already been started and not ended!");
		s_GLRendererData->ActiveRenderPass = renderPass;
	
		s_GLRendererData->ActiveRenderPass->GetSpecification().Framebuffer->Bind();
		if (shouldClear)
		{
			const glm::vec4 &clearColor = s_GLRendererData->ActiveRenderPass->GetSpecification().Framebuffer->GetSpecification().ClearColor;
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	void OpenGLRenderingAPI::EndRenderPass(const Ref<CommandBuffer> &renderCommandBuffer)
	{
		HL_ASSERT(s_GLRendererData->ActiveRenderPass, "Did you forget to call BeginRenderPass() ?");
	
		s_GLRendererData->ActiveRenderPass->GetSpecification().Framebuffer->Unbind();
		s_GLRendererData->ActiveRenderPass = nullptr;
	}

	void OpenGLRenderingAPI::ClearScreenColor(const glm::vec4 &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderingAPI::ClearScreenBuffers()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderingAPI::DrawIndexed(Ref<VertexArray> &va, PrimitiveType type)
	{
		glDrawElements(utils::ConvertToOpenGLPrimitiveType(type), va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderingAPI::DrawIndexed(uint32 indexCount, Ref<Material> &material, Ref<UniformBufferSet> &uniformBufferSet, PrimitiveType type, bool depthTest, const glm::mat4 &localTransform)
	{
		if (!depthTest)
			SetDepthTest(false);

		material->UpdateForRendering();
		glDrawElements(utils::ConvertToOpenGLPrimitiveType(type), indexCount, GL_UNSIGNED_INT, nullptr);

		if (!depthTest)
			SetDepthTest(true);
	}

	void OpenGLRenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
		glDrawElementsInstanced(utils::ConvertToOpenGLPrimitiveType(type), va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);
	}

	void OpenGLRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
		glDrawElements(utils::ConvertToOpenGLPrimitiveType(type), va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderingAPI::DrawFullscreenQuad(Ref<CommandBuffer> &renderCommandBuffer, Ref<VertexArray> &va, const Ref<UniformBufferSet> &uniformBufferSet, const Ref<StorageBufferSet> &storageBufferSet, Ref<Material> &material, const glm::mat4 &transform)
	{
		va->Bind();
		s_GLRendererData->FullscreenQuadVertexBuffer->Bind();
		s_GLRendererData->FullscreenQuadIndexBuffer->Bind();
		
		if (material)
		{
			material->UpdateForRendering();
		}

		glDrawElements(GL_TRIANGLES, s_GLRendererData->FullscreenQuadIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderingAPI::DrawStaticMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset)
	{
		va->Bind();
		model->Get()->GetVertexBuffer()->Bind();
		model->Get()->GetIndexBuffer()->Bind();

		auto &submeshes = model->Get()->GetSubmeshes();
		for (Mesh submesh : submeshes)
		{
			auto material = materials->GetMaterial(submesh.MaterialIndex).As<OpenGLMaterial>();
			// TODO: This if is temporary because we do not have the material system setup yet
			if (material)
			{
				material->UpdateForRendering();
				SetDepthTest(material->GetFlag(MaterialFlag::DepthTest));
			}

			if (model->Get()->m_IsAnimated)
			{
				AnimatedBoneTransformUniformBuffer buffer;
				for (uint64 i = 0; model->Get()->m_BoneTransforms.size(); ++i)
				{
					HLString uniformName = HLString("u_BoneTransforms[") + HLString::ToString(i) + HLString("]"); // unused, can go away if not needed for debug print
					buffer.BoneTransform[i] = model->Get()->m_BoneTransforms[i];
				}

				Ref<UniformBuffer> ub = UniformBuffer::Create(sizeof(AnimatedBoneTransformUniformBuffer), 22);
				ub->SetData(&buffer, sizeof(buffer));
				ub->Bind();
			}

			glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32) * submesh.BaseIndex), submesh.BaseVertex);
		}
	}

	void OpenGLRenderingAPI::DrawDynamicMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset)
	{
		va->Bind();
		model->Get()->GetVertexBuffer()->Bind();
		model->Get()->GetIndexBuffer()->Bind();

		auto &submeshes = model->Get()->GetSubmeshes();
		for (Mesh submesh : submeshes)
		{
			auto material = materials->GetMaterial(submesh.MaterialIndex).As<OpenGLMaterial>();
			// TODO: This if is temporary because we do not have the material system setup yet
			if (material)
			{
				material->UpdateForRendering();
				SetDepthTest(material->GetFlag(MaterialFlag::DepthTest));
			}

			if (model->Get()->m_IsAnimated)
			{
				AnimatedBoneTransformUniformBuffer buffer;
				for (uint64 i = 0; model->Get()->m_BoneTransforms.size(); ++i)
				{
					HLString uniformName = HLString("u_BoneTransforms[") + HLString::ToString(i) + HLString("]");
					buffer.BoneTransform[i] = model->Get()->m_BoneTransforms[i];
				}

				Ref<UniformBuffer> ub = UniformBuffer::Create(sizeof(AnimatedBoneTransformUniformBuffer), 22);
				ub->SetData(&buffer, sizeof(buffer));
				ub->Bind();
			}

			glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32) * submesh.BaseIndex), submesh.BaseVertex);
		}
	}

	void OpenGLRenderingAPI::DrawInstancedStaticMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
		va->Bind();
		model->Get()->GetVertexBuffer()->Bind();
		model->Get()->GetIndexBuffer()->Bind();

		auto &submeshes = model->Get()->GetSubmeshes();
		for (Mesh submesh : submeshes)
		{
			auto material = materials->GetMaterial(submesh.MaterialIndex).As<OpenGLMaterial>();
			// TODO: This if is temporary because we do not have the material system setup yet
			if (material)
			{				
				material->UpdateForRendering();
				SetDepthTest(material->GetFlag(MaterialFlag::DepthTest));
			}

			if (model->Get()->m_IsAnimated)
			{
				AnimatedBoneTransformUniformBuffer buffer;
				for (uint64 i = 0; model->Get()->m_BoneTransforms.size(); ++i)
				{
					HLString uniformName = HLString("u_BoneTransforms[") + HLString::ToString(i) + HLString("]");
					buffer.BoneTransform[i] = model->Get()->m_BoneTransforms[i];
				}

				Ref<UniformBuffer> ub = UniformBuffer::Create(sizeof(AnimatedBoneTransformUniformBuffer), 22);
				ub->SetData(&buffer, sizeof(buffer));
				ub->Bind();
			}

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32) * submesh.BaseIndex), instanceCount, submesh.BaseVertex);
		}
	}

	void OpenGLRenderingAPI::DrawInstancedDynamicMesh(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<MaterialTable> materials, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount)
	{
		va->Bind();
		model->Get()->GetVertexBuffer()->Bind();
		model->Get()->GetIndexBuffer()->Bind();

		auto &submeshes = model->Get()->GetSubmeshes();
		for (Mesh submesh : submeshes)
		{
			auto material = materials->GetMaterial(submesh.MaterialIndex).As<OpenGLMaterial>();
			// TODO: This if is temporary because we do not have the material system setup yet
			if (material)
			{
				material->UpdateForRendering();
				SetDepthTest(material->GetFlag(MaterialFlag::DepthTest));
			}

			if (model->Get()->m_IsAnimated)
			{
				AnimatedBoneTransformUniformBuffer buffer;
				for (uint64 i = 0; model->Get()->m_BoneTransforms.size(); ++i)
				{
					HLString uniformName = HLString("u_BoneTransforms[") + HLString::ToString(i) + HLString("]");
					buffer.BoneTransform[i] = model->Get()->m_BoneTransforms[i];
				}

				Ref<UniformBuffer> ub = UniformBuffer::Create(sizeof(AnimatedBoneTransformUniformBuffer), 22);
				ub->SetData(&buffer, sizeof(buffer));
				ub->Bind();
			}

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32) * submesh.BaseIndex), instanceCount, submesh.BaseVertex);
		}
	}

	void OpenGLRenderingAPI::DrawInstancedStaticMeshWithMaterial(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<StaticModel> model, uint32 submeshIndex, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount, Ref<Material> overrideMaterial)
	{
		va->Bind();
		model->Get()->GetVertexBuffer()->Bind();
		model->Get()->GetIndexBuffer()->Bind();

		auto &submeshes = model->Get()->GetSubmeshes();
		for (Mesh &submesh : submeshes)
		{
			overrideMaterial->UpdateForRendering();
			SetDepthTest(overrideMaterial->GetFlag(MaterialFlag::DepthTest));

			if (model->Get()->m_IsAnimated)
			{
				AnimatedBoneTransformUniformBuffer buffer;
				for (uint64 i = 0; model->Get()->m_BoneTransforms.size(); ++i)
				{
					HLString uniformName = HLString("u_BoneTransforms[") + HLString::ToString(i) + HLString("]");
					buffer.BoneTransform[i] = model->Get()->m_BoneTransforms[i];
				}

				Ref<UniformBuffer> ub = UniformBuffer::Create(sizeof(AnimatedBoneTransformUniformBuffer), 22);
				ub->SetData(&buffer, sizeof(buffer));
				ub->Bind();
			}

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void *)(sizeof(uint32) * submesh.BaseIndex), instanceCount, submesh.BaseVertex);
		}
	}

	void OpenGLRenderingAPI::DrawInstancedDynamicMeshWithMaterial(Ref<CommandBuffer> renderCommandBuffer, Ref<VertexArray> va, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<DynamicModel> model, uint32 submeshIndex, Ref<VertexBuffer> transformBuffer, uint32 transformBufferOffset, uint32 instanceCount, Ref<Material> overrideMaterial)
	{
		va->Bind();
		model->Get()->GetVertexBuffer()->Bind();
		model->Get()->GetIndexBuffer()->Bind();

		auto &submeshes = model->Get()->GetSubmeshes();
		for (Mesh &submesh : submeshes)
		{
			overrideMaterial->UpdateForRendering();
			SetDepthTest(overrideMaterial->GetFlag(MaterialFlag::DepthTest));

			if (model->Get()->m_IsAnimated)
			{
				AnimatedBoneTransformUniformBuffer buffer;
				for (uint64 i = 0; model->Get()->m_BoneTransforms.size(); ++i)
				{
					HLString uniformName = HLString("u_BoneTransforms[") + HLString::ToString(i) + HLString("]");
					buffer.BoneTransform[i] = model->Get()->m_BoneTransforms[i];
				}

				Ref<UniformBuffer> ub = UniformBuffer::Create(sizeof(AnimatedBoneTransformUniformBuffer), 22);
				ub->SetData(&buffer, sizeof(buffer));
				ub->Bind();
			}

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void *)(sizeof(uint32) * submesh.BaseIndex), instanceCount, submesh.BaseVertex);
		}
	}

	void OpenGLRenderingAPI::SetWireframe(bool wf)
	{
		glPolygonMode(GL_FRONT_AND_BACK, wf ? GL_LINE : GL_FILL);
	}

	void OpenGLRenderingAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderingAPI::SetBlendMode(bool bEnabled)
	{
		if (bEnabled)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void OpenGLRenderingAPI::SetMultiSample(bool bEnabled)
	{
		if (bEnabled)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
	}

	void OpenGLRenderingAPI::SetDepthTest(bool bEnabled)
	{
		if (bEnabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRenderingAPI::SetLineThickness(float thickness)
	{
		glLineWidth(thickness);
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

		return Ref<Environment>::Create(filePath, envUnfiltered, envFiltered, irradianceMap);
	}
	
	Ref<Texture3D> OpenGLRenderingAPI::CreatePreethamSky(float turbidity, float azimuth, float inclination)
	{
		const uint32 cubemapSize = Renderer::GetConfig().EnvironmentMapResolution;
		Ref<Texture3D> envMap = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);
		glm::vec3 params = { turbidity, azimuth, inclination };

		Ref<OpenGLShader> preethamSkyShader = Renderer::GetShaderLibrary()->Get("PreethamSky").As<OpenGLShader>();
		preethamSkyShader->Bind();
		// TODO: use Uniform buffers instead (for reference see SceneRenderer.cpp)
	//	preethamSkyShader->SetUniform("u_Uniforms.TurbidityAzimuthInclination", params);

		return envMap;
	}
}

#endif // HIGHLO_API_OPENGL

