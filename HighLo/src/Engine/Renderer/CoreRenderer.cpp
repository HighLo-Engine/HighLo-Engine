// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.


#include "HighLoPch.h"
#include "CoreRenderer.h"

#include "Renderer.h"
#include "Engine/Factories/MeshFactory.h"

#if 0

namespace highlo
{
	CoreRenderer::SceneData CoreRenderer::s_SceneData = { glm::mat4(1.0f), glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };

	Ref<Material>	CoreRenderer::s_DefaultMaterial = nullptr;
	Ref<Mesh>		CoreRenderer::s_CubeMesh = nullptr;
	Ref<Mesh>		CoreRenderer::s_SphereMesh = nullptr;
	Ref<Mesh>		CoreRenderer::s_CapsuleMesh = nullptr;

	void CoreRenderer::Init()
	{
		s_DefaultMaterial = Material::Create();
		s_DefaultMaterial->IsUsingPBR = false;

		s_CubeMesh = MeshFactory::CreateCube({ 1.0f, 1.0f, 1.0f });
		s_SphereMesh = MeshFactory::CreateSphere(1.0f);
		s_CapsuleMesh = MeshFactory::CreateCapsule(1.0f, 5.0f);

		HL_CORE_INFO("Core Renderer Initialized");
	}

	void CoreRenderer::Shutdown() {}

	void CoreRenderer::BeginScene(const ICamera &camera, const Light &light)
	{
		s_SceneData.m_ProjectionMatrix	= camera.GetProjection();
		s_SceneData.m_ViewMatrix		= camera.GetViewMatrix();
		s_SceneData.m_CameraPosition	= camera.GetPosition();
		s_SceneData.m_LightPosition		= light.Position;
		s_SceneData.m_LightColor		= light.Color * light.Intensity;

		auto scene_buffer = s_DefaultMaterial->m_StaticShader->GetVSSceneUniformBuffer();
		scene_buffer->SetBufferValue(&s_SceneData);
		scene_buffer->UploadToShader();
	}

	void CoreRenderer::DrawCube(const glm::vec3 &position, float size, float rotation, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.z = color.z;

		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_CubeMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawCube(const glm::vec3 &position, Ref<Material> material, float size, float rotation)
	{
		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_CubeMesh, material, transform);
	}

	void CoreRenderer::DrawCube(const Transform &transform, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.z = color.z;

		RenderMesh(s_CubeMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawCube(const Transform &transform, Ref<Material> material)
	{
		RenderMesh(s_CubeMesh, material, transform);
	}

	void CoreRenderer::DrawSphere(const glm::vec3 &position, float size, float rotation, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.z = color.z;

		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_SphereMesh, s_DefaultMaterial, transform);
	}
	
	void CoreRenderer::DrawSphere(const glm::vec3 &position, Ref<Material> material, float size, float rotation)
	{
		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_SphereMesh, material, transform);
	}

	void CoreRenderer::DrawSphere(const Transform &transform, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.z = color.z;

		RenderMesh(s_SphereMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawSphere(const Transform &transform, Ref<Material> material)
	{
		RenderMesh(s_SphereMesh, material, transform);
	}

	void CoreRenderer::DrawCapsule(const glm::vec3 &position, float size, float rotation, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.z = color.z;

		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_CapsuleMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawCapsule(const glm::vec3 &position, Ref<Material> material, float size, float rotation)
	{
		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_CapsuleMesh, material, transform);
	}

	void CoreRenderer::DrawCapsule(const Transform &transform, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.Color.z = color.z;

		RenderMesh(s_CapsuleMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawCapsule(const Transform &transform, Ref<Material> material)
	{
		RenderMesh(s_CapsuleMesh, material, transform);
	}

	void CoreRenderer::DrawMesh(const Ref<Mesh> &mesh, const Transform &transform)
	{
		RenderMesh(mesh, mesh->GetMaterial(), transform);
	}

	void CoreRenderer::DrawRawMeshData(Ref<Mesh> &mesh)
	{
		RenderVertexArray(mesh->GetVertexArray());
	}

	void CoreRenderer::RenderVertexArray(Ref<VertexArray> &va)
	{
		va->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(va);
	}

	Ref<Shader> CoreRenderer::SelectMaterialShader(Ref<Material> material, bool animated)
	{
		if (material->IsUsingPBR)
		{
			if (animated)
				return material->m_AnimatedShaderPBR;
			else
				return material->m_StaticShaderPBR;
		}
		else
		{
			if (animated)
				return material->m_AnimatedShader;
			else
				return material->m_StaticShader;
		}
	}

	void CoreRenderer::SetObjectDataBuffer(Ref<Shader> shader, const Transform &transform)
	{
		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", (void*)(&transform.GetTransform()[0]));
		object_data_buffer->UploadToShader();
	}

	void CoreRenderer::RenderMesh(Ref<Mesh> mesh, Ref<Material> material, const Transform &transform)
	{
		Ref<Shader> shader = SelectMaterialShader(material, mesh->HasAnimation());
		shader->Bind();
		material->Bind();

		if (mesh->HasAnimation())
		{
			glm::mat4 *boneTransforms = mesh->GetAnimation()->GetCurrentPoseTransforms();

			auto btb = shader->GetBuffer("BoneTransformsBuffer");
			btb->SetBufferValue(&boneTransforms[0][0][0]);
			btb->UploadToShader();
		}

		SetObjectDataBuffer(shader, transform);
		RenderVertexArray(mesh->GetVertexArray());
	}
}

#endif

