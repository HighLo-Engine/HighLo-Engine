#include "Sandbox.h"

void Sandbox::OnInitialize()
{
	// TO-DO: Fix for GLFW configuration.
	//HLApplication::Get().GetWindow().SetWindowIcon("assets/textures/HighLoEngine.ico");

	m_Camera = Ref<FPSCamera>::Create();
	m_Camera->SetYaw(90);
	
	Renderer::SetWireframe(true);
	ImGuiRenderer::ShouldDisplayDebugInformation(true);

	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.2f, 0.06f, 0.06f, 1.0f));

	CoreRenderer::BeginScene(*m_Camera);
	CoreRenderer::DrawCube({ -1, 0, 5 }, 2.0f, { 0.2f, 1.0f, 0.6f });
	CoreRenderer::DrawSphere({ 6, 0, 6 }, 2.0f, { 0.2f, 1.0f, 0.6f });
	CoreRenderer::EndScene();
}

void Sandbox::OnShutdown()
{
	HL_TRACE("Sandbox Shutdown");
}

void Sandbox::OnEvent(Event& e)
{
	if (e.IsInCategory(EventCategory::EventCategoryApplication))
		HL_TRACE(e.ToString());
}

void Sandbox::OnImGuiRender()
{
}
