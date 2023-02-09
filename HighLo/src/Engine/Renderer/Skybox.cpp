// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Skybox.h"

#include "Engine/Factories/MeshFactory.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/CoreRenderer.h"

namespace highlo
{
	// TODO: Needs Refactor

	static constexpr uint32 SKYBOX_SIZE = 2048;

	Ref<Skybox> Skybox::Create()
	{
		return Ref<Skybox>::Create();
	}
	
	Skybox::Skybox()
	{
		m_Mesh = MeshFactory::CreateCube({ (float)SKYBOX_SIZE, (float)SKYBOX_SIZE, (float)SKYBOX_SIZE }).As<StaticModel>();
		m_Shader = Renderer::GetShaderLibrary()->Get("SkyboxShader");
	}

	void Skybox::Render(ICamera &camera)
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
	}
}
