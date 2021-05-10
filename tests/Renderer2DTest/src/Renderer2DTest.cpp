#include "Renderer2DTest.h"

void Renderer2DTest::OnInitialize()
{
	m_Camera = Ref<Camera>::Create();
	m_Camera->SetOrthographic();
}

void Renderer2DTest::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer2D::BeginScene(m_Camera->GetViewProjectionMatrix());
	Renderer2D::DrawLine({ 0.0f, 0.0f, 0.0f }, { 50.0f, 50.0f, 50.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Renderer2D::EndScene();

}

void Renderer2DTest::OnShutdown()
{
}

void Renderer2DTest::OnEvent(Event &e)
{
	m_Camera->OnEvent(e);
}

