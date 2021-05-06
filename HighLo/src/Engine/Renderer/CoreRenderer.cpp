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
		s_SphereMesh = MeshFactory::CreateSphere(18.0f);
		s_CapsuleMesh = MeshFactory::CreateCapsule(1.0f, 10.0f);

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
		Ref<Shader> shader = s_DefaultMaterial->m_StaticShader;
		shader->Bind();

		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;
		s_DefaultMaterial->Bind(true);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size, size, size });

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		s_CubeMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(s_CubeMesh->GetVertexArray());
	}

	void CoreRenderer::DrawCube(const glm::vec3& position, Ref<Material> material, float size, float rotation)
	{
		Ref<Shader> shader = s_DefaultMaterial->m_StaticShader;
		shader->Bind();

		material->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size, size, size });

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		s_CubeMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(s_CubeMesh->GetVertexArray());
	}

	void CoreRenderer::DrawSphere(const glm::vec3& position, float size, float rotation, const glm::vec3& color)
	{
		Ref<Shader> shader = s_DefaultMaterial->m_StaticShader;
		shader->Bind();

		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;
		s_DefaultMaterial->Bind(true);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size, size, size });

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		s_SphereMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(s_SphereMesh->GetVertexArray());
	}
	
	void CoreRenderer::DrawSphere(const glm::vec3& position, Ref<Material> material, float size, float rotation)
	{
		Ref<Shader> shader = s_DefaultMaterial->m_StaticShader;
		shader->Bind();

		material->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size, size, size });

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		s_SphereMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(s_SphereMesh->GetVertexArray());
	}

	void CoreRenderer::DrawCapsule(const glm::vec3 &position, float size, float rotation, const glm::vec3 &color)
	{
		Ref<Shader> shader = s_DefaultMaterial->m_StaticShader;
		shader->Bind();

		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.x = color.x;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.y = color.y;
		s_DefaultMaterial->Properties.m_RenderProperties.m_Color.z = color.z;
		s_DefaultMaterial->Bind(true);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size, size, size });

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		s_CapsuleMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(s_CapsuleMesh->GetVertexArray());
	}

	void CoreRenderer::DrawCapsule(const glm::vec3 &position, Ref<Material> material, float size, float rotation)
	{
		Ref<Shader> shader = s_DefaultMaterial->m_StaticShader;
		shader->Bind();

		material->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			                * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 })
			                * glm::scale(glm::mat4(1.0f), { size, size, size });

		auto object_data_buffer = shader->GetBuffer("VS_ObjectBuffer");
		object_data_buffer->SetVariableValue("u_Transform", &transform);
		object_data_buffer->UploadToShader();

		s_CapsuleMesh->GetVertexArray()->Bind();
		Renderer::s_RenderingAPI->DrawIndexed(s_CapsuleMesh->GetVertexArray());
	}
}
