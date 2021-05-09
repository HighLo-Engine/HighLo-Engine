#include "HighLoPch.h"
#include "Skybox.h"

#include "Engine/Renderer/MeshFactory.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/CoreRenderer.h"

namespace highlo
{
	static constexpr uint32 SKYBOX_SIZE = 2000;

	Ref<Skybox> Skybox::Create()
	{
		return Ref<Skybox>(new Skybox());
	}
	
	Skybox::Skybox()
	{
		CreateMesh();
		CreateShader();
	}

	void Skybox::Render(ICamera& camera)
	{
		if (!m_Texture)
		{
			HL_CORE_WARN("Skybox>  [-] Failed to Render | Missing Texture [-]");
			return;
		}

		m_Texture->Bind(0);
		m_Shader->Bind();

		m_SceneData.m_ProjectionMatrix = camera.GetProjection();
		m_SceneData.m_ViewMatrix = camera.GetViewMatrix();

		m_Shader->GetVSSceneUniformBuffer()->SetBufferValue(&m_SceneData);
		m_Shader->GetVSSceneUniformBuffer()->UploadToShader();

		CoreRenderer::DrawRawMeshData(m_Mesh);
	}

	void Skybox::CreateMesh()
	{
		m_Mesh = MeshFactory::CreateCube({ (float)SKYBOX_SIZE, (float)SKYBOX_SIZE, (float)SKYBOX_SIZE });
	}

	void Skybox::CreateShader()
	{
		m_Shader = Renderer::GetShaderLibrary()->Get("SkyboxShader");
		m_Shader->AddBuffer("VS_SceneBuffer", Shader::GetVSSceneUniformBuffer());
	}
}
