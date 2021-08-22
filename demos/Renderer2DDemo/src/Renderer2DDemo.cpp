#include "Renderer2DDemo.h"

void Renderer2DDemo::OnInitialize()
{
	m_Camera = Ref<Camera>::Create();
	m_Camera->SetOrthographic();
	GetWindow().CenterWindow();
	m_Texture = Texture2D::LoadFromFile("assets/textures/Pistol.png").As<Texture2D>();
}

void Renderer2DDemo::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	Renderer2D::BeginScene(m_Camera->GetViewProjectionMatrix());
	Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Renderer2D::DrawQuad({ 2.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, m_Texture);
	Renderer2D::EndScene();
}

void Renderer2DDemo::OnShutdown()
{
}

void Renderer2DDemo::OnEvent(Event &e)
{
	m_Camera->OnEvent(e);
}

void Renderer2DDemo::OnUIRender(Timestep timestep)
{
}

void Renderer2DDemo::OnResize(uint32 width, uint32 height)
{
	m_Camera->OnWindowResize(width, height);
}

