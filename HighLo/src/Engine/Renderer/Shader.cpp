#include "HighLoPch.h"
#include "Shader.h"
#include <filesystem>
#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLShader.h"
#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Shader.h"
#endif // HIGHLO_API_DX11

#include "Engine/Math/HLMath.h"

namespace highlo
{
	Ref<UniformBuffer> Shader::m_VS_SceneBuffer;

#ifdef HIGHLO_API_OPENGL
    Ref<Shader> Shader::Create(const ShaderSource& source, const BufferLayout& layout)
    {
		static bool first_time = true;
		if (first_time)
		{
			CreateVSSceneBuffer();
			first_time = false;
		}

        return Ref<OpenGLShader>::Create(source);
    }
	
	Ref<Shader> Shader::CreateComputeShader(const ShaderSource& source)
	{
		return Ref<OpenGLShader>::Create(source, true);
	}

#endif // HIGHLO_API_OPENGL
#ifdef HIGHLO_API_DX11
    Ref<Shader> Shader::Create(const ShaderSource& source, const BufferLayout& layout)
    {
		static bool first_time = true;
		if (first_time)
		{
			CreateVSSceneBuffer();
			first_time = false;
		}

        return Ref<DX11Shader>::Create(source, layout);
    }

	Ref<Shader> Shader::CreateComputeShader(const ShaderSource& source)
	{
		return Ref<DX11Shader>::Create(source, layout, true);
	}

#endif // HIGHLO_API_DX11

	Ref<UniformBuffer> Shader::GetBuffer(const HLString& name)
	{
		if (m_BufferMap.find(name) != m_BufferMap.end())
			return m_BufferMap.at(name);
		else
			return nullptr;
	}

	void Shader::AddBuffer(const HLString& name, Ref<UniformBuffer> buffer)
	{
		m_BufferMap[name] = buffer;
	}

	void Shader::CreateVSSceneBuffer()
	{
		m_VS_SceneBuffer = UniformBuffer::Create(
			"VS_SceneBuffer",
			{
				UniformVariable("u_ProjectionMatrix", sizeof(glm::mat4)),
				UniformVariable("u_ViewMatrix", sizeof(glm::mat4)),
				UniformVariable("u_CameraPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding01", sizeof(float)),
				UniformVariable("u_LightPosition", sizeof(glm::vec3)),
				UniformVariable("u_Padding02", sizeof(float)),
				UniformVariable("u_LightColor", sizeof(glm::vec3)),
				UniformVariable("u_Padding03", sizeof(float))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32)HL_UB_SLOT::VS_SCENE_BUFFER
		);
	}

	const HLString Shader::GetDefaultEngineShaderFolder()
	{
		return "assets/shaders/";
	}

	ShaderSource Shader::GetDefaultEngineStaticShaderSource()
	{
		static ShaderSource source;
		static bool first_time = true;

		if (first_time)
		{
#ifdef HIGHLO_API_OPENGL
			source = LoadShaderSource(GetDefaultEngineShaderFolder() + "DefaultShader.glsl");
#endif
#ifdef HIGHLO_API_DX11
			source = LoadShaderSource(GetDefaultEngineShaderFolder() + "DefaultShader.hlsl");
#endif
			first_time = false;
		}

		return source;
	}

	ShaderSource Shader::GetDefaultEngineAnimatedShaderSource()
	{
		static ShaderSource source;
		static bool first_time = true;

		if (first_time)
		{
#ifdef HIGHLO_API_OPENGL
			source = LoadShaderSource(GetDefaultEngineShaderFolder() + "DefaultAnimatedShader.glsl");
#endif
#ifdef HIGHLO_API_DX11
			source = LoadShaderSource(GetDefaultEngineShaderFolder() + "DefaultAnimatedShader.hlsl");
#endif
			first_time = false;
		}

		return source;
	}

    ShaderSource Shader::LoadShaderSource(const HLString& filename)
    {
		ShaderSource result;

		enum class ShaderType
		{
			NONE = -1, VERTEX_SHADER = 0, TESS_CONTROL_SHADER = 2, TESS_EVAL_SHADER = 3, GEOMETRY_SHADER = 4, PIXEL_SHADER = 5, COMPUTE_SHADER = 6
		};

		std::unordered_map<ShaderType, std::stringstream> shader_sources;
		shader_sources[ShaderType::VERTEX_SHADER] = std::stringstream();
		shader_sources[ShaderType::TESS_CONTROL_SHADER] = std::stringstream();
		shader_sources[ShaderType::TESS_EVAL_SHADER] = std::stringstream();
		shader_sources[ShaderType::GEOMETRY_SHADER] = std::stringstream();
		shader_sources[ShaderType::PIXEL_SHADER] = std::stringstream();
		shader_sources[ShaderType::COMPUTE_SHADER] = std::stringstream();

		std::ifstream file;
		try
		{
			file.open(filename);

			std::string line;
			ShaderType type = ShaderType::NONE;

			while (std::getline(file, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = ShaderType::VERTEX_SHADER;
					else if (line.find("tess_control") != std::string::npos)
						type = ShaderType::TESS_CONTROL_SHADER;
					else if (line.find("tess_eval") != std::string::npos)
						type = ShaderType::TESS_EVAL_SHADER;
					else if (line.find("geometry") != std::string::npos)
						type = ShaderType::GEOMETRY_SHADER;
					else if (line.find("pixel") != std::string::npos)
						type = ShaderType::PIXEL_SHADER;
					else if (line.find("compute") != std::string::npos)
						type = ShaderType::COMPUTE_SHADER;
				}
				else
				{
					shader_sources[type] << line << "\n";
				}
			}
		}
		catch (std::ifstream::failure e)
		{
			HL_CORE_ERROR("Failed to load shader: " + filename);
			HL_CORE_ERROR(strerror(errno));
		}

		result.m_VertexShaderSrc					= HLString(shader_sources[ShaderType::VERTEX_SHADER].str().c_str());
		result.m_TessellationControlShaderSrc		= HLString(shader_sources[ShaderType::TESS_CONTROL_SHADER].str().c_str());
		result.m_TessellationEvaluationShaderSrc	= HLString(shader_sources[ShaderType::TESS_EVAL_SHADER].str().c_str());
		result.m_GeometryShaderSrc					= HLString(shader_sources[ShaderType::GEOMETRY_SHADER].str().c_str());
		result.m_PixelShaderSrc						= HLString(shader_sources[ShaderType::PIXEL_SHADER].str().c_str());
		result.m_ComputeShaderSrc					= HLString(shader_sources[ShaderType::COMPUTE_SHADER].str().c_str());

		return result;
    }
}
