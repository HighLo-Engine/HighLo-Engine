#include "HighLoPch.h"
#include "CoreRenderer.h"

#include "Renderer.h"

namespace highlo
{
	CoreRenderer::SceneData CoreRenderer::m_SceneData = { glm::mat4(1.0f), glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };

	Ref<Material>	CoreRenderer::m_DefaultMaterial = nullptr;
	Ref<Mesh>		CoreRenderer::m_CubeMesh = nullptr;
	Ref<Mesh>		CoreRenderer::m_SphereMesh = nullptr;

	void CoreRenderer::Init()
	{
		m_DefaultMaterial = Material::Create();
		m_DefaultMaterial->AddTexture(Texture2D::CreateFromColor({ 255, 255, 255 }));

		m_CubeMesh = Mesh::Create(Shapes::_3D::Cube);
		m_SphereMesh = Mesh::Create(Shapes::_3D::Sphere);

		HL_CORE_INFO("Core Renderer Initialized");
	}

	void CoreRenderer::Shutdown() {}

	void CoreRenderer::BeginScene(const ICamera& camera)
	{
		m_SceneData.m_ProjectionMatrix	= camera.GetProjection();
		m_SceneData.m_ViewMatrix		= camera.GetViewMatrix();
		m_SceneData.m_CameraPosition	= camera.GetPosition();

		auto scene_buffer = m_DefaultMaterial->m_StaticShader->GetBuffer("VS_SceneBuffer");
		scene_buffer->SetBufferValue(&m_SceneData);
		scene_buffer->UploadToShader();
	}
	
	void CoreRenderer::EndScene() {}

	void CoreRenderer::DrawCube(const glm::vec3& position, float size, const glm::vec3& color)
	{
		Ref<Shader> shader = m_DefaultMaterial->m_StaticShader;
		shader->Bind();

		m_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		m_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		m_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;
		m_DefaultMaterial->Bind(true);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, size, size));

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		m_CubeMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(m_CubeMesh->GetVertexArray());
	}

	void CoreRenderer::DrawCube(const glm::vec3& position, float size, Ref<Material> material)
	{
		Ref<Shader> shader = m_DefaultMaterial->m_StaticShader;
		shader->Bind();

		material->Bind();

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, size, size));

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		m_CubeMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(m_CubeMesh->GetVertexArray());
	}

	void CoreRenderer::DrawSphere(const glm::vec3& position, float size, const glm::vec3& color)
	{
		Ref<Shader> shader = m_DefaultMaterial->m_StaticShader;
		shader->Bind();

		m_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		m_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		m_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;
		m_DefaultMaterial->Bind(true);

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, size, size));

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		m_SphereMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(m_SphereMesh->GetVertexArray());
	}
	
	void CoreRenderer::DrawSphere(const glm::vec3& position, float size, Ref<Material> material)
	{
		Ref<Shader> shader = m_DefaultMaterial->m_StaticShader;
		shader->Bind();

		material->Bind();

		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, position);
		transform = glm::scale(transform, glm::vec3(size, size, size));

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		m_SphereMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(m_SphereMesh->GetVertexArray());
	}
}
