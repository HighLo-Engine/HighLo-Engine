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

				case PrimitiveType::Lines:
					return GL_LINES;

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
			//		HL_CORE_TRACE("[OpenGL Debug NOTIFICATION] {0}", message);
					break;
					}
			}
		}
	}

	void OpenGLRenderingAPI::Init()
	{
		Renderer::Submit([]()
		{
			glDebugMessageCallback(utils::OpenGLLogMessage, nullptr);
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			// Load Renderer Caps
			auto &caps = Renderer::GetCapabilities();
			caps.Vendor = (const char*)glGetString(GL_VENDOR);
			caps.Device = (const char*)glGetString(GL_RENDERER);
			caps.Version = (const char*)glGetString(GL_VERSION);
			glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &caps.MaxTextureUnits);
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy);

			utils::DumpGPUInfos();

			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // for seamless cube maps
			glFrontFace(GL_CCW);

			glEnable(GL_DEPTH_TEST);
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
		});
	}

	void OpenGLRenderingAPI::Shutdown()
	{
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

	void OpenGLRenderingAPI::DrawIndexed(uint32 indexCount, PrimitiveType type, bool depthTest)
	{
		if (!depthTest)
			SetDepthTest(false);

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

	void OpenGLRenderingAPI::BeginFrame()
	{
	}

	void OpenGLRenderingAPI::EndFrame()
	{
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

	Ref<Environment> OpenGLRenderingAPI::CreateEnvironment(const HLString &path, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		if (!Renderer::GetConfig().ComputeEnvironmentMaps)
			return Renderer::GetEmptyEnvironment();

		Ref<OpenGLShader> equirectangularConversionShader = Renderer::GetShaderLibrary()->Get("EquirectangularToCubeMap").As<OpenGLShader>();
		Ref<OpenGLShader> envFilteringShader = Renderer::GetShaderLibrary()->Get("EnvironmentMipFilter").As<OpenGLShader>();
		Ref<OpenGLShader> envIrradianceShader = Renderer::GetShaderLibrary()->Get("EnvironmentIrradiance").As<OpenGLShader>();

		Ref<Texture3D> envUnfiltered = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);
		Ref<Texture3D> envFiltered = Texture3D::Create(TextureFormat::RGBA32F, cubemapSize, cubemapSize);
		Ref<Texture3D> irradianceMap = Texture3D::Create(TextureFormat::RGBA32F, irradianceMapSize, irradianceMapSize);

		Ref<Texture2D> envEquirect = Texture2D::LoadFromFile(path, TextureFormat::RGBA32F);
		HL_ASSERT(envEquirect->GetFormat() == TextureFormat::RGBA32F, "Texture is not a HDR Texture!");

		equirectangularConversionShader->Bind();
		envEquirect->Bind(1);

		// Create unfiltered environment map
		Renderer::Submit([envUnfiltered, cubemapSize, envEquirect, envFiltered]()
		{
			 glBindImageTexture(0, envUnfiltered->GetRendererID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			 glDispatchCompute(cubemapSize / 32, cubemapSize / 32, 6);
			 glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			 glGenerateTextureMipmap(envUnfiltered->GetRendererID());

			 // Create filtered environment map
			 glCopyImageSubData(envUnfiltered->GetRendererID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
								envFiltered->GetRendererID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
								envFiltered->GetWidth(), envFiltered->GetHeight(), 6);
		});

		envFilteringShader->Bind();
		envUnfiltered->Bind(1);

		Renderer::Submit([envFilteringShader, envFiltered, cubemapSize]() mutable
		{
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
		});

		envIrradianceShader->Bind();
		envFiltered->Bind(1);

		Renderer::Submit([irradianceMap]()
		{
			// Create irradiance map
			glBindImageTexture(0, irradianceMap->GetRendererID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			glDispatchCompute(irradianceMap->GetWidth() / 32, irradianceMap->GetHeight() / 32, 6);
			glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			glGenerateTextureMipmap(irradianceMap->GetRendererID());
		});

		return Ref<Environment>::Create(envUnfiltered, envFiltered, irradianceMap, nullptr);
	}
}

#endif // HIGHLO_API_OPENGL
