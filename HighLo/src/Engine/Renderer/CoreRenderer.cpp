#include "HighLoPch.h"
#include "CoreRenderer.h"

#include "Renderer.h"
#include "MeshFactory.h"

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
		s_DefaultMaterial->AddTexture(Texture2D::CreateFromColor({ 255, 255, 255 }));

		s_CubeMesh = MeshFactory::CreateCube({ 1.0f, 1.0f, 1.0f });
		s_SphereMesh = MeshFactory::CreateSphere(1);
		s_CapsuleMesh = MeshFactory::CreateCapsule(1.0f, 5.0f);

		HL_CORE_INFO("Core Renderer Initialized");
	}

	void CoreRenderer::Shutdown() {}

	void CoreRenderer::BeginScene(const ICamera& camera)
	{
		s_SceneData.m_ProjectionMatrix	= camera.GetProjection();
		s_SceneData.m_ViewMatrix		= camera.GetViewMatrix();
		s_SceneData.m_CameraPosition	= camera.GetPosition();

		auto scene_buffer = s_DefaultMaterial->m_StaticShader->GetBuffer("VS_SceneBuffer");
		scene_buffer->SetBufferValue(&s_SceneData);
		scene_buffer->UploadToShader();
	}
	
	void CoreRenderer::EndScene() {}

	void CoreRenderer::DrawCube(const glm::vec3& position, float size, float rotation, const glm::vec3& color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;

		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_CubeMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawCube(const glm::vec3& position, Ref<Material> material, float size, float rotation)
	{
		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_CubeMesh, material, transform);
	}

	void CoreRenderer::DrawCube(const Transform &transform, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;

		RenderMesh(s_CubeMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawCube(const Transform &transform, Ref<Material> material)
	{
		RenderMesh(s_CubeMesh, material, transform);
	}

	void CoreRenderer::DrawSphere(const glm::vec3& position, float size, float rotation, const glm::vec3& color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;

		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_SphereMesh, s_DefaultMaterial, transform);
	}
	
	void CoreRenderer::DrawSphere(const glm::vec3& position, Ref<Material> material, float size, float rotation)
	{
		Transform transform;
		transform.Scale({ size, size, size });
		transform.Rotate(rotation, { 0, 0, 1 });
		transform.Translate(position);

		RenderMesh(s_SphereMesh, material, transform);
	}

	void CoreRenderer::DrawSphere(const Transform &transform, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;

		RenderMesh(s_SphereMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawSphere(const Transform &transform, Ref<Material> material)
	{
		RenderMesh(s_SphereMesh, material, transform);
	}

	void CoreRenderer::DrawCapsule(const glm::vec3 &position, float size, float rotation, const glm::vec3 &color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;

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

	void CoreRenderer::DrawCapsule(const Transform& transform, const glm::vec3& color)
	{
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;

		RenderMesh(s_CapsuleMesh, s_DefaultMaterial, transform);
	}

	void CoreRenderer::DrawCapsule(const Transform& transform, Ref<Material> material)
	{
		RenderMesh(s_CapsuleMesh, material, transform);
	}

	void CoreRenderer::DrawMesh(const Ref<Mesh>& mesh, const Transform& transform)
	{
		RenderMesh(mesh, mesh->material, transform);
	}

	void CoreRenderer::RenderVertexArray(Ref<VertexArray>& va)
	{
		va->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(va);
	}

	Ref<Shader> CoreRenderer::SelectMaterialShader(Ref<Material> material, bool animated)
	{
		return material->m_StaticShader;
	}

	void CoreRenderer::SetObjectDataBuffer(Ref<Shader> shader, const Transform& transform)
	{
		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", (void*)(&transform.GetTransform()[0]));
		object_data_buffer->UploadToShader();
	}

	void CoreRenderer::RenderMesh(Ref<Mesh> mesh, Ref<Material> material, const Transform& transform)
	{
		Ref<Shader> shader = SelectMaterialShader(material, (mesh->animation != nullptr));
		shader->Bind();
		material->Bind();

		SetObjectDataBuffer(shader, transform);
		RenderVertexArray(mesh->GetVertexArray());
	}
}
