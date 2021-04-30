#include "Sandbox.h"

void Sandbox::OnInitialize()
{
	HL_TRACE("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
}

void Sandbox::OnShutdown()
{
	HL_TRACE("Sandbox Shutdown");
}

void Sandbox::OnEvent(Event& e)
{
	if (e.IsInCategory(EventCategory::EventCategoryWindow))
		HL_TRACE(e.ToString());
}
