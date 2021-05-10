#include "Sandbox.h"

struct HealthComponent
{
	int Health = 100;
};

void Sandbox::OnInitialize()
{
	// TODO: Fix for GLFW configuration.
	//HLApplication::Get().GetWindow().SetWindowIcon("assets/textures/HighLoEngine.ico");

	VirtualFileSystem::Get()->Mount("textures", "assets/textures/");
	VirtualFileSystem::Get()->Mount("models", "assets/models/");

	m_Camera = Ref<FPSCamera>::Create();

	auto health_comp = m_Entity.AddComponent<HealthComponent>();
	HL_TRACE(health_comp->Health);

	health_comp->Health = 40;

	HL_TRACE(m_Entity.GetComponent<HealthComponent>()->Health);
	

	ImGuiRenderer::ShouldDisplayDebugInformation(true);
	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	m_Camera->Update();

	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

	CoreRenderer::BeginScene(*m_Camera, m_Light);
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

void Sandbox::OnResize(uint32 width, uint32 height)
{
	m_Camera->OnWindowResize(width, height);
}
