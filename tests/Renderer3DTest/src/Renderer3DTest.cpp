#include "Renderer3DTest.h"

void Renderer3DTest::OnInitialize()
{
	m_Camera = Ref<FPSCamera>::Create();

	Renderer::SetWireframe(true);
	ImGuiRenderer::ShouldDisplayDebugInformation(true);
}

void Renderer3DTest::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	CoreRenderer::BeginScene(*m_Camera);
	CoreRenderer::DrawCube({ -1, 0, 5 }, 2.0f, 0.0f, { 0.2f, 1.0f, 0.6f });
	CoreRenderer::DrawSphere({ 6, 0, 6 }, 2.0f, 0.0f, { 0.2f, 1.0f, 0.6f });
	CoreRenderer::DrawCapsule({ 2, 0, 8 }, 1.0f, 30.0f, { 1.0f, 0.3f, 0.2f });
}

void Renderer3DTest::OnShutdown()
{
}

void Renderer3DTest::OnEvent(Event &e)
{
	if (e.IsInCategory(EventCategory::EventCategoryApplication))
		HL_TRACE(e.ToString());
}
