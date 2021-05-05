#include "HighLoPch.h"
#include "Material.h"
#include "Engine/Math/HLMath.h"

namespace highlo
{
	Ref<Material> Material::Create(MaterialProperties props)
	{
		return Ref<Material>(new Material(props));
	}

	Material::Material(MaterialProperties props)
		: Properties(props)
	{
		CreateShaders();
		ApplyNewProperties();
	}

	void Material::CreateShaders()
	{
		BufferLayout static_layout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 }
		};

		BufferLayout animated_layout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 },
			{ "TANGENT"	, ShaderDataType::Float3 },
			{ "BINORMAL", ShaderDataType::Float3 },
			{ "BONE_IDS", ShaderDataType::Int4 },
			{ "BONE_WEIGHTS", ShaderDataType::Float4 }
		};

		m_StaticShader    = Shader::Create(Shader::GetDefaultEngineStaticShaderSource(), static_layout);
		m_AnimatedShader  = Shader::Create(Shader::GetDefaultEngineAnimatedShaderSource(), animated_layout);

		auto VS_SceneBuffer = UniformBuffer::Create(
			"VS_SceneBuffer",
			{
				UniformVariable("u_ProjectionMatrix", sizeof(glm::mat4)),
				UniformVariable("u_ViewMatrix", sizeof(glm::mat4)),
				UniformVariable("u_CameraPosition", sizeof(glm::vec3))
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32)HL_UB_SLOT::VS_SCENE_BUFFER
		);

		m_StaticShader->AddBuffer("VS_SceneBuffer", VS_SceneBuffer);
		m_AnimatedShader->AddBuffer("VS_SceneBuffer", VS_SceneBuffer);

		auto VS_ObjectBuffer = UniformBuffer::Create(
			"VS_ObjectBuffer",
			{
				UniformVariable("u_Transform", sizeof(glm::mat4)),
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32)HL_UB_SLOT::VS_OBJECT_BUFFER
		);

		m_StaticShader->AddBuffer("VS_ObjectBuffer", VS_ObjectBuffer);
		m_AnimatedShader->AddBuffer("VS_ObjectBuffer", VS_ObjectBuffer);

		m_MaterialDataBuffer = UniformBuffer::Create(
			"MaterialDataBuffer",
			{
				UniformVariable("u_Color", sizeof(glm::vec4)),
				UniformVariable("u_Roughness", sizeof(float)),
			},
			UniformBufferParentShader::PIXEL_SHADER,
			(uint32)HL_UB_SLOT::MATERIAL_DATA_BUFFER
		);

		m_StaticShader->AddBuffer("MaterialDataBuffer", m_MaterialDataBuffer);
		m_AnimatedShader->AddBuffer("MaterialDataBuffer", m_MaterialDataBuffer);
	}

	void Material::ApplyNewProperties()
	{
		m_MaterialDataBuffer->SetBufferValue(&Properties.m_RenderProperties);
		m_MaterialDataBuffer->UploadToShader();
	}
}
