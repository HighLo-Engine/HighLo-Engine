#include "Sandbox.h"

void Sandbox::OnInitialize()
{
	HL_INFO("Sandbox Initialized");
}

void Sandbox::OnUpdate(Timestep timestep)
{
}

void Sandbox::OnShutdown()
{
}

void Sandbox::OnEvent(Event& e)
{
	HL_INFO(e.ToString());
}
