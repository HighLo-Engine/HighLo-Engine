#include "Sandbox.h"

void Sandbox::OnInitialize()
{
	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
	Renderer::ClearScreenBuffers();
	Renderer::ClearScreenColor(glm::vec4(0.2f, 0.06f, 0.06f, 1.0f));
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
