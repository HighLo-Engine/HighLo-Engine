#include "Renderer2DTest.h"

void Renderer2DTest::OnInitialize()
{
	m_Camera = OrthographicCamera::Create(1270.0f, 940.0f);
}

void Renderer2DTest::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Transform transform;
	transform.FromPosition({ 0.0f, 0.0f, 0.0f });

	Renderer2D::BeginScene(m_Camera->GetProjection() * m_Camera->GetViewMatrix(), true);
	Renderer2D::DrawQuad(transform.GetTransform(), { 0.8f, 0.2f, 0.3f, 1.0f });
	Renderer2D::EndScene();

}

void Renderer2DTest::OnShutdown()
{
}

void Renderer2DTest::OnEvent(Event &e)
{
	m_Camera->OnEvent(e);
}

