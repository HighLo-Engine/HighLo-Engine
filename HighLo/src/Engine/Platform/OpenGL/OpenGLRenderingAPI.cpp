#include "HighLoPch.h"
#include "OpenGLRenderingAPI.h"

#ifdef HIGHLO_API_OPENGL

#include <glm/glm.hpp>
#include <glm/gtx/extended_min_max.hpp>
#include <glad/glad.h>

#include "Engine/Renderer/Renderer.h"
#include "OpenGLImage.h"
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
	}

	void OpenGLRenderingAPI::ClearScreenColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderingAPI::ClearScreenBuffers()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderingAPI::DrawIndexed(Ref<VertexArray>& va, PrimitiveType type)
	{
		glDrawElements(utils::ConvertToOpenGLPrimitiveType(type), va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderingAPI::DrawInstanced(Ref<VertexArray>& va, uint32 count, PrimitiveType type)
	{
		glDrawElementsInstanced(utils::ConvertToOpenGLPrimitiveType(type), va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);
	}
	
	void OpenGLRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray>& va, PrimitiveType type)
	{
		glDrawElements(utils::ConvertToOpenGLPrimitiveType(type), va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	
	void OpenGLRenderingAPI::SetWireframe(bool wf)
	{
		glPolygonMode(GL_FRONT_AND_BACK, wf ? GL_LINE : GL_FILL);
	}
	
	void OpenGLRenderingAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
		glViewport(x, y, width, height);
	}

	Ref<Environment> OpenGLRenderingAPI::CreateEnvironment(const HLString &path)
	{
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		if (!Renderer::GetConfig().ComputeEnvironmentMaps)
			return Renderer::GetEmptyEnvironment();

		const uint32 cubemapSize = 2048;
		const uint32 irradianceMapSize = 32;

		Ref<Shader> equirectangularConversionShader = Renderer::GetShaderLibrary()->Get("EquirectangularToCubeMap");
		Ref<Shader> envFilteringShader = Renderer::GetShaderLibrary()->Get("EnvironmentMipFilter");
		Ref<Shader> envIrradianceShader = Renderer::GetShaderLibrary()->Get("EnvironmentIrradiance");

		static auto environmentBuffer = UniformBuffer::Create(
			"EnvironmentBuffer",
			{ UniformVariable("Roughness", sizeof(float)) },
			UniformBufferParentShader::COMPUTE_SHADER,
			(uint32)HL_UB_SLOT::ENVIRONMENT_DATA_BUFFER);

		envFilteringShader->AddBuffer("EnvironmentBuffer", environmentBuffer);

		Ref<Texture3D> envUnfiltered = Texture3D::Create(ImageFormat::RGBA32F, cubemapSize, cubemapSize);
		Ref<Texture3D> envFiltered = Texture3D::Create(ImageFormat::RGBA32F, cubemapSize, cubemapSize);
		Ref<Texture3D> irradianceMap = Texture3D::Create(ImageFormat::RGBA32F, irradianceMapSize, irradianceMapSize);

		Ref<Texture2D> envEquirect = Texture2D::LoadFromFile(path, ImageFormat::RGBA32F);
		HL_ASSERT(envEquirect->GetFormat() == ImageFormat::RGBA32F, "Texture is not a HDR Texture!");

		equirectangularConversionShader->Bind();
		envEquirect->Bind(1);

		// Create unfiltered environment map
		glBindImageTexture(0, envUnfiltered->GetRendererID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute(cubemapSize / 32, cubemapSize / 32, 6);
		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		glGenerateTextureMipmap(envUnfiltered->GetRendererID());

		// Create filtered environment map
		glCopyImageSubData(envUnfiltered->GetRendererID(), GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
						   envFiltered->GetRendererID(),   GL_TEXTURE_CUBE_MAP, 0, 0, 0, 0,
						   envFiltered->GetWidth(), envFiltered->GetHeight(), 6);

		envFilteringShader->Bind();
		envUnfiltered->Bind(1);

		const float deltaRoughness = 1.0f / glm::max((float)(envFiltered->GetMipLevelCount() - 1.0f), 1.0f);
		for (int32 level = 1, size = cubemapSize / 2; level < (int32)envFiltered->GetMipLevelCount(); ++level, size /= 2)
		{
			glBindImageTexture(0, envFiltered->GetRendererID(), level, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);

			Ref<UniformBuffer> buffer = envFilteringShader->GetBuffer("EnvironmentBuffer");
			float value = (float)level * deltaRoughness;
			buffer->SetBufferValue(&value);
			buffer->UploadToShader();

			const GLuint numGroups = glm::max(1, size / 32);
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

		return Ref<Environment>::Create(envUnfiltered, envFiltered, irradianceMap, nullptr);
	}
}

#endif // HIGHLO_API_OPENGL
