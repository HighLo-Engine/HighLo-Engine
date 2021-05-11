#include "Renderer2DTest.h"

void Renderer2DTest::OnInitialize()
{
	m_Camera = Ref<Camera>::Create();
	m_Camera->SetOrthographic();
}

void Renderer2DTest::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	Renderer2D::BeginScene(glm::mat4(1.0f), false);
	//Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	//Renderer2D::DrawLine({ 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 5.0f }, { 1.0f, 0.3f, 0.2f, 1.0f });
	Renderer2D::DrawCircle({ 0.0f, 0.0f, 0.0f }, 10.0f, { 1.0f, 0.3f, 0.2f, 1.0f }, 1.0f);
	Renderer2D::EndScene();

}

void Renderer2DTest::OnShutdown()
{
}

void Renderer2DTest::OnEvent(Event &e)
{
	m_Camera->OnEvent(e);
}

