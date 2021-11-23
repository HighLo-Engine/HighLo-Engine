// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Shader.h"

#ifdef HIGHLO_API_OPENGL
#include "Engine/Platform/OpenGL/OpenGLShader.h"
#elif HIGHLO_API_DX11
#include "Engine/Platform/DX11/DX11Shader.h"
#elif HIGHLO_API_VULKAN
#include "Engine/Platform/Vulkan/VulkanShader.h"
#endif // HIGHLO_API_OPENGL

#include "Engine/Math/Math.h"

namespace highlo
{
	Ref<UniformBuffer> Shader::m_VS_SceneBuffer;

    Ref<Shader> Shader::Create(const ShaderSource &source, const BufferLayout &layout)
    {
		static bool first_time = true;
		if (first_time)
		{
			CreateVSSceneBuffer();
			first_time = false;
		}

        return Ref<OpenGLShader>::Create(source);
    }

	Ref<UniformBuffer> Shader::GetBuffer(const HLString &name)
	{
		if (m_BufferMap.find(name) != m_BufferMap.end())
			return m_BufferMap.at(name);
			
		return nullptr;
	}

	void Shader::AddBuffer(const HLString &name, Ref<UniformBuffer> buffer)
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
}

